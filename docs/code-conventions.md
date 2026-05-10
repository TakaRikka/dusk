# Code conventions for Dusk

## Upstream when appropriate

Bug fixes, documentation improvements, code cleanup, etc that also apply to the [original decompilation project](https://github.com/zeldaret/tp) should preferably be PR'd there.

## Properly indicate Dusk-modified code

When modifying original game code (i.e. in decomp) for Dusk's purposes, please clearly delineate such code as being Dusk-specific. Generally, this can be done by using `#if TARGET_PC` and keeping the original code in place. Use `#if AVOID_UB` for Undefined Behavior fixes to the original codebase.

## Miscellaneous things

* The original codebase makes heavy use of global `operator new` and similar overloads to allocate into a strict tree of heaps. This would cause many linkage headaches for us, so effectively all uses of `new` or `delete` in the original game code have been replaced with `JKR_NEW`, `JKR_DELETE`, or similar macros. See `JKRHeap.h` for the full list.  
