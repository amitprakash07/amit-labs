
#include "plane.h"

amit::geometry::Plane::Plane(const amit::maths::Vector3& normal, const amit::maths::Vector3& point_on_plane)
    : Primitive(amit::geometry::PrimitiveType::kPlane)
{
    normal_   = normal;
    float a   = normal.x();
    float b   = normal.y();
    float c   = normal.z();
    distance_ = a * point_on_plane.x() + b * point_on_plane.y() + c * point_on_plane.z();
}

amit::maths::Vector3 amit::geometry::Plane::ClosestPointFromPointOnPlane(
    const amit::maths::Vector3& point_on_plane) const
{
    float t = (normal_.Dot(point_on_plane) - distance_) / normal_.Dot(normal_);
    return (point_on_plane - (normal_ * t));
}

amit::geometry::Plane amit::geometry::Plane::ComputePlane(const amit::maths::Vector3& vec_a,
                                                              const amit::maths::Vector3& vec_b,
                                                              const amit::maths::Vector3& vec_c)
{
    Plane p;
    p.normal_   = (vec_b - vec_a).Cross(vec_c - vec_a).CreateNormalized();
    p.distance_ = p.normal_.Dot(vec_a);
    return p;
}