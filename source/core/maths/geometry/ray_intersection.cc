#include "utils.h"
#include "ray_intersection.h"

using namespace amit::geometry;

HitInfo::HitInfo()
    : distance(amit::maths::kBigFloat)
{
}

template <>
bool Ray::Intersect<Plane>(const Plane& primitive, HitInfo& hit_info) const
{
    const Plane& plane = primitive;
    float        t     = (plane.Distance() - plane.Normal().Dot(origin)) / plane.Normal().Dot(direction);

    if (t >= 0.0f && t <= 1.0f)
    {
        hit_info.hit_point = origin + direction * t;
        hit_info.distance  = t;
        hit_info.normal    = maths::Vector3(0, 0, 1);
        return true;
    }

    return false;
}

template <>
bool Ray::Intersect<Sphere>(const Sphere& primitive, HitInfo& hit_info) const
{
    // Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting,
    // returns t value of intersection and intersection point q

    const Sphere& sphere = primitive;
    maths::Vector3          m      = origin - sphere.centre;
    float                   b      = m.Dot(direction);
    float                   c      = m.Dot(m) - sphere.radius * sphere.radius;

    // Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
    if (c > 0.0f && b > 0.0f)
    {
        return false;
    }

    float discriminant = b * b - c;

    // A negative discriminant corresponds to ray missing sphere
    if (discriminant < 0.0f)
    {
        return false;
    }

    // Ray now found to intersect sphere, compute smallest t value of intersection
    hit_info.distance = -b - sqrtf(discriminant);

    // If t is negative, ray started inside sphere so clamp t to zero
    if (hit_info.distance < 0.0f)
    {
        hit_info.distance = 0.0f;
    }

    hit_info.hit_point = origin + direction * hit_info.distance;
    return true;
}

template <>
bool Ray::Intersect<BoundingBox>(const BoundingBox& primitive, HitInfo& hit_info) const
{
    // Intersect ray R(t) = p + t*d against AABB a. When intersecting,
    // return intersection distance tmin and point q of intersection

    const BoundingBox& bounding_box = primitive;

    bool  is_hit = false;
    float tx1, tx2, tx_entry, tx_exit;
    float ty1, ty2, ty_entry, ty_exit;
    float tz1, tz2, tz_entry, tz_exit;

    tx1 = tx2 = tx_entry = tx_exit = ty1 = ty2 = ty_entry = ty_exit = tz1 = tz2 = tz_entry = tz_exit = amit::maths::kBigFloat;

    // Check X-Normal
    if (direction.x() != 0.0f)
    {
        tx1 = (-origin.x() + bounding_box.min.x()) / direction.x();
        tx2 = (-origin.x() + bounding_box.max.x()) / direction.x();

        tx_entry = tx1 < tx2 ? tx1 : tx2;
        tx_exit  = tx1 > tx2 ? tx1 : tx2;
    }
    else
    {
        if (origin.x() > bounding_box.max.x() || origin.x() < bounding_box.min.x())
        {
            return false;
        }
    }

    // Check y-Plane
    if (direction.y() != 0.0f)
    {
        ty1      = (-origin.y() + bounding_box.min.y()) / direction.y();
        ty2      = (-origin.y() + bounding_box.max.y()) / direction.y();
        ty_entry = ty1 < ty2 ? ty1 : ty2;
        ty_exit  = ty1 > ty2 ? ty1 : ty2;
    }
    else
    {
        if (origin.y() > bounding_box.max.y() || origin.y() < bounding_box.min.y())
        {
            return false;
        }
    }

    // Check z-Plane
    if (direction.z() != 0.0f)
    {
        tz1      = (-origin.z() + bounding_box.min.z()) / direction.z();
        tz2      = (-origin.z() + bounding_box.max.z()) / direction.z();
        tz_entry = tz1 < tz2 ? tz1 : tz2;
        tz_exit  = tz1 > tz2 ? tz1 : tz2;
    }
    else
    {
        if (origin.z() > bounding_box.max.z() || origin.z() < bounding_box.min.z())
        {
            return false;
        }
    }

    float tEntry = (tx_entry > (ty_entry > tz_entry ? ty_entry : tz_entry)) ? tx_entry : (ty_entry > tz_entry ? ty_entry : tz_entry);
    float tExit  = tx_exit < (ty_exit < tz_exit ? ty_exit : tz_exit) ? tx_exit : (ty_exit < tz_exit ? ty_exit : tz_exit);

    if (tEntry < tExit)
    {
        is_hit = true;
    }

    return is_hit;
}

template <>
bool Ray::Intersect<Triangle>(const Triangle& primitive, HitInfo& hit_info) const
{
    primitive;
    hit_info;
    return false;
    /*const Triangle& triangle = primitive;
    const Vertex&   vertex_a = triangle.Vert_A();
    const Vertex&   vertex_b = triangle.Vert_B();
    const Vertex&   vertex_c = triangle.Vert_C();

    Vector3 normal               = (vertex_b - vertex_a).Cross(vertex_c - vertex_a);
    Vector3 face_normal_normalized = normal.CreateNormalized();

    if (direction.Dot(face_normal_normalized) <= kEpsilon)
    {
        return false;
    }

    float t = (-(origin - vertex_a)).Dot(face_normal_normalized) / direction.Dot(face_normal_normalized);

    if ((origin - vertex_a).Dot(face_normal_normalized) <= kEpsilon)
    {
        return false;
    }

    bool is_hit = false;
    if (t < hit_info.distance)
    {
        Vector3 p          = origin + direction * t;
        hit_info.hit_point = p;
        hit_info.normal    = face_normal_normalized;
        hit_info.distance  = t;
        is_hit              = true;
    }

    return is_hit;*/
}
