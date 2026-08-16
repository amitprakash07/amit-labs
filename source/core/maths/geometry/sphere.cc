#include "sphere.h"

using namespace amit::geometry;

bool Sphere::DoesIntersect(const Sphere& other) const
{
    maths::Vector3 d     = centre - other.centre;
    float   dist2 = d.Dot(d);
    // Spheres intersect if squared distance is less than squared sum of radii
    float radiusSum = radius + other.radius;
    return dist2 <= radiusSum * radiusSum;
}