#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>

unsigned int str_to_uint(char *str);
char* trim_str(char *str);
bool  is_num(char *str);
char* file_to_str(char *filename);           

#endif /* UTIL_H_ */
