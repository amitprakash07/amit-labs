#ifndef CORE_CPU_RENDERER_RASTERIZER_H_
#define CORE_CPU_RENDERER_RASTERIZER_H_

#include <functional>

#include "core/maths/geometry/geometry.h"
#include "core/maths/geometry/geometry_primitives.h"
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

    using FragmentShader = std::function<void(const RasterizedFragment&)>;

    class Rasterizer
    {
    public:
        Rasterizer() = default;

        template <graphics::CoordinateSpaceConcept coordinate_space>
        void Rasterize(const graphics::RenderConfig&,
                       graphics::RenderState&,
                       graphics::RenderFrameStats&,
                       graphics::DrawOptions&,
                       const amit::graphics::RenderPrimitiveLine<coordinate_space>&,
                       const FragmentShader&) const
        {
        }

        template <graphics::CoordinateSpaceConcept coordinate_space>
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