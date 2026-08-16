#ifndef CORE_MATHS_GEOMETRY_BOUNDING_BOX_H_
#define CORE_MATHS_GEOMETRY_BOUNDING_BOX_H_

#include <array>
#include <vector>

#include "core/maths/linear_algebra/vector3.h"
#include "point.h"

namespace amit::geometry
{
    class BoundingBox
    {
    public:
        bool DoesCollides(const BoundingBox& other) const;

        // Computes the square distance between a point p and an AABB b
        float SquareDistancePoint(const maths::Vector3& point) const;

        // Given point p, return the point q on or in AABB b that is closest to p
        maths::Vector3 ClosestPtPointAABB(const maths::Vector3& point) const;
        float          SqDistPointAABB(maths::Vector3 point) const;

        maths::Vector3 min;
        maths::Vector3 max;
    };

    class AxisAlignedBoundingBox
    {
    public:
        AxisAlignedBoundingBox() = default;
        AxisAlignedBoundingBox(const Point3D& min, const Point3D& max)
            : min_(min)
            , max_(max)
        {
        }

        void Expand(const std::vector<Point3D>& points)
        {
            for (const auto& point : points)
            {
                AddToBox(point);
            }
        }

        bool IsInside(Point3D point) const;

        std::array<Point3D, 4> GetVertices() const
        {
            std::array<Point3D, 4> ret_vertices;
            ret_vertices[0] = Point3D(min_.x, min_.y, min_.z);
            ret_vertices[1] = Point3D(max_.x, min_.y, min_.z);
            ret_vertices[2] = Point3D(max_.x, max_.y, min_.z);
            ret_vertices[3] = Point3D(min_.x, max_.y, min_.z);
            return ret_vertices;
        }

        Point3D GetMin() const
        {
            return min_;
        }

        Point3D GetMax() const
        {
            return max_;
        }

    private:
        void AddToBox(const Point3D& point)
        {
            min_.x = std::min(min_.x, point.x);
            min_.y = std::min(min_.y, point.y);
            min_.z = std::min(min_.z, point.z);

            max_.x = std::max(max_.x, point.x);
            max_.y = std::max(max_.y, point.y);
            max_.z = std::max(max_.z, point.z);
        }

        Point3D min_{std::numeric_limits<float>::max(),
                     std::numeric_limits<float>::max(),
                     std::numeric_limits<float>::max()};

        Point3D max_{std::numeric_limits<float>::lowest(),
                     std::numeric_limits<float>::lowest(),
                     std::numeric_limits<float>::lowest()};
    };

}  // namespace amit::geometry

#endif
