file(READ "${SOURCE_DIR}/cmake/capstone.cmake.in" _content)

# Insert PATCH_COMMAND before CONFIGURE_COMMAND in the ExternalProject_Add.
# Bracket args prevent cmake from substituting ${...} while writing this file.
string(REPLACE
    "    CONFIGURE_COMMAND \"\""
    [=[    PATCH_COMMAND "${CMAKE_COMMAND}" -DDIR=${CMAKE_CURRENT_BINARY_DIR}/capstone-src -P "${CAPSTONE_FIX_SCRIPT}"
    CONFIGURE_COMMAND ""]=]
    _content "${_content}")

file(WRITE "${SOURCE_DIR}/cmake/capstone.cmake.in" "${_content}")

file(READ "${SOURCE_DIR}/src/funchook_unix.c" _unix_content)

# macOS rejects the POSIX mprotect RWX/RW transition for executable image pages on arm64.
# Use Mach VM_PROT_COPY for the short patch window, then restore RX permissions.
if (NOT _unix_content MATCHES "VM_PROT_READ \\| VM_PROT_WRITE \\| VM_PROT_COPY")
    string(REPLACE
        [=[    rv = mprotect(mstate->addr, mstate->size, prot);]=]
        [=[#ifdef __APPLE__
    kern_return_t kr = vm_protect(mach_task_self(), (vm_address_t)mstate->addr,
                                  (vm_size_t)mstate->size, FALSE,
                                  VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
    if (kr == KERN_SUCCESS) {
        funchook_log(funchook, "  unprotect memory %p (size=%"PRIuPTR", prot=read,write,copy) <- %p (size=%"PRIuPTR")\n",
                     mstate->addr, mstate->size, start, len);
        return 0;
    }
    funchook_set_error_message(funchook, "Failed to unprotect memory %p (size=%"PRIuPTR", prot=read,write,copy) <- %p (size=%"PRIuPTR", error=%s)",
                               mstate->addr, mstate->size, start, len,
                               mach_error_string(kr));
    return FUNCHOOK_ERROR_MEMORY_FUNCTION;
#endif
    rv = mprotect(mstate->addr, mstate->size, prot);]=]
        _unix_content "${_unix_content}")

    string(REPLACE
        [=[    char errbuf[128];
    int rv = mprotect(mstate->addr, mstate->size, PROT_READ | PROT_EXEC);]=]
        [=[    char errbuf[128];
#ifdef __APPLE__
    kern_return_t kr = vm_protect(mach_task_self(), (vm_address_t)mstate->addr,
                                  (vm_size_t)mstate->size, FALSE,
                                  VM_PROT_READ | VM_PROT_EXECUTE);

    if (kr == KERN_SUCCESS) {
        funchook_log(funchook, "  protect memory %p (size=%"PRIuPTR", prot=read,exec)\n",
                     mstate->addr, mstate->size);
        return 0;
    }
    funchook_set_error_message(funchook, "Failed to protect memory %p (size=%"PRIuPTR", prot=read,exec, error=%s)",
                               mstate->addr, mstate->size,
                               mach_error_string(kr));
    return FUNCHOOK_ERROR_MEMORY_FUNCTION;
#endif
    int rv = mprotect(mstate->addr, mstate->size, PROT_READ | PROT_EXEC);]=]
        _unix_content "${_unix_content}")
endif ()

file(WRITE "${SOURCE_DIR}/src/funchook_unix.c" "${_unix_content}")
