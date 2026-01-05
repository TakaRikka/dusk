#ifndef NW4R_DB_ASSERT_H
#define NW4R_DB_ASSERT_H

#include <revolution/types.h>

#include <stdio>

namespace nw4hbm {
    namespace db {
        // Forward declarations
        namespace detail {
            class ConsoleHead;
        }

        /* DECL_WEAK */ void VPanic(const char* file, int line, const char* fmt, std::va_list vlist);
        /* DECL_WEAK */ void VWarning(const char* file, int line, const char* fmt, std::va_list vlist);

        namespace detail {
            void Log(const char* fmt, ...);
            /* DECL_WEAK */ void Panic(const char* file, int line, const char* fmt, ...);
            /* DECL_WEAK */ void Warning(const char* file, int line, const char* msg, ...);
        }

        detail::ConsoleHead* Assertion_SetConsole(detail::ConsoleHead* console);
        detail::ConsoleHead* Assertion_GetConsole();
        void Assertion_ShowConsole(u32 time);
        void Assertion_HideConsole();
        void Assertion_SetWarningTime(u32 time);
        bool Assertion_SetAutoWarning(bool enable);
    }  // namespace db
}  // namespace nw4r

#if NW4HBM_DEBUG
#define NW4R_DB_WARNING(line, exp, ...) \
    (void)((exp) || (nw4hbm::db::detail::Warning(__FILE__, line, __VA_ARGS__), 0))
#define NW4R_DB_ASSERTMSG(line, exp, ...) \
    (void)((exp) || (nw4hbm::db::detail::Panic(__FILE__, line, __VA_ARGS__), 0))
#else
#define NW4R_DB_WARNING(line, exp, ...) (void)0
#define NW4R_DB_ASSERTMSG(line, exp, ...) (void)0
#endif

#define NW4R_ASSERT(line, exp) \
    NW4R_DB_ASSERTMSG(line, (exp), "Failed assertion " #exp)

#define NW4R_ASSERT_CHECK_NULL(line, ptr) \
    NW4R_DB_ASSERTMSG(line, (ptr != NULL), "Pointer must not be NULL ("#ptr")")

#define POINTER_VALID_TEST(ptr_)                                                                             \
    (((unsigned long)ptr_ & 0xFF000000) == 0x80000000 || ((unsigned long)ptr_ & 0xFF800000) == 0x81000000 || \
     ((unsigned long)ptr_ & 0xF8000000) == 0x90000000 || ((unsigned long)ptr_ & 0xFF000000) == 0xC0000000 || \
     ((unsigned long)ptr_ & 0xFF800000) == 0xC1000000 || ((unsigned long)ptr_ & 0xF8000000) == 0xD0000000 || \
     ((unsigned long)ptr_ & 0xFFFFC000) == 0xE0000000)

#define NW4R_ASSERT_VALID_PTR(line, ptr) \
    NW4R_DB_ASSERTMSG(line, POINTER_VALID_TEST(ptr), "Pointer Error\n" #ptr "(=%p) is not valid pointer.", ptr)

#define NW4R_ASSERT_MIN(line_, var_, minValue_)                                                   \
    NW4R_DB_ASSERTMSG(line_, minValue_ <= var_,                                                   \
                      #var_ " is out of bounds(%d)\n%d <= " #var_ " not satisfied.", (int)(var_), \
                      (int)(minValue_))

#define NW4R_ASSERT_MINMAX(line_, var_, minValue_, maxValue_)                                          \
    NW4R_DB_ASSERTMSG(line_, (var_) >= (minValue_) && (var_) < (maxValue_),                            \
                      #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied.", (int)(var_), \
                      (int)(minValue_), (int)(maxValue_))

#define NW4R_ASSERT_MINMAXLT(line_, var_, minValue_, maxValue_)                                         \
    NW4R_DB_ASSERTMSG(line_, (var_) >= (minValue_) && (var_) <= (maxValue_),                            \
                      #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied.", (int)(var_), \
                      (int)(minValue_), (int)(maxValue_))

#define NW4R_IS_ALIGNED_(x, align) \
    (((unsigned long)(x) & ((align) - 1)) == 0)

#define NW4R_ASSERT_ALIGN2(line, exp)                                                         \
    NW4R_DB_ASSERTMSG(line, NW4R_IS_ALIGNED_(exp, 2),                                         \
                      "Alignment Error(0x%x)\n" #exp " must be aligned to 2 bytes boundary.", \
                      exp)

#define NW4R_ASSERT_ALIGN32(line, exp)                                                         \
    NW4R_DB_ASSERTMSG(line, NW4R_IS_ALIGNED_(exp, 32),                                         \
                      "Alignment Error(0x%x)\n" #exp " must be aligned to 32 bytes boundary.", \
                      exp)

// NW4HBM variants
#define NW4HBM_ASSERT_CHECK_NULL(line, ptr) \
    NW4R_DB_ASSERTMSG(line, (ptr != NULL), "NW4HBM:Pointer must not be NULL ("#ptr")")

#define NW4HBM_ASSERT(line, exp) \
    NW4R_DB_ASSERTMSG(line, (exp), "NW4HBM:Failed assertion " #exp)

#define NW4HBM_ASSERT_VALID_PTR(line, ptr) \
    NW4R_DB_ASSERTMSG(line, POINTER_VALID_TEST(ptr), "NW4HBM:Pointer Error\n" #ptr "(=%p) is not valid pointer.", ptr)

#define NW4HBM_ASSERT_ALIGN2(line, exp)                                                              \
    NW4R_DB_ASSERTMSG(line, NW4R_IS_ALIGNED_(exp, 2),                                                \
                      "NW4HBM:Alignment Error(0x%x)\n" #exp " must be aligned to 2 bytes boundary.", \
                      exp)

#define NW4HBM_ASSERT_ALIGN32(line, exp)                                                              \
    NW4R_DB_ASSERTMSG(line, NW4R_IS_ALIGNED_(exp, 32),                                                \
                      "NW4HBM:Alignment Error(0x%x)\n" #exp " must be aligned to 32 bytes boundary.", \
                      exp)

#define NW4HBM_ASSERT_ALIGNED(line, exp, align)                                                       \
    NW4R_DB_ASSERTMSG(line, NW4R_IS_ALIGNED_(exp, align),                                                \
                      "NW4HBM:Alignment Error(0x%x)\n" #exp " must be aligned to " #align " bytes boundary.", \
                      exp)

#endif
