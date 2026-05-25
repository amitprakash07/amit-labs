#ifndef CORE_CPU_RENDERER_RASTERIZER_H_
#define CORE_CPU_RENDERER_RASTERIZER_H_

#include <functional>

#include "core/maths/geometry/geometry.h"
#include "core/graphics/render_context.h"
#include "core/graphics_common/render_primitives.h"

namespace amit::render::cpu
{
    struct RasterizedFragment
    {
        enum class Kind : uint8_t
        {
            Primitive,
            BoundingBox
        };

        graphics::PixelCoordinate       coordinate;
        graphics::FloatColor            color;
        graphics::UVCoordinate          uv = {};
        geometry::BaryCentricCoordinate barycentric_coordinate;
        float                           depth = 0.0f;
        Kind                            fragment_kind;
    };

    using FragmentShader     = std::function<void(const RasterizedFragment&)>;
    using ClipPlaneDistances = std::array<float, 6>;

    class Rasterizer
    {
    public:
        Rasterizer() = default;

        template <graphics::PrimitiveAttributeCoordinateSpaceConcept coordinate_space>
        void Rasterize(const graphics::RenderConfig&,
                       graphics::RenderState&,
                       graphics::RenderFrameStats&,
                       graphics::DrawOptions&,
                       const amit::graphics::RenderPrimitiveLine<coordinate_space>&,
                       const FragmentShader&) const
        {
        }

        template <graphics::PrimitiveAttributeCoordinateSpaceConcept coordinate_space>
        void Rasterize(const graphics::RenderConfig&,
                       graphics::RenderState&,
                       graphics::RenderFrameStats&,
                       graphics::DrawOptions&,
                       const amit::graphics::RenderPrimitiveTriangle<coordinate_space>&,
                       const FragmentShader&) const
        {
        }

    private:
        void RenderFragment(graphics::ScopedDrawCallStats& draw_stats_scope,
                            const RasterizedFragment&      fragment,
                            const FragmentShader&          fragment_shader) const
        {
            fragment_shader(fragment);
            draw_stats_scope.IncrementStatCount(graphics::RenderStatCounter::kRasterizedPixelCount);
        }

        ClipPlaneDistances GetClipPlaneDistances(const amit::graphics::ClipSpace::Position& clip_position) const;

        bool IsFullyOutsideClipVolume(
            const amit::graphics::RenderPrimitiveTriangle<amit::graphics::ClipSpace>& triangle) const;
        bool IsFullyOutsideClipVolume(const amit::graphics::RenderPrimitiveLine<amit::graphics::ClipSpace>& line) const;

        amit::graphics::VertexAttributes<amit::graphics::ScreenSpace> TransformClipVertexToScreenSpace(
            const amit::graphics::Viewport&                                    viewport,
            const amit::graphics::VertexAttributes<amit::graphics::ClipSpace>& clip_vertex) const;
    };

    // Screen space

    template <>
    void Rasterizer::Rasterize(const graphics::RenderConfig&,
                               graphics::RenderState&,
                               graphics::RenderFrameStats&,
                               graphics::DrawOptions&,
                               const amit::graphics::RenderPrimitiveLine<graphics::ScreenSpace>&,
                               const FragmentShader&) const;

    template <>
    void Rasterizer::Rasterize(const graphics::RenderConfig&,
                               graphics::RenderState&,
                               graphics::RenderFrameStats&,
                               graphics::DrawOptions&,
                               const amit::graphics::RenderPrimitiveTriangle<graphics::ScreenSpace>&,
                               const FragmentShader&) const;

    // Clip Space
    template <>
    void Rasterizer::Rasterize(const graphics::RenderConfig&,
                               graphics::RenderState&,
                               graphics::RenderFrameStats&,
                               graphics::DrawOptions&,
                               const amit::graphics::RenderPrimitiveLine<graphics::ClipSpace>&,
                               const FragmentShader&) const;

    template <>
    void Rasterizer::Rasterize(const graphics::RenderConfig&,
                               graphics::RenderState&,
                               graphics::RenderFrameStats&,
                               graphics::DrawOptions&,
                               const amit::graphics::RenderPrimitiveTriangle<graphics::ClipSpace>&,
                               const FragmentShader&) const;

}  // namespace amit::render::cpu

#endif