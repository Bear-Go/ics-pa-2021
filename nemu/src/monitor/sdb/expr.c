#include <isa.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
	 TK_NOTYPE = 256, TK_NUM, TK_HEXNUM,
	 TK_EQ, TK_NEQ, TK_AND, TK_REG,
	 TK_REF, TK_NEG
	 /* TODO: Add more token types */
};

static struct rule {
	const char *regex;
	int token_type;
} rules[] = {
	/* TODO: Add more rules.
	* Pay attention to the precedence level of different rules.
	*/
	{"0x[0-9a-fA-F]+",		TK_HEXNUM},			// hex-number
	{"[0-9]+",				TK_NUM},			// number
	{" +", 					TK_NOTYPE},			// spaces
	{"\\+", 				'+'},				// plus
	{"-", 					'-'},				// minus
	{"\\*", 				'*'},				// mul
	{"/", 					'/'},				// div
	{"\\(", 				'('},				// left bracket
	{"\\)", 				')'},				// right bracket
	{"==", 					TK_EQ},				// equal
	{"!=", 					TK_NEQ},			// not equal
	{"&&", 					TK_AND},			// and
	{"\\$[a-zA-Z0-9]+", 	TK_REG}				// reg
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while  (e[position] != '\0') {
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i ++) {
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
					i, rules[i].regex, position, substr_len, substr_len, substr_start);

				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				Add codes to record the token in the array `tokens'. 
				For certain types of tokens, some extra actions should be performed.
				*/

				switch (rules[i].token_type) {
					case TK_NOTYPE: break;
					default: tokens[nr_token].type = rules[i].token_type;
							sprintf(tokens[nr_token].str, "%.*s", substr_len, substr_start);
							++ nr_token;
				}
				break;
			}
      	}
		  
		if (i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}  

	return true;
}

bool *is_exp_right;

static bool check_parentheses(int p, int q) {
	//check whether the brackets on both ends cound be removed
	if (tokens[p].type == '(' && tokens[q].type == ')') {
		int cnt = 0;
		for(++ p; p < q; ++ p) {
			if (tokens[p].type == '(') ++ cnt;
			if (tokens[p].type == ')') -- cnt;
			if (cnt < 0) return false;
		}
	}
	else return false;
	return true;
}

static int main_op(int p, int q) {
	//find the location of main operator
	int priority = 0;
	int cnt = 0, loc = 0;
	for (; p <= q; -- q) {
		printf("\tis %d main op token = %s \n",q,tokens[q].str);
		if (tokens[q].type == ')') -- cnt;
		if (tokens[q].type == '(') ++ cnt;
		if (cnt || tokens[q].type == TK_NUM || tokens[q].type == TK_HEXNUM) continue;
		switch (tokens[q].type) {
			case TK_NEG:
				if (priority < 2) loc = q, priority = 2;
				break;
			case '*':case '/':
				if (priority < 3) loc = q, priority = 3;
				break;
			case '+':case '-':
				if (priority < 4) loc = q, priority = 4;
				break;
			case TK_EQ:case TK_NEQ:
				if (priority < 7) loc = q, priority = 7;
				break;
			case TK_AND:
				if (priority < 11) loc = q, priority = 11;
				break;	
			default:
				break;
		} 
	}
	if ( !priority ) {
		*is_exp_right = false;
		return -1;
	}
	return loc;
}

static word_t eval(int p, int q) {
	if ( !*is_exp_right ) return 0;
	printf("time: runnig between p=%d and q=%d\n",p,q);
	if (p > q) {
		//bad expression
		printf("choice: branch p > q\n");
		return 0;
	}
	else if (p == q) {
		//single token should be a number just return the value
		printf("choice: branch p == q\n");
		word_t num;
		switch (tokens[p].type) {
			case TK_NUM		: sscanf(tokens[p].str, "%d", &num); return num;
			case TK_HEXNUM	: sscanf(tokens[p].str, "%x", &num); return num;
 			case TK_REG 	: num = isa_reg_str2val(tokens[p].str+1, is_exp_right);
			 					return ( *is_exp_right ? num : 0);
			default: *is_exp_right = false; printf("Error: p==q and undefined number\n"); return 0;
		}
	}
	else if (check_parentheses(p, q) == true) {
		//remove the pair of brackets
		printf("choice: branch remove brackets\n");
		printf("p=%d and q=%d brackets have been removed\n",p,q);
		return eval(p + 1, q - 1);
	}
	else {
		
		printf("choice: branch find mainop\n");
		int op = main_op(p, q);
		if (op < p || op > q) {
			*is_exp_right = false; 
			printf("Error: main operater not found \n"); 
			return 0;
		}
		printf("main operator is %s\tlocation is %d \n",tokens[op].str,op);
		word_t val1 = eval(p, op - 1);
		word_t val2 = eval(op + 1, q);
		switch (tokens[op].type) {
			case '+'	: return val1 + val2;
			case '-'	: return val1 - val2;
			case '*'	: return val1 * val2;
			case '/'	: if (val2) {return val1 / val2;} else {*is_exp_right = false; printf("Error: division by zero\n"); return 0;}
			case TK_EQ	: return val1 == val2;
			case TK_NEQ : return val1 != val2;
			case TK_AND : return val1 && val2;
			case TK_NEG : return -val2;
			default: assert(0);
		}
	}
	printf("In the eval wrong branch\n");
	return 0;
}

static bool is_brackets_match(int p, int q) {
	int cnt = 0;
	for (; p <= q; ++ p) {
		if (tokens[p].type == '(') ++ cnt;
		if (tokens[p].type == ')') -- cnt;
		if (cnt < 0) return false;
	}
	return !cnt;
}

static int modify_token(int p, int q) {
	for (int i = p; i <= q; ++ i) {
		bool is_pre_type_sign = tokens[i-1].type != TK_NUM && 
								tokens[i-1].type != TK_HEXNUM && 
								tokens[i-1].type != TK_REG && 
								tokens[i-1].type != ')';
		if (tokens[i].type == '*' && (i == p || is_pre_type_sign ) ) {
			tokens[i].type = TK_REF;
			printf("i = %d: * have been changed to pointer\n",i);
		}
		if (tokens[i].type == '-' && (i == p || is_pre_type_sign ) ) {
			tokens[i].type = TK_NEG;
			printf("i = %d: - have been changed to neg\n",i);
		}
	}
	return 0;
}

word_t expr(char *e, bool *success) {
	printf("Expression is:%s \n",e);
	if ( !make_token(e) ) {
		*success = false;
		printf("Error: make_token() mistake\n");
		return 0;
	}
	printf("Massage: finish make_token()\n");
	/* TODO: Insert codes to evaluate the expression. */
	is_exp_right = success;
	if ( !is_brackets_match(0, nr_token - 1) ) {
		*is_exp_right = false;
		printf("Error: is_brackets_match() mistake\n");
		return 0;
	}
	printf("Massage: finish is_brackets_match()\n");
	modify_token(0, nr_token - 1);
	printf("Massage: finish modify_token()\n");
	return eval(0, nr_token - 1);
}