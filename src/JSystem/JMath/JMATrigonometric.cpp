#include "JSystem/JSystem.h" // IWYU pragma: keep

// don't include header until this "zero" mess is figured out
// #include "JSystem/JMath/JMATrigonometric.h"
#include "math.h"
#include "global.h"

static f32 dummy() {
    return 0.0f;
}

namespace std {
template <typename A1, typename B1>
struct pair {
    A1 a1;
    B1 b1;
    pair() {
        f32 tmp = 0.0f;
        a1 = tmp;
        b1 = tmp;
        // a1 = A1();
        // b1 = B1();
    }
};
}  // namespace std

inline f64 getConst() {
    return 6.2831854820251465;
}

template<int N, typename T>
struct TSinCosTable {
    std::pair<T, T> table[1 << N];
    TSinCosTable() {
        for (int i = 0; i < 1 << N; i++) {
            table[i].a1 = sin((i * getConst()) / (1 << N));
            table[i].b1 = cos((i * getConst()) / (1 << N));
        }
    }
};

inline f64 getConst2() {
    return 9.765625E-4;
}

struct TAtanTable {
    f32 table[1025];
    u8 pad[0x1C];
    TAtanTable() {
        // (u32) cast needed for cmplwi instead of cmpwi
        for (int i = 0; i < (u32)1024; i++) {
            table[i] = atan(getConst2() * i);
        }
        table[0] = 0.0f;
        table[1024] = 0.7853982;  // 0.25 * PI
    }
};

struct TAsinAcosTable {
    f32 table[1025];
    u8 pad[0x1C];
    TAsinAcosTable() {
        for (int i = 0; i < 1024; i++) {
            table[i] = asin(getConst2() * i);
        }
        table[0] = 0.0f;
        table[1024] = 0.7853982;  // 0.25 * PI
    }
};

namespace JMath {

TSinCosTable<13, f32> sincosTable_;

TAtanTable atanTable_;

TAsinAcosTable asinAcosTable_;

}  // namespace JMath
