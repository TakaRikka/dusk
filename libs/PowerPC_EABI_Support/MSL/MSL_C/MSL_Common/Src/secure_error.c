#include <cstdio>

typedef void (*constraint_handler_t)(const char*, void*, int);

static constraint_handler_t __msl_constraint_handler = NULL;

void ignore_handler_s(const char* msg, void* ptr, int error);

void __msl_runtime_constraint_violation_s(const char* msg, void* ptr, int error) {
    if (__msl_constraint_handler)
        (*__msl_constraint_handler)(msg, ptr, error);
    else
        ignore_handler_s(msg, ptr, error);
}

void ignore_handler_s(const char* msg, void* ptr, int error) {}
