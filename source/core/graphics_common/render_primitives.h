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
    struct VertexAttributes
    {
        geometry::Point3D      position;
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
    class RenderPrimitive;

    template <>
    class RenderPrimitive<RenderPrimitiveType::kPoint>
    {
    public:
        RenderPrimitive()
            : point_vertex_{}
            , object_label_(RenderPrimitiveType::kPoint)
        {
        }

        RenderPrimitive(const VertexAttributes& point)
            : point_vertex_{point}
            , object_label_(RenderPrimitiveType::kPoint)
        {
        }

        const VertexAttributes& PointVertex() const
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
        VertexAttributes                 point_vertex_;
        ObjectLabel<RenderPrimitiveType> object_label_;
    };

    template <>
    class RenderPrimitive<RenderPrimitiveType::kLine>
    {
    public:
        RenderPrimitive()
            : end_points_{}
            , object_label_(RenderPrimitiveType::kLine)
        {
        }

        RenderPrimitive(const VertexAttributes& start, const VertexAttributes& end)
            : end_points_{start, end}
            , object_label_(RenderPrimitiveType::kLine)

        {
        }

        RenderPrimitive(const std::array<VertexAttributes, 2>& end_points)
            : end_points_(end_points)
            , object_label_(RenderPrimitiveType::kLine)
        {
        }

        const VertexAttributes& Start() const
        {
            return end_points_[0];
        }

        const VertexAttributes& End() const
        {
            return end_points_[1];
        }

        std::array<VertexAttributes, 2> GetEndPoints() const
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
        std::array<VertexAttributes, 2>  end_points_;
        ObjectLabel<RenderPrimitiveType> object_label_;
    };

    template <>
    class RenderPrimitive<RenderPrimitiveType::kTriangle>
    {
    public:
        RenderPrimitive(const VertexAttributes& vert_a, const VertexAttributes& vert_b, const VertexAttributes& vert_c)
            : vertices_{vert_a, vert_b, vert_c}
            , object_label_(RenderPrimitiveType::kTriangle)
        {
        }

        RenderPrimitive(const std::array<VertexAttributes, 3>& vertices)
            : vertices_(vertices)
            , object_label_(RenderPrimitiveType::kTriangle)
        {
        }

        const VertexAttributes& VertA() const
        {
            return vertices_[0];
        }

        const VertexAttributes& VertB() const
        {
            return vertices_[1];
        }

        const VertexAttributes& VertC() const
        {
            return vertices_[2];
        }

        RenderPrimitive<RenderPrimitiveType::kLine> Edge_0() const
        {
            return {vertices_[0], vertices_[1]};
        }

        RenderPrimitive<RenderPrimitiveType::kLine> Edge_1() const
        {
            return {vertices_[1], vertices_[2]};
        }

        RenderPrimitive<RenderPrimitiveType::kLine> Edge_2() const
        {
            return {vertices_[2], vertices_[0]};
        }

        std::array<VertexAttributes, 3> GetVertices() const
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
        std::array<VertexAttributes, 3>  vertices_;
        ObjectLabel<RenderPrimitiveType> object_label_;
    };

}  // namespace amit::graphics

#endif
