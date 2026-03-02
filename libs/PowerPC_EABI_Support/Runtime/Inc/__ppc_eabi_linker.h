#ifndef __PPC_EABI_LINKER
#define __PPC_EABI_LINKER

extern char _stack_addr[];
extern char _stack_end[];
extern char _heap_addr[];
extern char _heap_end[];
extern const char _fextabindex_rom[];
extern char _fextabindex[];
extern char _eextabindex[];

extern char _SDA_BASE_[];
extern char _SDA2_BASE_[];

typedef struct __rom_copy_info {
    char* rom;
    char* addr;
    unsigned int size;
} __rom_copy_info;

extern __rom_copy_info _rom_copy_info[];

typedef struct __bss_init_info {
    char* addr;
    unsigned int size;
} __bss_init_info;

extern __bss_init_info _bss_init_info[];

typedef struct __eti_init_info {
    void* eti_start;
    void* eti_end;
    void* code_start;
    unsigned long code_size;
} __eti_init_info;

extern __eti_init_info _eti_init_info[];
extern const char _f_init_rom[];
extern char _f_init[];
extern char _e_init[];
extern const char _f_text_rom[];
extern char _f_text[];
extern char _e_text[];
extern const char _f_rodata_rom[];
extern char _f_rodata[];
extern char _e_rodata[];
extern const char _fextab_rom[];
extern char _fextab[];
extern char _eextab[];
extern const char _f_data_rom[];
extern char _f_data[];
extern char _e_data[];
extern char _f_bss[];
extern char _e_bss[];
extern const char _f_sdata_rom[];
extern char _f_sdata[];
extern char _e_sdata[];
extern char _f_sbss[];
extern char _e_sbss[];
extern const char _f_sdata2_rom[];
extern char _f_sdata2[];
extern char _e_sdata2[];
extern char _f_sbss2[];
extern char _e_sbss2[];
extern const char _f_PPC_EMB_sdata0_rom[];
extern char _f_PPC_EMB_sdata0[];
extern char _e_PPC_EMB_sdata0[];
extern char _f_PPC_EMB_sbss0[];
extern char _e_PPC_EMB_sbss0[];

#endif  // __PPC_EABI_LINKER