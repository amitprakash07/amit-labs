#ifndef CORE_MATHS_GEOMETRY_PLANE_H_
#define CORE_MATHS_GEOMETRY_PLANE_H_

#include "primitive.h"

namespace amit::geometry
{
    class Plane : public Primitive
    {
    public:
        static Plane   ComputePlane(const maths::Vector3& vec_a,
                                    const maths::Vector3& vec_b,
                                    const maths::Vector3& vec_c);
        maths::Vector3 ClosestPointFromPointOnPlane(const maths::Vector3& point_on_plane) const;

        Plane(const maths::Vector3& normal, const maths::Vector3& point_on_plane);
        Plane()
            : Primitive(PrimitiveType::kPlane)
            , normal_(kYAxis)
            , distance_(0.0f)
        {
        }

        inline maths::Vector3 Normal() const
        {
            return normal_;
        }

        inline float Distance() const
        {
            return distance_;
        }

    private:
        maths::Vector3 normal_;    //< Unit vector representing the normal of the plane
        float          distance_;  //< distance from the origin to the plane in the direction of the normal
    };
}  // namespace amit::geometry

#endif