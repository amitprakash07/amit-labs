#ifndef CORE_GRAPHICS_COMMON_RENDER_PRIMITIVES_H_
#define CORE_GRAPHICS_COMMON_RENDER_PRIMITIVES_H_

#include <array>
#include <string_view>

#include "core/common/core.h"
#include "core/maths/geometry/point.h"

#include "color.h"
#include "graphics_common.h"
#include "vector4.h"

namespace amit::graphics
{
    using LocalSpace  = geometry::Point3D;
    using WorldSpace  = maths::Vector3;
    using ClipSpace   = maths::Vector4;
    using NDCSpace    = maths::Vector3;
    using ScreenSpace = geometry::Point3D;  // x,y and z for the depth buffer

    template <typename TCoordinateSpace>
    concept CoordinateSpaceConcept =
        std::is_same_v<TCoordinateSpace, LocalSpace> || std::is_same_v<TCoordinateSpace, WorldSpace> ||
        std::is_same_v<TCoordinateSpace, ClipSpace> || std::is_same_v<TCoordinateSpace, NDCSpace>;

    template <CoordinateSpaceConcept CoordinateSpace>
    struct VertexAttributes
    {
        CoordinateSpace        position;
        graphics::Rgb8         color;
        graphics::UVCoordinate uv;
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

    template <CoordinateSpaceConcept CoordinateSpace>
    class RenderPrimitivePoint : public RenderPrimitive<RenderPrimitiveType::kPoint>
    {
    public:
        RenderPrimitivePoint()
            : point_vertex_{}
            , object_label_(RenderPrimitiveType::kPoint)
        {
        }

        RenderPrimitivePoint(const VertexAttributes<CoordinateSpace>& point)
            : point_vertex_{point}
            , object_label_(RenderPrimitiveType::kPoint)
        {
        }

        const VertexAttributes<CoordinateSpace>& PointVertex() const
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
        VertexAttributes<CoordinateSpace> point_vertex_;
        ObjectLabel<RenderPrimitiveType>  object_label_;
    };

    template <CoordinateSpaceConcept CoordinateSpace>
    class RenderPrimitiveLine : public RenderPrimitive<RenderPrimitiveType::kLine>
    {
    public:
        RenderPrimitiveLine()
            : end_points_{}
            , object_label_(RenderPrimitiveType::kLine)
        {
        }

        RenderPrimitiveLine(const VertexAttributes<CoordinateSpace>& start,
                            const VertexAttributes<CoordinateSpace>& end)
            : end_points_{start, end}
            , object_label_(RenderPrimitiveType::kLine)

        {
        }

        RenderPrimitiveLine(const std::array<VertexAttributes<CoordinateSpace>, 2>& end_points)
            : end_points_(end_points)
            , object_label_(RenderPrimitiveType::kLine)
        {
        }

        const VertexAttributes<CoordinateSpace>& Start() const
        {
            return end_points_[0];
        }

        const VertexAttributes<CoordinateSpace>& End() const
        {
            return end_points_[1];
        }

        std::array<VertexAttributes<CoordinateSpace>, 2> GetEndPoints() const
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
        std::array<VertexAttributes<CoordinateSpace>, 2> end_points_;
        ObjectLabel<RenderPrimitiveType>                 object_label_;
    };

    template <CoordinateSpaceConcept CoordinateSpace>
    class RenderPrimitiveTriangle : public RenderPrimitive<RenderPrimitiveType::kTriangle>
    {
    public:
        RenderPrimitiveTriangle(const VertexAttributes<CoordinateSpace>& vert_a,
                                const VertexAttributes<CoordinateSpace>& vert_b,
                                const VertexAttributes<CoordinateSpace>& vert_c)
            : vertices_{vert_a, vert_b, vert_c}
            , object_label_(RenderPrimitiveType::kTriangle)
        {
        }

        RenderPrimitiveTriangle(const std::array<VertexAttributes<CoordinateSpace>, 3>& vertices)
            : vertices_(vertices)
            , object_label_(RenderPrimitiveType::kTriangle)
        {
        }

        const VertexAttributes<CoordinateSpace>& VertA() const
        {
            return vertices_[0];
        }

        const VertexAttributes<CoordinateSpace>& VertB() const
        {
            return vertices_[1];
        }

        const VertexAttributes<CoordinateSpace>& VertC() const
        {
            return vertices_[2];
        }

        RenderPrimitiveLine<CoordinateSpace> Edge_0() const
        {
            return {vertices_[0], vertices_[1]};
        }

        RenderPrimitiveLine<CoordinateSpace> Edge_1() const
        {
            return {vertices_[1], vertices_[2]};
        }

        RenderPrimitiveLine<CoordinateSpace> Edge_2() const
        {
            return {vertices_[2], vertices_[0]};
        }

        std::array<VertexAttributes<CoordinateSpace>, 3> GetVertices() const
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
        std::array<VertexAttributes<CoordinateSpace>, 3> vertices_;
        ObjectLabel<RenderPrimitiveType>                 object_label_;
    };
}  // namespace amit::graphics

#endif
