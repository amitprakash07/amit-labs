#ifndef CORE_MATHS_GEOMETRY_SPHERE_H_
#define CORE_MATHS_GEOMETRY_SPHERE_H_

#include "core/maths/linear_algebra/vector3.h"
#include "primitive.h"

namespace amit::geometry
{
    class Sphere : public Primitive
    {
    public:
        Sphere(const amit::maths::Vector3& centre, float radius)
            : Primitive(PrimitiveType::kSphere)
            , centre(centre)
            , radius(radius)
        {
        }

        Sphere()
            : Primitive(PrimitiveType::kSphere)
            , radius(1.0f)
        {
        }

        bool DoesIntersect(const Sphere& other) const;

        amit::maths::Vector3 centre;
        float                  radius;
    };
}  // namespace amit::geometry

#endif