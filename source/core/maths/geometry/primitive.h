#ifndef CORE_MATHS_GEOMETRY_PRIMITIVE_H_
#define CORE_MATHS_GEOMETRY_PRIMITIVE_H_

#include <cstdint>

#include "core/maths/linear_algebra/vector3.h"
#include "core/common/core.h"

namespace amit::geometry
{
    enum class PrimitiveType : std::uint8_t
    {
        kUnknown = 0,
        kPoint,
        kLineSegment,
        kTriangle,
        kBoundingBox,
        kPlane,
        kSphere
    };

    inline std::string_view EnumTypeToString(PrimitiveType primitive_type)
    {
        switch (primitive_type)
        {
        case PrimitiveType::kUnknown:
            return "Unknown";
        case PrimitiveType::kPoint:
            return "Point";
        case PrimitiveType::kLineSegment:
            return "LineSegment";
        case PrimitiveType::kTriangle:
            return "Triangle";
        case PrimitiveType::kBoundingBox:
            return "BoundingBox";
        case PrimitiveType::kPlane:
            return "Plane";
        case PrimitiveType::kSphere:
            return "Sphere";
        }

        return "InvalidPrimitiveType";
    }

    const amit::maths::Vector3 kYAxis{0.0f, 1.0f, 0.0f};
    const amit::maths::Vector3 kXAxis{1.0f, 0.0f, 0.0f};
    const amit::maths::Vector3 kZAxis{0.0f, 0.0f, 1.0f};

    class Primitive
    {
    public:
        Primitive()
            : object_label_(primitive_type_)
        {
        }

        virtual ~Primitive() = default;

        virtual PrimitiveType GetType() const
        {
            return primitive_type_;
        }

        amit::common::ObjectLabel<PrimitiveType> GetObjectLabel() const
        {
            return object_label_;
        }

    protected:
        Primitive(PrimitiveType type)
            : primitive_type_(type)
            , object_label_(primitive_type_)
        {
        }

        PrimitiveType primitive_type_{PrimitiveType::kUnknown};

        amit::common::ObjectLabel<PrimitiveType> object_label_;
    };
}  // namespace amit::geometry
#endif