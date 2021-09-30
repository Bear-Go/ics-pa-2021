#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>

word_t expr(char *e, bool *success);
int set_wp(char *e);
bool delete_wp(int NO);
void list_wp();
void scan_wp(vaddr_t pc);
#endif
