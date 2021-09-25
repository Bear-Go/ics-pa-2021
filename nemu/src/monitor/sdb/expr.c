#include <isa.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

//static const int STR_MAX_LEN = 32+1 = 33;
//static const int EXP_MAX_SIZE = 32;

enum {
	 TK_NOTYPE = 256, TK_NUM, TK_EQ,

	 /* TODO: Add more token types */
};

static struct rule {
	const char *regex;
	int token_type;
} rules[] = {
	/* TODO: Add more rules.
	* Pay attention to the precedence level of different rules.
	*/
	{"[0-9]{33}",	TK_NUM},			// number
	{" +", 			TK_NOTYPE},			// spaces
	{"\\+", 		'+'},				// plus
	{"-", 			'-'},				// minus
	{"\\*", 		'*'},				// mul
	{"/", 			'/'},				// div
	{"\\(", 		'('},				// left bracket
	{"\\)", 		')'},				// right bracket
	{"==", 			TK_EQ},				// equal
	{""}
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
	char str[33];
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

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				* to record the token in the array `tokens'. For certain types
				* of tokens, some extra actions should be performed.
				*/

				switch (rules[i].token_type) {
					case TK_NOTYPE: break;
					default: tokens[nr_token].type = rules[i].token_type;
							strncpy(tokens[nr_token].str, substr_start, substr_len);
							tokens[nr_token].str[substr_len] = '\0';
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
	return true;
}

static int main_op(int p, int q) {
	//find the location of main operator
	return 0;
}

static word_t eval(int p, int q) {
	if (! *is_exp_right) return 0;
	if (p > q) {
		//bad expression
		*is_exp_right = false;
		return 0;
	}
	else if (p == q) {
		//single token should be a number just return the value
		word_t num;
		switch (tokens[p].type) {
			case TK_NUM: sscanf(tokens[p].str, "%d", &num); return num;
			default: *is_exp_right = false; return 0;
		}
	}
	else if (check_parentheses(p, q) == true) {
		//remove the pair of brackets
		return eval(p + 1, q - 1);
	}
	else {
		int op = main_op();
		word_t val1 = eval(p, op - 1);
		word_t val2 = eval(op + 1, q);
		switch (tokens[op].type) {
			case '+': return val1+val2;
			case '-': return val1-val2;
			case '*': return val1*val2;
			case '/': if(val2) {return val1/val2;} else {*is_exp_right = false; return 0;}
			default: *is_exp_right = false; return 0;
		}
	}
	printf("In the eval wrong branch\n");
	return 0;
}

word_t expr(char *e, bool *success) {
	if (!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	is_exp_right = success;
	return eval(0, nr_token - 1);
}
