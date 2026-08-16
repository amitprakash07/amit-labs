#ifndef CORE_MATHS_GEOMETRY_RAY_INTERSECTION_H_
#define CORE_MATHS_GEOMETRY_RAY_INTERSECTION_H_

#include "core/maths/linear_algebra/vector3.h"

#include "sphere.h"
#include "plane.h"
#include "bounding_box.h"
#include "triangle.h"

namespace amit::geometry
{
    struct HitInfo
    {
        amit::maths::Vector3 hit_point;
        amit::maths::Vector3 normal;
        float                  distance;
        HitInfo();
    };

    class Ray
    {
    public:
        template <class Primitive>
        bool Intersect(const Primitive& primitive, HitInfo& hit_info) const
        {
            return false;
        }

        amit::maths::Vector3 origin;
        amit::maths::Vector3 direction;
    };

}  // namespace amit::geometry

template <>
bool amit::geometry::Ray::Intersect<amit::geometry::Plane>(const amit::geometry::Plane& primitive,
                                                           amit::geometry::HitInfo&     hit_info) const;

template <>
bool amit::geometry::Ray::Intersect<amit::geometry::Sphere>(const amit::geometry::Sphere& primitive,
                                                            amit::geometry::HitInfo&      hit_info) const;

template <>
bool amit::geometry::Ray::Intersect<amit::geometry::BoundingBox>(const amit::geometry::BoundingBox& primitive,
                                                                 amit::geometry::HitInfo&           hit_info) const;

template <>
bool amit::geometry::Ray::Intersect<amit::geometry::Triangle>(const amit::geometry::Triangle& primitive,
                                                              amit::geometry::HitInfo&        hit_info) const;

#endif
