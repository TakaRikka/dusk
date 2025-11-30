#include "extras.h"
#include "ctype.h"

int stricmp(const char* str1, const char* str2) {
	char a_var;
	char b_var;

	do {
		b_var = _tolower(*str1++);
		a_var = _tolower(*str2++);

		if (b_var < a_var) {
			return -1;
		}
		if (b_var > a_var) {
			return 1;
		}
	} while (b_var != 0);

	return 0;
}

int strnicmp(const char* str1, const char* str2, int n) {
    return __msl_strnicmp(str1, str2, n);
}
