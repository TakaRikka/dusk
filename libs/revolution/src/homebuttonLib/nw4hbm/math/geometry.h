#ifndef NW4HBM_MATH_GEOMETRY_H
#define NW4HBM_MATH_GEOMETRY_H

#include <revolution/types.h>

#include "types.h"

namespace nw4hbm {
    namespace math {
        enum IntersectionResult {
            INTERSECTION_NONE = 0,
            INTERSECTION_1 = 1,
            INTERSECTION_2 = 2,

            INTERSECTION_LINE3_ON_PLANE = INTERSECTION_2,
            INTERSECTION_RAY3_ON_PLANE = INTERSECTION_2,
            INTERSECTION_SEGMENT3_ON_PLANE = INTERSECTION_2,

            INTERSECTION_OUTSIDE = 0,
            INTERSECTION_INSIDE = 1,
            INTERSECTION_INTERSECT = 2
        };

        struct SEGMENT3 {
            VEC3 P0;  // size 0x0c, offset 0x00
            VEC3 P1;  // size 0x0c, offset 0x0c
        };  // size 0x18

        struct RAY3 {
            VEC3 P;  // size 0x0c, offset 0x00
            VEC3 d;  // size 0x0c, offset 0x0c
        };  // size 0x18

        struct LINE3 {
            // methods
        public:
            // cdtors
            LINE3() {}
            LINE3(VEC3 const& Pt, VEC3 const& dir, bool isNormalized) : P(Pt), d(dir) {
                if (!isNormalized)
                    Normalize();
            }

            // methods
            void Normalize() { VEC3Normalize(&d, &d); }

            void Set(SEGMENT3 const* S) {
                P = S->P0;
                VEC3Sub(&d, &S->P1, &S->P0);

                Normalize();
            }

            // members
        public:
            VEC3 P;  // size 0x0c, offset 0x00
            VEC3 d;  // size 0x0c, offset 0x0c
        };  // size 0x18

        struct CAPSULE {
            SEGMENT3 S;  // size 0x18, offset 0x00
            f32 r;       // size 0x04, offset 0x18
        };  // size 0x1c

        struct PLANE {
            // methods
        public:
            // methods
            void Set(VEC3 const* P0, VEC3 const* P1, VEC3 const* P2);

            f32 Test(VEC3 const& P) const { return VEC3Dot(&N, &P) + d; }

            // members
        public:
            VEC3 N;  // size 0x0c, offset 0x00
            f32 d;   // size 0x04, offset 0x0c
        };  // size 0x10

        struct AABB {
            // methods
        public:
            // cdtors
            AABB() {}

            // methods
            void Normalize();
            void Set(VEC3 const* arrayPoint, unsigned numPoints);
            void Set(AABB const* box, MTX34 const* M);

            // members
        public:
            VEC3 Pmin;  // size 0x0c, offset 0x00
            VEC3 Pmax;  // size 0x0c, offset 0x0c
        };  // size 0x18

        struct SPHERE {
            // methods
        public:
            // methods
            void Set(VEC3 const* arrayPoint, unsigned numPoints);

            // members
        public:
            VEC3 C;  // size 0x0c, offset 0x00
            f32 r;   // size 0x04, offset 0x0c
        };  // size 0x10

        class FRUSTUM {
            // methods
        public:
            // methods
            void Set(f32 fovy, f32 aspect, f32 n, f32 f, MTX34 const& camera);
            void Set(f32 top, f32 bottom, f32 left, f32 right, f32 n, f32 f, MTX34 const& camera);

            bool IntersectSphere(SPHERE const* S) const;
            bool IntersectAABB(AABB const* B) const;
            IntersectionResult IntersectAABB_Ex(AABB const* B) const;

            // members
        public:
            MTX34 cam;          // size 0x30, offset 0x00
            PLANE leftPlane;    // size 0x10, offset 0x30
            PLANE rightPlane;   // size 0x10, offset 0x40
            PLANE topPlane;     // size 0x10, offset 0x50
            PLANE bottomPlane;  // size 0x10, offset 0x60
            f32 near;           // size 0x04, offset 0x70
            f32 far;            // size 0x04, offset 0x74
            AABB box;           // size 0x18, offset 0x78
            PLANE planes[6];    // size 0x60, offset 0x90
        };  // size 0xf0

        f32 DistSqPoint3ToLine3(VEC3 const* P, LINE3 const* L, f32* t);
        f32 DistSqPoint3ToRay3(VEC3 const* P, RAY3 const* R, f32* t);
        f32 DistSqPoint3ToSegment3(VEC3 const* P, SEGMENT3 const* S, f32* t);
        f32 DistSqPoint3ToPlane(VEC3 const* P, PLANE const* J, VEC3* Q);
        f32 DistSqSphereToPlane(SPHERE const* S, PLANE const* J);
        f32 DistSqPoint3ToPolyline3(VEC3 const* P, VEC3 const* vertices, unsigned nVertices);
        f32 DistSqLine3ToLine3(LINE3 const* L0, LINE3 const* L1, f32* s, f32* t);
        f32 DistSqSegment3ToSegment3(SEGMENT3 const* S1, SEGMENT3 const* S2, f32* s, f32* t);
        f32 DistSqLine3ToRay3(LINE3 const* L, RAY3 const* R, f32* s, f32* t);
        f32 DistSqLine3ToSegment3(LINE3 const* L0, SEGMENT3 const* S, f32* s, f32* t);
        f32 DistSqRay3ToRay3(RAY3 const* R0, RAY3 const* R1, f32* s, f32* t);
        f32 DistSqRay3ToSegment3(RAY3 const* R0, SEGMENT3 const* S, f32* s, f32* t);

        IntersectionResult IntersectionLine3Plane(LINE3 const* L, PLANE const* J, f32* t, VEC3* I);
        IntersectionResult IntersectionRay3Plane(RAY3 const* R, PLANE const* J, f32* t, VEC3* I);
        IntersectionResult IntersectionSegment3Plane(SEGMENT3 const* S, PLANE const* J, f32* t,
                                                     VEC3* I);
        IntersectionResult IntersectionLine3Sphere(LINE3 const* L, SPHERE const* sphere, f32* t0,
                                                   f32* t1);
        IntersectionResult IntersectionRay3Sphere(RAY3 const* R, SPHERE const* sphere, f32* t0,
                                                  f32* t1);
        bool IntersectionRay3Sphere(RAY3 const* R, SPHERE const* sphere);
        IntersectionResult IntersectionSegment3Sphere(SEGMENT3 const* S, SPHERE const* sphere,
                                                      f32* t0, f32* t1);

        bool IntersectionRay3AABB(RAY3 const* R, AABB const* box, f32* t);
        bool IntersectionAABB(AABB const* a, AABB const* b);
        bool IntersectionSphereAABB(SPHERE const* sphere, AABB const* aabb);
        bool IntersectionSphere(SPHERE const* s0, SPHERE const* s1);
        bool IntersectionCapsule(CAPSULE const* C0, CAPSULE const* C1);
        bool IntersectionRay3Capsule(RAY3 const* R, CAPSULE const* C);
        bool IntersectionLine3Capsule(LINE3 const* L, CAPSULE const* C);
        bool IntersectionPlaneCapsule(PLANE const* J, CAPSULE const* C);

        SPHERE* MergeSphere(SPHERE* s2, SPHERE const* s0, SPHERE const* s1);
        AABB* MergeAABB(AABB* a2, AABB const* a0, AABB const* a1);
    }  // namespace math
}  // namespace nw4hbm

#endif  // NW4HBM_MATH_GEOMETRY_H
