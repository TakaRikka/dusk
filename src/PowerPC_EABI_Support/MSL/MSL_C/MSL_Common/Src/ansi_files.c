#include "ansi_files.h"
#include "critical_regions.h"

static unsigned char stdin_buff[0x100];

static unsigned char stdout_buff[0x100];

static unsigned char stderr_buff[0x100];

extern files __files = {
    {
        0,
        0,
        1,
        1,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        {0, 0},
        {0, 0},
        0,
        stdin_buff,
        0x00000100,
        stdin_buff,
        0,
        0,
        0,
        0,
        NULL,
        __read_console,
        __write_console,
        __close_console,
        NULL,
        &__files._stdout,
    },
    {
        1,
        0,
        2,
        1,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        {0, 0},
        {0, 0},
        0,
        stdout_buff,
        0x00000100,
        stdout_buff,
        0,
        0,
        0,
        0,
        NULL,
        __read_console,
        __write_console,
        __close_console,
        NULL,
        &__files._stderr,
    },
    {
        2,
        0,
        2,
        0,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        {0, 0},
        {0, 0},
        0,
        stderr_buff,
        0x00000100,
        stderr_buff,
        0,
        0,
        0,
        0,
        NULL,
        __read_console,
        __write_console,
        __close_console,
        NULL,
        &__files.empty,
    },
    {
        0,      0, 0,    0,          0,    0, 0, 0, 0, 0,    0,    0,    0,    0,    {0, 0},
        {0, 0}, 0, NULL, 0x00000000, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL,
    },
};

void __close_all(void) {
    FILE* file = &__files._stdin;
    FILE* last_file;

    __begin_critical_region(stdin_access);

    while (file != NULL) {
        if (file->file_mode.file_kind != __closed_file) {
            fclose(file);
        }

        last_file = file;
        file = file->next_file;

        if (last_file->is_dynamically_allocated) {
            free(last_file);
        } else {
            last_file->file_mode.file_kind = __unavailable_file;
            if (file != NULL && file->is_dynamically_allocated) {
                last_file->next_file = NULL;
            }
        }
    }

    __end_critical_region(stdin_access);
}

unsigned int __flush_all(void) {
    unsigned int ret = 0;
    FILE* file = &__files._stdin;

    while (file) {
        if (file->file_mode.file_kind != __closed_file && fflush(file)) {
            ret = -1;
        }
        file = file->next_file;
    }

    return ret;
}
