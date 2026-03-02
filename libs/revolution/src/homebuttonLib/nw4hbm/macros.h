#ifndef NW4HBM_MACROS_H
#define NW4HBM_MACROS_H

// Doesn't exist in line info, but still helpful to have

#if defined(CHAR_BIT)
#define CHAR_BIT_ CHAR_BIT
#else
#define CHAR_BIT_ 8  // most common; default
#endif

// offset_ is in chars
#define NW4HBM_BYTE_(byte_, offset_) (static_cast<unsigned char>(byte_) << CHAR_BIT_ * (offset_))

// File versions and FourChars

#define NW4HBM_FILE_VERSION(major_, minor_) (NW4HBM_BYTE_(major_, 1) | NW4HBM_BYTE_(minor_, 0))

#define NW4HBM_FOUR_CHAR(a_, b_, c_, d_)                                                           \
    (NW4HBM_BYTE_(a_, 3) | NW4HBM_BYTE_(b_, 2) | NW4HBM_BYTE_(c_, 1) | NW4HBM_BYTE_(d_, 0))

#endif  // NW4HBM_MACROS_H
