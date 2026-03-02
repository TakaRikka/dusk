#ifndef NW4HBM_MATH_TYPES_H
#define NW4HBM_MATH_TYPES_H

#include <revolution/types.h>
#include <revolution/mtx.h>

namespace nw4hbm {
    namespace math {
        struct _VEC2 {
            f32 x;  // size 0x04, offset 0x00
            f32 y;  // size 0x04, offset 0x04
        };  // size 0x08

        struct _VEC3 {
            f32 x;  // size 0x04, offset 0x00
            f32 y;  // size 0x04, offset 0x04
            f32 z;  // size 0x04, offset 0x08
        };  // size 0x0c

        struct _VEC4 {
            f32 x;  // size 0x04, offset 0x00
            f32 y;  // size 0x04, offset 0x04
            f32 z;  // size 0x04, offset 0x08
            f32 w;  // size 0x04, offset 0x0c
        };  // size 0x10

        struct _QUAT {
            f32 x;  // size 0x04, offset 0x00
            f32 y;  // size 0x04, offset 0x04
            f32 z;  // size 0x04, offset 0x08
            f32 w;  // size 0x04, offset 0x0c
        };  // size 0x10

        struct _MTX33 {
            union {
                struct {
                    f32 _00;  // size 0x04, offset 0x00
                    f32 _01;  // size 0x04, offset 0x04
                    f32 _02;  // size 0x04, offset 0x08
                    f32 _10;  // size 0x04, offset 0x0c
                    f32 _11;  // size 0x04, offset 0x10
                    f32 _12;  // size 0x04, offset 0x14
                    f32 _20;  // size 0x04, offset 0x18
                    f32 _21;  // size 0x04, offset 0x1c
                    f32 _22;  // size 0x04, offset 0x20
                };  // size 0x24

                f32 m[3][3];   // size 0x24
                f32 a[3 * 3];  // size 0x24
            };  // size 0x24, offset 0x00
        };  // size 0x24

        struct _MTX34 {
            union {
                struct {
                    f32 _00;  // size 0x04, offset 0x00
                    f32 _01;  // size 0x04, offset 0x04
                    f32 _02;  // size 0x04, offset 0x08
                    f32 _03;  // size 0x04, offset 0x0c
                    f32 _10;  // size 0x04, offset 0x10
                    f32 _11;  // size 0x04, offset 0x14
                    f32 _12;  // size 0x04, offset 0x18
                    f32 _13;  // size 0x04, offset 0x1c
                    f32 _20;  // size 0x04, offset 0x20
                    f32 _21;  // size 0x04, offset 0x24
                    f32 _22;  // size 0x04, offset 0x28
                    f32 _23;  // size 0x04, offset 0x2c
                };  // size 0x30

                f32 m[3][4];   // size 0x30
                f32 a[3 * 4];  // size 0x30
                Mtx mtx;       // size 0x30
            };  // size 0x30, offset 0x00
        };  // size 0x30

        struct _MTX44 {
            union {
                struct {
                    f32 _00;  // size 0x04, offset 0x00
                    f32 _01;  // size 0x04, offset 0x04
                    f32 _02;  // size 0x04, offset 0x08
                    f32 _03;  // size 0x04, offset 0x0c
                    f32 _10;  // size 0x04, offset 0x10
                    f32 _11;  // size 0x04, offset 0x14
                    f32 _12;  // size 0x04, offset 0x18
                    f32 _13;  // size 0x04, offset 0x1c
                    f32 _20;  // size 0x04, offset 0x20
                    f32 _21;  // size 0x04, offset 0x24
                    f32 _22;  // size 0x04, offset 0x28
                    f32 _23;  // size 0x04, offset 0x2c
                    f32 _30;  // size 0x04, offset 0x30
                    f32 _31;  // size 0x04, offset 0x34
                    f32 _32;  // size 0x04, offset 0x38
                    f32 _33;  // size 0x04, offset 0x3c
                };  // size 0x40

                f32 m[4][4];   // size 0x40
                f32 a[4 * 4];  // size 0x40
                Mtx44 mtx;     // size 0x40
            };  // size 0x40, offset 0x00
        };  // size 0x40

        struct VEC2 : public _VEC2 {
            // methods
        public:
            // cdtors
            VEC2() {}
            VEC2(f32 fx, f32 fy) {
                x = fx;
                y = fy;
            }

            // operators
            operator f32*() { return reinterpret_cast<f32*>(this); }
            operator f32 const*() const { return reinterpret_cast<f32 const*>(this); }

            // methods
            void Report(bool bNewline, char const* name) const;

            // members
        public:
            /* base _VEC2 */  // size 0x08, offset 0x00
        };  // size 0x08

        struct VEC3 : public _VEC3 {
            // methods
        public:
            // cdtors
            VEC3() {}
            VEC3(f32 fx, f32 fy, f32 fz) {
                x = fx;
                y = fy;
                z = fz;
            }

            // operators
            VEC3 operator+(VEC3 const& rhs) const;
            VEC3 operator-(VEC3 const& rhs) const;

            VEC3 operator*(f32 f) const;

            operator VecPtr() { return reinterpret_cast<VecPtr>(this); }
            operator CVecPtr() const { return (CVecPtr)(this); }

            // methods
            void Report(bool bNewline, char const* name) const;

            // members
        public:
            /* base _VEC3 */  // size 0x0c, offset 0x00
        };  // size 0x0c

        struct VEC4 : public _VEC4 {
            // methods
        public:
            // cdtors
            VEC4() {}

            // methods
            void Report(bool bNewline, char const* name) const;

            // members
        public:
            /* base _VEC4 */  // size 0x10, offset 0x00
        };  // size 0x10

        struct QUAT : public _QUAT {
            // methods
        public:
            // methods
            void Report(bool bNewline, char const* name) const;

            // members
        public:
            /* base _QUAT */  // size 0x10, offset 0x00
        };  // size 0x10

        struct MTX33 : public _MTX33 {
            // methods
        public:
            // methods
            void Report(bool bNewline, char const* name) const;

            // members
        public:
            /* base _MTX33 */  // size 0x24, offset 0x00
        };  // size 0x24

        struct MTX34 : public _MTX34 {
            // methods
        public:
            // cdtors
            MTX34() {}

            // operators
            operator MtxPtr() { return mtx; }
            operator CMtxP() const { return mtx; }

            // methods
            void Report(bool bNewline, char const* name) const;

            // members
        public:
            /* base _MTX34 */  // size 0x30, offset 0x00
        };  // size 0x30

        struct MTX44 : public _MTX44 {
            // methods
        public:
            // methods
            void Report(bool bNewline, char const* name) const;

            // members
        public:
            /* base _MTX44 */  // size 0x24, offset 0x00
        };  // size 0x24

        inline VEC3* VEC3Add(register VEC3* pOut, register VEC3 const* p1,
                             register VEC3 const* p2) {
            register f32 a, b, c;

#if defined(__MWERKS__)
            asm
            {
#define qr0 0

                // xy
			psq_l	a, 0(p1), FALSE, qr0
			psq_l	b, 0(p2), FALSE, qr0

			ps_add	c, a, b
			psq_st	c, 0(pOut), FALSE, qr0

                    // z-
			psq_l	a, 8(p1), TRUE, qr0
			psq_l	b, 8(p2), TRUE, qr0

			ps_add	c, a, b
			psq_st	c, 8(pOut), TRUE, qr0

#undef qr0
            }
#else
#pragma unused(p1, p2, a, b, c)
#endif

            return pOut;
        }

        inline VEC3* VEC3Sub(register VEC3* pOut, register VEC3 const* p1,
                             register VEC3 const* p2) {
            register f32 a, b, c;

#if defined(__MWERKS__)
            asm
            {
#define qr0 0

                // xy
			psq_l	a, 0(p1), FALSE, qr0
			psq_l	b, 0(p2), FALSE, qr0

			ps_sub	c, a, b
			psq_st	c, 0(pOut), FALSE, qr0

                    // z-
			psq_l	a, 8(p1), TRUE, qr0
			psq_l	b, 8(p2), TRUE, qr0

			ps_sub	c, a, b
			psq_st	c, 8(pOut), TRUE, qr0

#undef qr0
            }
#else
#pragma unused(p1, p2, a, b, c)
#endif

            return pOut;
        }

        inline VEC3* VEC3Scale(register VEC3* pOut, register VEC3 const* p, register f32 scale) {
            register f32 a, b;

#if defined(__MWERKS__)
            asm
            {
#define qr0 0

                // xy
			psq_l		a, 0(p), FALSE, qr0
			ps_muls0	b, a, scale
			psq_st		b, 0(pOut), FALSE, qr0

                    // z-
			psq_l		a, 8(p), TRUE, qr0
			ps_muls0	b, a, scale
			psq_st		b, 8(pOut), TRUE, qr0

#undef qr0
            }
#else
#pragma unused(p, scale, a, b)
#endif

            return pOut;
        }

        inline VEC3* VEC3Lerp(register VEC3* pOut, register VEC3 const* p1, register VEC3 const* p2,
                              register f32 t) {
            register f32 a, b, c;

#if defined(__MWERKS__)
            asm
            {
#define qr0 0

                // xy
			psq_l		a, 0(p1), FALSE, qr0
			psq_l		b, 0(p2), FALSE, qr0

			ps_sub		c, b, a
			ps_madds0	c, c, t, a
			psq_st		c, 0(pOut), FALSE, qr0

                    // z-
			psq_l		a, 8(p1), TRUE, qr0
			psq_l		b, 8(p2), TRUE, qr0

			ps_sub		c, b, a
			ps_madds0	c, c, t, a
			psq_st		c, 8(pOut), TRUE, qr0

#undef qr0
            }
#else
#pragma unused(p1, p2, t, a, b, c)
#endif

            return pOut;
        }

        inline f32 VEC3Dot(register VEC3 const* p1, register VEC3 const* p2) {
            register f32 _v1, _v2, _v3, _v4, _v5;

#if defined(__MWERKS__)
            asm
                {
#define qr0 0

                    // yz
			psq_l	_v2, 4(p1), FALSE, qr0
			psq_l	_v3, 4(p2), FALSE, qr0

			ps_mul	_v2, _v2, _v3

                        // x-
			psq_l	_v5, 0(p1), TRUE, qr0
			psq_l	_v4, 0(p2), TRUE, qr0

			ps_madd	_v3, _v5, _v4, _v2
			ps_sum0	_v1, _v3, _v2, _v2

#undef qr0
                }
#else
#pragma unused(p1, p2, _v2, _v3, _v4, _v5)
            _v1 = 0;
#endif

            return _v1;
        }

        inline f32 VEC3LenSq(register VEC3 const* p) {
            register f32 vxy, vzz, sqmag;

#if defined(__MWERKS__)
            asm
                {
#define qr0 0

			psq_l	vxy, 0(p), FALSE, qr0
			ps_mul	vxy, vxy, vxy

			lfs		vzz, 8(p)
			ps_madd	sqmag, vzz, vzz, vxy
			ps_sum0	sqmag, sqmag, vxy, vxy

#undef qr0
                }
#else
#pragma unused(p, vxy, vzz)
            sqmag = 0;
#endif

            return sqmag;
        }

        inline VEC3* VEC3Cross(VEC3* pOut, VEC3 const* p1, VEC3 const* p2) {
            PSVECCrossProduct(*p1, *p2, *pOut);

            return pOut;
        }

        inline VEC3* VEC3Normalize(VEC3* pOut, VEC3 const* p) {
            PSVECNormalize(*p, *pOut);

            return pOut;
        }

        inline f32 VEC3DistSq(VEC3 const* p1, VEC3 const* p2) {
            return PSVECSquareDistance(*p1, *p2);
        }

        // Line info puts these operators outside of the class body
        inline VEC3 VEC3::operator+(VEC3 const& rhs) const {
            VEC3 tmp;
            VEC3Add(&tmp, this, &rhs);

            return tmp;
        }

        inline VEC3 VEC3::operator-(VEC3 const& rhs) const {
            VEC3 tmp;
            VEC3Sub(&tmp, this, &rhs);

            return tmp;
        }

        inline VEC3 VEC3::operator*(f32 f) const {
            VEC3 tmp;
            VEC3Scale(&tmp, this, f);

            return tmp;
        }

        inline MTX34* MTX34Mult(MTX34* pOut, MTX34 const* p1, MTX34 const* p2) {
            PSMTXConcat(*p1, *p2, *pOut);

            return pOut;
        }

        inline MTX34* MTX34Copy(MTX34* pOut, const MTX34* p) {
            PSMTXCopy(*p, *pOut);

            return pOut;
        }

        inline MTX34* MTX34Identity(MTX34* pOut) {
            PSMTXIdentity(*pOut);

            return pOut;
        }

        inline u32 MTX34Inv(MTX34* pOut, MTX34 const* p) {
            return PSMTXInverse(*p, *pOut);
        }

        inline VEC3* VEC3TransformCoord(VEC3* pOut, MTX34 const* pM, VEC3 const* pV) {
            PSMTXMultVec(*pM, *pV, *pOut);

            return pOut;
        }

        VEC2* VEC2Maximize(VEC2* pOut, VEC2 const* p1, VEC2 const* p2);
        VEC2* VEC2Minimize(VEC2* pOut, VEC2 const* p1, VEC2 const* p2);
        VEC2* VEC2Normalize(VEC2* pOut, VEC2 const* p);

        VEC3* VEC3Maximize(VEC3* pOut, VEC3 const* p1, VEC3 const* p2);
        VEC3* VEC3Minimize(VEC3* pOut, VEC3 const* p1, VEC3 const* p2);

        VEC4* VEC4Add(VEC4* pOut, VEC4 const* p1, VEC4 const* p2);
        VEC4* VEC4Sub(VEC4* pOut, VEC4 const* p1, VEC4 const* p2);
        VEC4* VEC4Scale(VEC4* pOut, VEC4 const* p, f32 scale);
        VEC4* VEC4Lerp(VEC4* pOut, VEC4 const* p1, VEC4 const* p2, f32 t);
        f32 VEC4Dot(VEC4 const* p1, VEC4 const* p2);
        f32 VEC4LenSq(VEC4 const* p);
        f32 VEC4Len(VEC4 const* p);
        VEC4* VEC4Normalize(VEC4* pOut, VEC4 const* p);
        f32 VEC4DistSq(VEC4 const* p1, VEC4 const* p2);
        VEC4* VEC4Maximize(VEC4* pOut, VEC4 const* p1, VEC4 const* p2);
        VEC4* VEC4Minimize(VEC4* pOut, VEC4 const* p1, VEC4 const* p2);

        MTX33* MTX33Copy(MTX33* pOut, MTX33 const* p);
        MTX33* MTX33Zero(MTX33* pOut);
        MTX33* MTX33Identity(MTX33* pOut);

        MTX33* MTX34ToMTX33(MTX33* pOut, MTX34 const* pM);

        MTX33* MTX33MAdd(MTX33* pOut, f32 t, MTX33 const* p1, MTX33 const* p2);

        u32 MTX34InvTranspose(MTX33* inv, MTX34 const* src);
        MTX34* MTX34Zero(MTX34* pOut);
        bool MTX34IsIdentity(MTX34 const* p);
        MTX34* MTX34Add(MTX34* pOut, MTX34 const* p1, MTX34 const* p2);
        MTX34* MTX34Sub(MTX34* pOut, MTX34 const* p1, MTX34 const* p2);
        MTX34* MTX34Mult(MTX34* pOut, MTX34 const* p, f32 f);
        MTX34* MTX34Scale(MTX34* pOut, MTX34 const* pM, VEC3 const* pS);
        MTX34* MTX34Trans(MTX34* pOut, MTX34 const* pM, VEC3 const* pT);
        MTX34* MTX34MAdd(MTX34* pOut, f32 t, MTX34 const* p1, MTX34 const* p2);
        MTX34* MTX34RotAxisFIdx(MTX34* pOut, VEC3 const* pAxis, f32 fIdx);
        MTX34* MTX34RotXYZFIdx(MTX34* pOut, f32 fIdxX, f32 fIdxY, f32 fIdxZ);
        MTX34* MTX34RotXYZTransFIdx(MTX34* pOut, f32 fIdxX, f32 fIdxY, f32 fIdxZ, VEC3 const* pT);

        VEC3* VEC3TransformNormal(VEC3* pOut, MTX34 const* pM, VEC3 const* pV);
        VEC3* VEC3TransformNormalArray(VEC3* pOut, MTX34 const* pM, VEC3 const* pV, u32 count);

        MTX44* MTX44Zero(MTX44* pOut);
        MTX44* MTX44Identity(MTX44* pOut);
        MTX44* MTX44Copy(MTX44* pOut, MTX44 const* p);
        bool MTX44IsIdentity(MTX44 const* p);

        VEC4* VEC3Transform(VEC4* pOut, MTX44 const* pM, VEC3 const* pV);
        VEC3* VEC3TransformCoord(VEC3* pOut, MTX44 const* pM, VEC3 const* pV);
        VEC4* VEC3TransformArray(VEC4* pOut, MTX44 const* pM, VEC3 const* pV, u32 count);
        VEC3* VEC3TransformCoordArray(VEC3* pOut, MTX44 const* pM, VEC3 const* pV, u32 count);

        VEC4* VEC4Transform(VEC4* pOut, MTX44 const* pM, VEC4 const* pV);
        VEC4* VEC4TransformArray(VEC4* pOut, MTX44 const* pM, VEC4 const* pV, u32 count);
    }  // namespace math
}  // namespace nw4hbm

#endif  // NW4HBM_MATH_TYPES_H
