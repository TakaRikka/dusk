#ifndef DOLZEL_REL_H
#define DOLZEL_REL_H

// Shield versions move some RELs into the DOL, so need a way to switch which
// precompiled headers are being used depending on some variable
#if __FORCE_REL_IN_DOL__
    #if __MWERKS__ && !defined(DECOMPCTX)
    #include "d/dolzel.mch"
    #else
    #include "d/dolzel.pch"
    #endif
#else
    #if __MWERKS__ && !defined(DECOMPCTX)
    #include "d/dolzel_rel.mch"
    #else
    #include "d/dolzel_rel.pch"
    #endif
#endif

#endif // DOLZEL_REL_H
