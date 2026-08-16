#ifndef CORE_GRAPHICS_COMMON_RENDER_PRIMITIVES_H_
#define CORE_GRAPHICS_COMMON_RENDER_PRIMITIVES_H_

#include <array>
#include <format>
#include <string>
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
        static constexpr std::string_view ToString()
        {
            return "LocalSpace";
        }

        struct Position : geometry::Point3D
        {
            using geometry::Point3D::Point3D;

            Position() = default;

            explicit Position(const geometry::Point3D& point)
                : geometry::Point3D(point)
            {
            }

            std::string ToString() const
            {
                return std::format("({}, {}, {})", x, y, z);
            }
        };
    };

    struct WorldSpace
    {
        static constexpr std::string_view ToString()
        {
            return "WorldSpace";
        }

        struct Position : maths::Vector3
        {
            using maths::Vector3::Vector3;

            Position() = default;

            explicit Position(const maths::Vector3& vector)
                : maths::Vector3(vector)
            {
            }

            std::string ToString() const
            {
                return std::format("({}, {}, {})", x(), y(), z());
            }
        };
    };

    struct ClipSpace
    {
        static constexpr std::string_view ToString()
        {
            return "ClipSpace";
        }

        struct Position : maths::Vector4
        {
            using maths::Vector4::Vector4;

            Position() = default;

            explicit Position(const maths::Vector4& vector)
                : maths::Vector4(vector)
            {
            }

            std::string ToString() const
            {
                const float* const components = AsFloatArray();
                return std::format("({}, {}, {}, {})", components[0], components[1], components[2], components[3]);
            }
        };
    };

    struct NDCSpace
    {
        static constexpr std::string_view ToString()
        {
            return "NDCSpace";
        }

        struct Position : maths::Vector3
        {
            using maths::Vector3::Vector3;

            Position() = default;

            explicit Position(const maths::Vector3& vector)
                : maths::Vector3(vector)
            {
            }

            std::string ToString() const
            {
                return std::format("({}, {}, {})", x(), y(), z());
            }
        };
    };

    struct ScreenSpace
    {
        static constexpr std::string_view ToString()
        {
            return "ScreenSpace";
        }

        struct Position : geometry::Point3D
        {
            using geometry::Point3D::Point3D;

            Position() = default;

            explicit Position(const geometry::Point3D& point)
                : geometry::Point3D(point)
            {
            }

            std::string ToString() const
            {
                return std::format("({}, {}, {})", x, y, z);
            }
        };
    };

    template <typename AttributeCoordinateSpace>
    concept PrimitiveAttributeCoordinateSpaceConcept =
        requires {
            typename AttributeCoordinateSpace::Position;
            { AttributeCoordinateSpace::ToString() } -> std::convertible_to<std::string_view>;
            {
                std::declval<const AttributeCoordinateSpace::Position>().ToString()
            } -> std::convertible_to<std::string_view>;
        } &&
        (std::is_same_v<AttributeCoordinateSpace, LocalSpace> || std::is_same_v<AttributeCoordinateSpace, WorldSpace> ||
         std::is_same_v<AttributeCoordinateSpace, ClipSpace> || std::is_same_v<AttributeCoordinateSpace, NDCSpace> ||
         std::is_same_v<AttributeCoordinateSpace, ScreenSpace>);

    template <PrimitiveAttributeCoordinateSpaceConcept AttributeSpaceType>
    struct VertexAttributes
    {
        static constexpr std::string_view CoordinateSpaceLabel()
        {
            return AttributeSpaceType::ToString();
        }

        std::string ToString() const
        {
            return std::format("[Space:{}, position={}, color=({}, {}, {}) uv=({}, {})",
                               AttributeSpaceType::ToString(),
                               position.ToString(),
                               color.r,
                               color.g,
                               color.b,
                               uv.x,
                               uv.y);
        }

        AttributeSpaceType::Position position;
        Rgb8               color;
        UVCoordinate       uv;
    };

    template <typename VertexAttributesType>
    concept VertexAttributesConcept = requires(const VertexAttributesType& vertex_attributes) {
        { VertexAttributesType::CoordinateSpaceLabel() } -> std::convertible_to<std::string_view>;
        { vertex_attributes.ToString() } -> std::convertible_to<std::string>;
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

        amit::common::ObjectLabel<RenderPrimitiveType> GetObjectLabel() const
        {
            return object_label_;
        }

        const amit::common::ObjectId& GetId() const
        {
            return object_label_.GetId();
        }

        static constexpr std::string_view PrimitiveKindLabel()
        {
            return EnumTypeToString(RenderPrimitiveType::kPoint);
        }

        std::string ToString() const
        {
            return std::format(
                "{} {} vertex={}", PrimitiveKindLabel(), object_label_.ToString(), point_vertex_.ToString());
        }

    private:
        VertexAttributes<PrimitiveAttributeCoordinateSpace> point_vertex_;
        amit::common::ObjectLabel<RenderPrimitiveType>                    object_label_;
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

        amit::common::ObjectLabel<RenderPrimitiveType> GetObjectLabel() const
        {
            return object_label_;
        }

        const amit::common::ObjectId& GetId() const
        {
            return object_label_.GetId();
        }

        static constexpr std::string_view PrimitiveKindLabel()
        {
            return EnumTypeToString(RenderPrimitiveType::kLine);
        }

        std::string ToString() const
        {
            return std::format("{}\n\tstart={}\n\tend={}",
                               object_label_.ToString(),
                               end_points_[0].ToString(),
                               end_points_[1].ToString());
        }

    private:
        std::array<VertexAttributes<PrimitiveAttributeCoordinateSpace>, 2> end_points_;
        amit::common::ObjectLabel<RenderPrimitiveType>                                   object_label_;
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

        amit::common::ObjectLabel<RenderPrimitiveType> GetObjectLabel() const
        {
            return object_label_;
        }

        const amit::common::ObjectId& GetId() const
        {
            return object_label_.GetId();
        }

        static constexpr std::string_view PrimitiveKindLabel()
        {
            return EnumTypeToString(RenderPrimitiveType::kTriangle);
        }

        std::string ToString() const
        {
            return std::format("{} {} vert_a={} vert_b={} vert_c={}",
                               PrimitiveKindLabel(),
                               object_label_.ToString(),
                               vertices_[0].ToString(),
                               vertices_[1].ToString(),
                               vertices_[2].ToString());
        }

    private:
        std::array<VertexAttributes<PrimitiveAttributeCoordinateSpace>, 3> vertices_;
        amit::common::ObjectLabel<RenderPrimitiveType>                                   object_label_;
    };
}  // namespace amit::graphics

#endif
