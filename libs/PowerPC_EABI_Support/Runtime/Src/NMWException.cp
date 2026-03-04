#include "NMWException.h"
#include "MWCPlusLib.h"

typedef void (*unexpected_handler)();
unexpected_handler set_unexpected(unexpected_handler handler);
void unexpected();

typedef void (*terminate_handler)();
terminate_handler set_terminate(terminate_handler handler);
void terminate();

#define ARRAY_HEADER_SIZE 16

extern "C" {
extern void abort();
}

namespace std {
static void dthandler() { abort(); }

static terminate_handler thandler = dthandler;

static void duhandler() { terminate(); }

static unexpected_handler uhandler = duhandler;

extern terminate_handler set_terminate(terminate_handler handler) {
	terminate_handler old = thandler;
	thandler              = handler;
	return old;
}

extern void terminate() { thandler(); }

extern unexpected_handler set_unexpected(unexpected_handler handler) {
	unexpected_handler old = uhandler;
	uhandler               = handler;
	return old;
}

extern void unexpected() { uhandler(); }
} // namespace std

extern char __throw_catch_compare(const char* throwtype, const char* catchtype, long* offset_result) {
	const char *cptr1, *cptr2;

	*offset_result = 0;

	if ((cptr2 = catchtype) == 0) {
		return true;
	}

	cptr1 = throwtype;

	if (*cptr2 == 'P') {
		cptr2++;
		if (*cptr2 == 'C')
			cptr2++;
		if (*cptr2 == 'V')
			cptr2++;
		if (*cptr2 == 'v') {
			if (*cptr1 == 'P' || *cptr1 == '*') {
				return true;
			}
		}
		cptr2 = catchtype;
	}

	switch (*cptr1) {
	case '*':
	case '!':
		if (*cptr1++ != *cptr2++)
			return false;
		for (;;) {
			if (*cptr1 == *cptr2++) {
				if (*cptr1++ == '!') {
					long offset;

					for (offset = 0; *cptr1 != '!';) {
						offset = offset * 10 + *cptr1++ - '0';
					}
					*offset_result = offset;
					return true;
				}
			} else {
				while (*cptr1++ != '!') { }
				while (*cptr1++ != '!') { }
				if (*cptr1 == 0)
					return false;

				cptr2 = catchtype + 1;
			}
		}
		return false;
	}

	while ((*cptr1 == 'P' || *cptr1 == 'R') && *cptr1 == *cptr2) {
		cptr1++;
		cptr2++;

		if (*cptr2 == 'C') {
			if (*cptr1 == 'C')
				cptr1++;
			cptr2++;
		}
		if (*cptr1 == 'C')
			return false;

		if (*cptr2 == 'V') {
			if (*cptr1 == 'V')
				cptr1++;
			cptr2++;
		}
		if (*cptr1 == 'V')
			return false;
	}

	for (; *cptr1 == *cptr2; cptr1++, cptr2++) {
		if (*cptr1 == 0)
			return true;
	}

	return false;
}

class __partial_array_destructor {
private:
	void* p;
	size_t size;
	size_t n;
	ConstructorDestructor dtor;

public:
	size_t i;

	__partial_array_destructor(void* array, size_t elementsize, size_t nelements, ConstructorDestructor destructor) {
		p    = array;
		size = elementsize;
		n    = nelements;
		dtor = destructor;
		i    = n;
	}

	~__partial_array_destructor() {
		char* ptr;

		if (i < n && dtor) {
			for (ptr = (char*)p + size * i; i > 0; i--) {
				ptr -= size;
				DTORCALL_COMPLETE(dtor, ptr);
			}
		}
	}
};

extern void* __construct_new_array(void* block, ConstructorDestructor ctor, ConstructorDestructor dtor, size_t size, size_t n) {
	char* ptr;

	if ((ptr = (char*)block) != 0L) {
		size_t* p = (size_t*)ptr;

		p[0] = size;
		p[1] = n;
		ptr += ARRAY_HEADER_SIZE;

		if (ctor) {
			__partial_array_destructor pad(ptr, size, n, dtor);
			char* p;

			for (pad.i = 0, p = (char*)ptr; pad.i < n; pad.i++, p += size) {
				CTORCALL_COMPLETE(ctor, p);
			}
		}
	}
	return ptr;
}

extern void __construct_array(void* ptr, ConstructorDestructor ctor, ConstructorDestructor dtor, size_t size, size_t n) {
	__partial_array_destructor pad(ptr, size, n, dtor);
	char* p;

	for (pad.i = 0, p = (char*)ptr; pad.i < n; pad.i++, p += size) {
		CTORCALL_COMPLETE(ctor, p);
	}
}

extern void __destroy_arr(void* block, ConstructorDestructor* dtor, size_t size, size_t n) {
	char* p;

	for (p = (char*)block + size * n; n > 0; n--) {
		p -= size;
		DTORCALL_COMPLETE(dtor, p);
	}
}

extern void __destroy_new_array(void* block, ConstructorDestructor dtor) {
	if (block) {
		if (dtor) {
			size_t i, objects, objectsize;
			char* p;

			objectsize = *(size_t*)((char*)block - ARRAY_HEADER_SIZE);
			objects    = ((size_t*)((char*)block - ARRAY_HEADER_SIZE))[1];
			p          = (char*)block + (objectsize * objects);

			for (i = 0; i < objects; i++) {
				p -= objectsize;
				DTORCALL_COMPLETE(dtor, p);
			}
		}

		::operator delete[]((char*)block - ARRAY_HEADER_SIZE);
	}
}

void __destroy_new_array2(void) {}

void __destroy_new_array3(void) {}
