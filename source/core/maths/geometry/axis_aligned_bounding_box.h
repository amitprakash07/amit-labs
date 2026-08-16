#ifndef CORE_MATHS_GEOMETRY_AXIS_ALIGNED_BOUNDING_BOX_H_
#define CORE_MATHS_GEOMETRY_AXIS_ALIGNED_BOUNDING_BOX_H_

#include "core/maths/linear_algebra/vector3.h"

namespace amit::geometry
{
    namespace Physics
    {
        struct Plane
        {
            Plane() = default;
            maths::Vector3 _normal;  // Plane normal. Points x on the plane satisfy Dot(n,x) = d
            float         _d;       // d = Dot(n,p) for a given point p on the plane
        };

        struct Sphere
        {
            Sphere() = default;
            maths::Vector3 _center;
            float         _radius;
        };

        struct Capsule
        {
            Capsule() = default;
            maths::Vector3 _a;
            maths::Vector3 _b;
            float         _radius;
        };

        struct AABBV2
        {
            AABBV2() = default;
            maths::Vector3 _pos;
            maths::Vector3 _extents;
        };

        struct OBB
        {
            OBB() = default;
            maths::Vector3 _pos;
            maths::Vector3 _extent;
            maths::Vector3 _axis[3];  // Local x-, y-, and z-axes
        };

        struct Ray
        {
            Ray() = default;
            maths::Vector3 _pos;
            maths::Vector3 _dir;
        };

        // Given three noncollinear points (ordered ccw), compute plane equation
        inline Plane ComputePlane(maths::Vector3 a, maths::Vector3 b, maths::Vector3 c)
        {
            Plane p;
            p._normal = (b - a).Cross(c - a).CreateNormalized();
            p._d      = p._normal.dot(a);
            return p;
        }

        struct AABBV1
        {
            AABBV1() = default;
            maths::Vector3 _min;
            maths::Vector3 _max;
        };

        // Test if AABB b intersects plane p
        inline bool TestAABBPlane(AABBV1 b, Plane p)
        {
            // These two lines not necessary with a (center, extents) AABB representation
            maths::Vector3 c = (b._max + b._min) * 0.5f;  // Compute AABB center
            maths::Vector3 e = b._max - c;                // Compute positive extents

            // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
            float r =
                e[0] * std::fabsf(p._normal[0]) + e[1] * std::fabsf(p._normal[1]) + e[2] * std::fabsf(p._normal[2]);
            // Compute distance of box center from plane
            float s = p._normal.dot(c) - p._d;  // Intersection occurs when distance s
                                                // falls within [-r,+r] interval
            return std::fabsf(s) <= r;
        }

        inline bool TestTriangleAABB(maths::Vector3 v0, maths::Vector3 v1, maths::Vector3 v2, AABBV1 b)
        {
            // Compute box center and extents (if not already given in that format)
            maths::Vector3 c  = (b._min + b._max) * 0.5f;
            float         e0 = (b._max.x - b._min.x) * 0.5f;
            float         e1 = (b._max.y - b._min.y) * 0.5f;
            float         e2 = (b._max.z - b._min.z) * 0.5f;
            // Translate triangle as conceptually moving AABB to origin
            v0 = v0 - c;
            v1 = v1 - c;
            v2 = v2 - c;
            // Compute edge vectors for triangle
            maths::Vector3 axis[3];
            axis[0] = maths::Vector3(1, 0, 0);
            axis[1] = maths::Vector3(0, 1, 0);
            axis[2] = maths::Vector3(0, 0, -1);
            maths::Vector3 edge[3];
            edge[0] = v1 - v0, edge[1] = v2 - v1, edge[2] = v0 - v2;
            // Test axes a00..a22 (category 3)
            float p0, p1, p2, r;
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    maths::Vector3 projectionSATAxis = axis[i].Cross(edge[j]);
                    r                               = std::fabsf(axis[0].dot(projectionSATAxis)) * e0 +
                        std::fabsf(axis[1].dot(projectionSATAxis)) * e1 +
                        std::fabsf(axis[2].dot(projectionSATAxis)) * e2;
                    p0        = v0.dot(projectionSATAxis);
                    p1        = v1.dot(projectionSATAxis);
                    p2        = v2.dot(projectionSATAxis);
                    float max = std::fmaxf(p0, std::fmaxf(p1, p2));
                    float min = std::fminf(p0, std::fminf(p1, p2));
                    // If the projection intervals [?r, r] and min(p0, p1, p2), max(p0, p1,
                    // p2) are disjoint for the given axis, the axis is a separating axis and
                    // the triangle and the AABB do not overlap.
                    if (max < -r || min > r)
                        return false;  // Axis is a separating axis
                }
            }
            // Test the three axes corresponding to the face normals of AABB b (category
            // 1). Exit if...
            // ... [-e0, e0] and [min(v0.x,v1.x,v2.x), max(v0.x,v1.x,v2.x)] do not overlap
            if (std::fmaxf(v0.x, std::fmaxf(v1.x, v2.x)) < -e0 || std::fminf(v0.x, std::fminf(v1.x, v2.x)) > e0)
                return 0;
            // ... [-e1, e1] and [min(v0.y,v1.y,v2.y), max(v0.y,v1.y,v2.y)] do not overlap
            if (std::fmaxf(v0.y, std::fmaxf(v1.y, v2.y)) < -e1 || std::fminf(v0.y, std::fminf(v1.y, v2.y)) > e1)
                return 0;
            // ... [-e2, e2] and [min(v0.z,v1.z,v2.z), max(v0.z,v1.z,v2.z)] do not overlap
            if (std::fmaxf(v0.z, std::fmaxf(v1.z, v2.z)) < -e2 || std::fminf(v0.z, std::fminf(v1.z, v2.z)) > e2)
                return 0;
            // Test separating axis corresponding to triangle face normal (category 2)
            Plane p;
            p._normal = edge[0].Cross(edge[1]);
            p._d      = p._normal.dot(v0);
            AABBV1 b2;
            b2._min = b._min - c;
            b2._max = b._max - c;
            return TestAABBPlane(b2, p);
        }
    }  // namespace Physics
}  // namespace amit::geometry

#endif
