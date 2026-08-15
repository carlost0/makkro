#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>
#include "sv.h"

unsigned int str_to_uint(const char *str);
int   is_num(const char *str);
str_view_t file_to_sv(const char *filename);           

#endif /* UTIL_H_ */
