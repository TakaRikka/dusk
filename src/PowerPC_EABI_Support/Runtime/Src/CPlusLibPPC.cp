#include "CPlusLibPPC.h"

extern "C" void* __copy(char *dest, char *src, size_t size) {
	char *p;
	
	if (dest && size) {
		p = dest;
		do {
			*p = *src;
			++p;
			++src;
			--size;
		} while (size);
	}
	
	return(dest);
}
