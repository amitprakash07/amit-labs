#ifndef CORE_GRAPHICS_COMMON_RENDER_PRIMITIVES_H_
#define CORE_GRAPHICS_COMMON_RENDER_PRIMITIVES_H_

#include <array>
#include <string_view>
#include <type_traits>

#include "core/common/core.h"
#include "core/maths/geometry/point.h"

#include "color.h"
#include "graphics_common.h"
#include "vector4.h"

namespace amit::graphics
{
    struct LocalSpace
    {
        struct Position : geometry::Point3D
        {
            using geometry::Point3D::Point3D;

            Position() = default;

            explicit Position(const geometry::Point3D& point)
                : geometry::Point3D(point)
            {
            }
        };
    };

    struct WorldSpace
    {
        struct Position : maths::Vector3
        {
            using maths::Vector3::Vector3;

            Position() = default;

            explicit Position(const maths::Vector3& vector)
                : maths::Vector3(vector)
            {
            }
        };
    };

    struct ClipSpace
    {
        struct Position : maths::Vector4
        {
            using maths::Vector4::Vector4;

            Position() = default;

            explicit Position(const maths::Vector4& vector)
                : maths::Vector4(vector)
            {
            }
        };
    };

    struct NDCSpace
    {
        struct Position : maths::Vector3
        {
            using maths::Vector3::Vector3;

            Position() = default;

            explicit Position(const maths::Vector3& vector)
                : maths::Vector3(vector)
            {
            }
        };
    };

    struct ScreenSpace
    {
        struct Position : geometry::Point3D
        {
            using geometry::Point3D::Point3D;

            Position() = default;

            explicit Position(const geometry::Point3D& point)
                : geometry::Point3D(point)
            {
            }
        };
    };

    template <typename AttributeCoordinateSpace>
    concept PrimitiveAttributeCoordinateSpaceConcept =
        requires { typename AttributeCoordinateSpace::Position; } &&
        (std::is_same_v<AttributeCoordinateSpace, LocalSpace> || std::is_same_v<AttributeCoordinateSpace, WorldSpace> ||
         std::is_same_v<AttributeCoordinateSpace, ClipSpace> || std::is_same_v<AttributeCoordinateSpace, NDCSpace> ||
         std::is_same_v<AttributeCoordinateSpace, ScreenSpace>);

    template <PrimitiveAttributeCoordinateSpaceConcept AttributeSpaceType>
    struct VertexAttributes
    {
        AttributeSpaceType::Position position;
        graphics::Rgb8               color;
        graphics::UVCoordinate       uv;
    };

    enum class RenderPrimitiveType
    {
        kPoint    = 0x1,
        kLine     = 0x2,
        kTriangle = 0x3,
    };

    inline std::string_view EnumTypeToString(RenderPrimitiveType primitive_type)
    {
        switch (primitive_type)
        {
        case RenderPrimitiveType::kPoint:
            return "Point";
        case RenderPrimitiveType::kLine:
            return "Line";
        case RenderPrimitiveType::kTriangle:
            return "Triangle";
        }

        return "InvalidPrimitiveType";
    }

    template <RenderPrimitiveType PrimitiveType>
    class RenderPrimitive
    {
    protected:
        RenderPrimitive() = default;
    };

    template <PrimitiveAttributeCoordinateSpaceConcept PrimitiveAttributeCoordinateSpace>
    class RenderPrimitivePoint : public RenderPrimitive<RenderPrimitiveType::kPoint>
    {
    public:
        RenderPrimitivePoint()
            : point_vertex_{}
            , object_label_(RenderPrimitiveType::kPoint)
        {
        }

        RenderPrimitivePoint(const VertexAttributes<PrimitiveAttributeCoordinateSpace>& point)
            : point_vertex_{point}
            , object_label_(RenderPrimitiveType::kPoint)
        {
        }

        const VertexAttributes<PrimitiveAttributeCoordinateSpace>& PointVertex() const
        {
            return point_vertex_;
        }

        ObjectLabel<RenderPrimitiveType> GetObjectLabel() const
        {
            return object_label_;
        }

        const ObjectId& GetId() const
        {
            return object_label_.GetId();
        }

    private:
        VertexAttributes<PrimitiveAttributeCoordinateSpace> point_vertex_;
        ObjectLabel<RenderPrimitiveType>          object_label_;
    };

    template <PrimitiveAttributeCoordinateSpaceConcept PrimitiveAttributeCoordinateSpace>
    class RenderPrimitiveLine : public RenderPrimitive<RenderPrimitiveType::kLine>
    {
    public:
        RenderPrimitiveLine()
            : end_points_{}
            , object_label_(RenderPrimitiveType::kLine)
        {
        }

        RenderPrimitiveLine(const VertexAttributes<PrimitiveAttributeCoordinateSpace>& start,
                            const VertexAttributes<PrimitiveAttributeCoordinateSpace>& end)
            : end_points_{start, end}
            , object_label_(RenderPrimitiveType::kLine)

        {
        }

        RenderPrimitiveLine(const std::array<VertexAttributes<PrimitiveAttributeCoordinateSpace>, 2>& end_points)
            : end_points_(end_points)
            , object_label_(RenderPrimitiveType::kLine)
        {
        }

        const VertexAttributes<PrimitiveAttributeCoordinateSpace>& Start() const
        {
            return end_points_[0];
        }

        const VertexAttributes<PrimitiveAttributeCoordinateSpace>& End() const
        {
            return end_points_[1];
        }

        std::array<VertexAttributes<PrimitiveAttributeCoordinateSpace>, 2> GetEndPoints() const
        {
            return end_points_;
        }

        ObjectLabel<RenderPrimitiveType> GetObjectLabel() const
        {
            return object_label_;
        }

        const ObjectId& GetId() const
        {
            return object_label_.GetId();
        }

    private:
        std::array<VertexAttributes<PrimitiveAttributeCoordinateSpace>, 2> end_points_;
        ObjectLabel<RenderPrimitiveType>                         object_label_;
    };

    template <PrimitiveAttributeCoordinateSpaceConcept PrimitiveAttributeCoordinateSpace>
    class RenderPrimitiveTriangle : public RenderPrimitive<RenderPrimitiveType::kTriangle>
    {
    public:
        RenderPrimitiveTriangle(const VertexAttributes<PrimitiveAttributeCoordinateSpace>& vert_a,
                                const VertexAttributes<PrimitiveAttributeCoordinateSpace>& vert_b,
                                const VertexAttributes<PrimitiveAttributeCoordinateSpace>& vert_c)
            : vertices_{vert_a, vert_b, vert_c}
            , object_label_(RenderPrimitiveType::kTriangle)
        {
        }

        RenderPrimitiveTriangle(const std::array<VertexAttributes<PrimitiveAttributeCoordinateSpace>, 3>& vertices)
            : vertices_(vertices)
            , object_label_(RenderPrimitiveType::kTriangle)
        {
        }

        const VertexAttributes<PrimitiveAttributeCoordinateSpace>& VertA() const
        {
            return vertices_[0];
        }

        const VertexAttributes<PrimitiveAttributeCoordinateSpace>& VertB() const
        {
            return vertices_[1];
        }

        const VertexAttributes<PrimitiveAttributeCoordinateSpace>& VertC() const
        {
            return vertices_[2];
        }

        RenderPrimitiveLine<PrimitiveAttributeCoordinateSpace> Edge_0() const
        {
            return {vertices_[0], vertices_[1]};
        }

        RenderPrimitiveLine<PrimitiveAttributeCoordinateSpace> Edge_1() const
        {
            return {vertices_[1], vertices_[2]};
        }

        RenderPrimitiveLine<PrimitiveAttributeCoordinateSpace> Edge_2() const
        {
            return {vertices_[2], vertices_[0]};
        }

        std::array<VertexAttributes<PrimitiveAttributeCoordinateSpace>, 3> GetVertices() const
        {
            return vertices_;
        }

        ObjectLabel<RenderPrimitiveType> GetObjectLabel() const
        {
            return object_label_;
        }

        const ObjectId& GetId() const
        {
            return object_label_.GetId();
        }

    private:
        std::array<VertexAttributes<PrimitiveAttributeCoordinateSpace>, 3> vertices_;
        ObjectLabel<RenderPrimitiveType>                         object_label_;
    };
}  // namespace amit::graphics

#endif
