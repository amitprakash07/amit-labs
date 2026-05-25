#include "rasterizer.h"

#include <array>

#include "core/graphics/barycentric_interpolation.h"
#include "core/graphics_common/color.h"
#include "core/graphics_common/render_primitives.h"
#include "core/maths/geometry/geometry.h"
#include "core/graphics/transform.h"

// Line Segment
template <>
void amit::render::cpu::Rasterizer::Rasterize(const graphics::RenderConfig& render_config,
                                              graphics::RenderState&        render_state,
                                              graphics::RenderFrameStats&   render_frame_stats,
                                              graphics::DrawOptions&        draw_options,
                                              const amit::graphics::RenderPrimitiveLine<graphics::ScreenSpace>& line,
                                              const FragmentShader& fragment_shader) const
{
    graphics::ScopedDrawCallStats draw_stats_scope(render_frame_stats, draw_options.GetStatsCollectionLevel(), line);

    (void)render_config;
    (void)render_state;

    auto RasterizeEdgeUsingLineEquation = [this, &draw_stats_scope, &fragment_shader, &line]() {
        // Equation of a line y - y1 = m(x - x1), where m = (y2 - y1) / (x2 - x1)
        float x1 = line.Start().position.x;
        float y1 = line.Start().position.y;

        float x2 = line.End().position.x;
        float y2 = line.End().position.y;

        float slope_denominator = x2 - x1;

        if (slope_denominator != 0.0f)
        {
            float slope = 0.0f;
            slope       = (y2 - y1) / slope_denominator;

            uint32_t start = 0;
            uint32_t end   = 0;

            if (x1 > x2)
            {
                start = static_cast<uint32_t>(x2);
                end   = static_cast<uint32_t>(x1);
            }
            else
            {
                start = static_cast<uint32_t>(x1);
                end   = static_cast<uint32_t>(x2);
            }

            for (uint32_t pixel_coordinate_x = start; pixel_coordinate_x < end; pixel_coordinate_x++)
            {
                float pixel_coordinate_y = y1 + (slope * (static_cast<float>(pixel_coordinate_x) - x1));

                RasterizedFragment primitive_fragment{.coordinate    = {static_cast<uint32_t>(pixel_coordinate_x),
                                                                        static_cast<uint32_t>(pixel_coordinate_y)},
                                                      .color         = graphics::kRgb8ColorRed.ToFloatColor(),
                                                      .fragment_kind = RasterizedFragment::Kind::Primitive};
                RenderFragment(draw_stats_scope, primitive_fragment, fragment_shader);
            }
        }
    };

    auto RasterizeEdgeUsingMidPointAlgo = [this, &draw_stats_scope, &fragment_shader, line]() {
        /* Bresenham Mid-point algo
         * 1. Calculate the differences dx and dy between the start and end points of the line.
         * F(x,y) = ax + by + c
         * F(x0,y0) = ax0 + by0 + c
         * F(x1,y1) = ax1 + by1 + c
         * F(x) => y = mx + b 
         * m = dy/dx
         * F(x,y) = 0 => (dy/dx)x - y + b
         * F(x,y) = 0 => dy*x - dx*y + b*dx
         * F(x,y) = Ax + By + C => A = dy, B = -dx, C = b*dx
         * Decision parameter Initial (d) = F(M) - F(x0, y0) where M is the mid-point of (x0,y0) and (x0 + 1, y0 + 1) 
         * F(M) = F(x0 +1, y0 + 1/2), and F(x0, y0) = 0
         * d = F(x0 +1, y0 + 1/2) = A(x0 +1) + B(y0 + 1/2) + C
         * d = Ax0 + A + By0 + B/2 + C
         * d = Ax0 + By0 + C + A + B/2
         * d = A + B/2 as F(x0, y0) = 0 => Ax0 + By0 + C = 0
         * d = dy + (-dx)/2 => d = dy - dx/2 => d = 2*dy - dx
         *
         * All of this for slope between 0 and 1, i.e. dx > dy
         * For other case where m > 1, we need to swap the role of x and y. It means y will move every time while
         * x change will be based on d.
         */
        int32_t x1 = static_cast<int32_t>(line.Start().position.x);
        int32_t y1 = static_cast<int32_t>(line.Start().position.y);

        int32_t x2 = static_cast<int32_t>(line.End().position.x);
        int32_t y2 = static_cast<int32_t>(line.End().position.y);

        int32_t dx = x2 - x1;
        int32_t dy = y2 - y1;

        int32_t sx = dx >= 0 ? 1 : -1;  // Step direction for x
        int32_t sy = dy >= 0 ? 1 : -1;  // Step direction for y

        dx = abs(dx);
        dy = abs(dy);

        int32_t x = x1;
        int32_t y = y1;

        if (dx >= dy)
        {
            int32_t d = 2 * dy - dx;  // Initial decision parameter

            for (int32_t i = 0; i <= dx; ++i)
            {
                RasterizedFragment primitive_fragment{
                    .coordinate    = {static_cast<uint32_t>(x), static_cast<uint32_t>(y)},
                    .color         = graphics::kRgb8ColorRed.ToFloatColor(),
                    .fragment_kind = RasterizedFragment::Kind::Primitive};
                RenderFragment(draw_stats_scope, primitive_fragment, fragment_shader);

                x = x + sx;  // Move to the next x coordinate
                if (d > 0)
                {
                    y = y + sy;               // Move to the next y coordinate
                    d = d + (2 * (dy - dx));  // Update decision parameter for the next point
                }
                else
                {
                    d = d + (2 * dy);  // Update decision parameter for the next point
                }
            }
        }
        else
        {
            int32_t d = 2 * dx - dy;  // Initial decision parameter

            for (int32_t i = 0; i <= dy; ++i)
            {
                RasterizedFragment primitive_fragment{
                    .coordinate    = {static_cast<uint32_t>(x), static_cast<uint32_t>(y)},
                    .color         = graphics::kRgb8ColorRed.ToFloatColor(),
                    .fragment_kind = RasterizedFragment::Kind::Primitive};
                RenderFragment(draw_stats_scope, primitive_fragment, fragment_shader);
                y = y + sy;  // Move to the next y coordinate
                if (d > 0)
                {
                    x = x + sx;               // Move to the next x coordinate
                    d = d + (2 * (dx - dy));  // Update decision parameter for the next point
                }
                else
                {
                    d = d + (2 * dx);  // Update decision parameter for the next point
                }
            }
        }
    };

    bool use_line_equation = false;

    if (use_line_equation)
    {
        RasterizeEdgeUsingLineEquation();
    }
    else
    {
        RasterizeEdgeUsingMidPointAlgo();
    }
}

// Triangle
template <>
void amit::render::cpu::Rasterizer::Rasterize(
    const graphics::RenderConfig&                                         render_config,
    graphics::RenderState&                                                render_state,
    graphics::RenderFrameStats&                                           render_frame_stats,
    graphics::DrawOptions&                                                draw_options,
    const amit::graphics::RenderPrimitiveTriangle<graphics::ScreenSpace>& triangle,
    const FragmentShader&                                                 fragment_shader) const
{
    graphics::ScopedDrawCallStats draw_stats_scope(
        render_frame_stats, draw_options.GetStatsCollectionLevel(), triangle);

    amit::geometry::AxisAlignedBoundingBox bounding_box;
    bounding_box.Expand({triangle.VertA().position, triangle.VertB().position, triangle.VertC().position});

    geometry::Point3D viewport_clamped_min = render_config.GetViewport().ClampToView(bounding_box.GetMin());
    geometry::Point3D viewport_clamped_max = render_config.GetViewport().ClampToView(bounding_box.GetMax());

    uint32_t x_start = static_cast<uint32_t>(viewport_clamped_min.x);
    uint32_t x_end   = static_cast<uint32_t>(viewport_clamped_max.x);

    uint32_t y_start = static_cast<uint32_t>(viewport_clamped_min.y);
    uint32_t y_end   = static_cast<uint32_t>(viewport_clamped_max.y);

    using EdgeFunctionValue = float;

    const std::array<geometry::ImplicitLineCoefficients, 3> triangle_edge_coefficients{
        geometry::MakeImplicitLineCoefficients(triangle.Edge_0().Start().position, triangle.Edge_0().End().position),
        geometry::MakeImplicitLineCoefficients(triangle.Edge_1().Start().position, triangle.Edge_1().End().position),
        geometry::MakeImplicitLineCoefficients(triangle.Edge_2().Start().position, triangle.Edge_2().End().position)};

    std::array<EdgeFunctionValue, 3> triangle_edge_function_value_row_start{
        geometry::Orient2D(triangle_edge_coefficients[0], geometry::Point3D(x_start, y_start)),
        geometry::Orient2D(triangle_edge_coefficients[1], geometry::Point3D(x_start, y_start)),
        geometry::Orient2D(triangle_edge_coefficients[2], geometry::Point3D(x_start, y_start))};

    if (draw_options.GetDrawMode() == graphics::PrimitiveDrawMode::kSolid)
    {
        for (uint32_t y_iter = y_start; y_iter < y_end; y_iter++)
        {
            std::array<EdgeFunctionValue, 3> edge_value           = triangle_edge_function_value_row_start;
            bool                             is_first_or_last_row = (y_iter == y_start || y_iter == y_end);

            for (uint32_t x_iter = x_start; x_iter < x_end; x_iter++)
            {
                bool is_first_or_last_column = (x_iter == x_start || x_iter == x_end);

                if ((edge_value[0] < 0 && edge_value[1] < 0 && edge_value[2] < 0) ||
                    (edge_value[0] >= 0 && edge_value[1] >= 0 && edge_value[2] >= 0))
                {
                    geometry::BaryCentricCoordinate barycentric_coordinate =
                        geometry::CalculateBaryCentricCoordinate(triangle.VertA().position,
                                                                 triangle.VertB().position,
                                                                 triangle.VertC().position,
                                                                 geometry::Point3D(x_iter, y_iter));

                    float interpolated_depth = triangle.VertA().position.z * barycentric_coordinate.alpha +
                                               triangle.VertB().position.z * barycentric_coordinate.beta +
                                               triangle.VertC().position.z * barycentric_coordinate.gamma;

                    if (render_state.TryUpdateDepth(graphics::ImageCoordinate{x_iter, y_iter}, interpolated_depth))
                    {
                        const graphics::FloatColor color_a = triangle.VertA().color.ToFloatColor();
                        const graphics::FloatColor color_b = triangle.VertB().color.ToFloatColor();
                        const graphics::FloatColor color_c = triangle.VertC().color.ToFloatColor();

                        amit::graphics::FloatColor interpolated_color{
                            color_a.r * barycentric_coordinate.alpha + color_b.r * barycentric_coordinate.beta +
                                color_c.r * barycentric_coordinate.gamma,
                            color_a.g * barycentric_coordinate.alpha + color_b.g * barycentric_coordinate.beta +
                                color_c.g * barycentric_coordinate.gamma,
                            color_a.b * barycentric_coordinate.alpha + color_b.b * barycentric_coordinate.beta +
                                color_c.b * barycentric_coordinate.gamma};

                        const graphics::UVCoordinate interpolated_uv = graphics::InterpolateUV(
                            triangle.VertA().uv, triangle.VertB().uv, triangle.VertC().uv, barycentric_coordinate);

                        RasterizedFragment primitive_fragment{.coordinate             = {x_iter, y_iter},
                                                              .color                  = interpolated_color,
                                                              .uv                     = interpolated_uv,
                                                              .barycentric_coordinate = barycentric_coordinate,
                                                              .depth                  = interpolated_depth,
                                                              .fragment_kind = RasterizedFragment::Kind::Primitive};

                        RenderFragment(draw_stats_scope, primitive_fragment, fragment_shader);
                    }
                }

                if (draw_options.GetDrawDebugFlag() == graphics::DrawDebugFlag::kWireframeBoundingBox &&
                    (is_first_or_last_column || is_first_or_last_row))
                {
                    RasterizedFragment bounding_box_fragment{.coordinate    = {x_iter, y_iter},
                                                             .color         = graphics::kRgb8ColorGreen.ToFloatColor(),
                                                             .fragment_kind = RasterizedFragment::Kind::BoundingBox};
                    RenderFragment(draw_stats_scope, bounding_box_fragment, fragment_shader);
                }

                edge_value[0] += triangle_edge_coefficients[0].a;
                edge_value[1] += triangle_edge_coefficients[1].a;
                edge_value[2] += triangle_edge_coefficients[2].a;
            }

            triangle_edge_function_value_row_start[0] += triangle_edge_coefficients[0].b;
            triangle_edge_function_value_row_start[1] += triangle_edge_coefficients[1].b;
            triangle_edge_function_value_row_start[2] += triangle_edge_coefficients[2].b;
        }
    }
    else if (draw_options.GetDrawMode() == graphics::PrimitiveDrawMode::kNone &&
             draw_options.GetDrawDebugFlag() == graphics::DrawDebugFlag::kWireframeBoundingBox)
    {
        for (uint32_t y_iter = y_start; y_iter <= y_end; y_iter++)
        {
            bool is_first_or_last_row = (y_iter == y_start || y_iter == y_end);

            for (uint32_t x_iter = x_start; x_iter <= x_end; x_iter++)
            {
                bool is_first_or_last_column = (x_iter == x_start || x_iter == x_end);

                if (is_first_or_last_column || is_first_or_last_row)
                {
                    RasterizedFragment bounding_box_fragment{.coordinate    = {x_iter, y_iter},
                                                             .color         = graphics::kRgb8ColorGreen.ToFloatColor(),
                                                             .fragment_kind = RasterizedFragment::Kind::BoundingBox};
                    RenderFragment(draw_stats_scope, bounding_box_fragment, fragment_shader);
                }
            }
        }
    }
    else if (draw_options.GetDrawMode() == graphics::PrimitiveDrawMode::kWireframe)
    {
        Rasterize(render_config, render_state, render_frame_stats, draw_options, triangle.Edge_0(), fragment_shader);
        Rasterize(render_config, render_state, render_frame_stats, draw_options, triangle.Edge_1(), fragment_shader);
        Rasterize(render_config, render_state, render_frame_stats, draw_options, triangle.Edge_2(), fragment_shader);
    }
}

// Line - Clip-Space

amit::render::cpu::ClipPlaneDistances amit::render::cpu::Rasterizer::GetClipPlaneDistances(
    const amit::graphics::ClipSpace::Position& clip_position) const
{
    const float* const clip_components = clip_position.AsFloatArray();

    const float x = clip_components[0];
    const float y = clip_components[1];
    const float z = clip_components[2];
    const float w = clip_components[3];

    // D3D-style homogeneous clip volume:
    // -w <= x <= w, -w <= y <= w, 0 <= z <= w.
    return {
        x + w,  // left
        w - x,  // right
        y + w,  // bottom
        w - y,  // top
        z,      // near
        w - z   // far
    };
}

bool amit::render::cpu::Rasterizer::IsFullyOutsideClipVolume(
    const amit::graphics::RenderPrimitiveTriangle<amit::graphics::ClipSpace>& triangle) const
{
    const ClipPlaneDistances vert_a_distances = GetClipPlaneDistances(triangle.VertA().position);
    const ClipPlaneDistances vert_b_distances = GetClipPlaneDistances(triangle.VertB().position);
    const ClipPlaneDistances vert_c_distances = GetClipPlaneDistances(triangle.VertC().position);

    for (std::size_t plane_index = 0; plane_index < vert_a_distances.size(); ++plane_index)
    {
        if (vert_a_distances[plane_index] < 0.0f && vert_b_distances[plane_index] < 0.0f &&
            vert_c_distances[plane_index] < 0.0f)
        {
            return true;
        }
    }

    return false;
}

bool amit::render::cpu::Rasterizer::IsFullyOutsideClipVolume(
    const amit::graphics::RenderPrimitiveLine<amit::graphics::ClipSpace>& line) const
{
    const ClipPlaneDistances start_distances = GetClipPlaneDistances(line.Start().position);
    const ClipPlaneDistances end_distances   = GetClipPlaneDistances(line.End().position);

    for (std::size_t plane_index = 0; plane_index < start_distances.size(); ++plane_index)
    {
        if (start_distances[plane_index] < 0.0f && end_distances[plane_index] < 0.0f)
        {
            return true;
        }
    }

    return false;
}

amit::graphics::VertexAttributes<amit::graphics::ScreenSpace>
amit::render::cpu::Rasterizer::TransformClipVertexToScreenSpace(
    const amit::graphics::Viewport&                                    viewport,
    const amit::graphics::VertexAttributes<amit::graphics::ClipSpace>& clip_vertex) const
{
    const amit::maths::Vector3 ndc_position    = amit::graphics::TransformClipSpaceToNdcSpace(clip_vertex.position);
    const amit::maths::Vector3 screen_position = amit::graphics::TransformNdcSpaceToScreenSpace(viewport, ndc_position);

    return amit::graphics::VertexAttributes<amit::graphics::ScreenSpace>{
        .position = amit::graphics::ScreenSpace::Position{amit::geometry::Point3D{
            screen_position.x(), screen_position.y(), screen_position.z()}},
        .color    = clip_vertex.color,
        .uv       = clip_vertex.uv};
}

template <>
void amit::render::cpu::Rasterizer::Rasterize(const graphics::RenderConfig& render_config,
                                              graphics::RenderState&        render_state,
                                              graphics::RenderFrameStats&   render_frame_stats,
                                              graphics::DrawOptions&        draw_options,
                                              const amit::graphics::RenderPrimitiveLine<graphics::ClipSpace>& line,
                                              const FragmentShader& fragment_shader) const
{
    if (IsFullyOutsideClipVolume(line))
    {
        graphics::ScopedDrawCallStats draw_stats_scope(
            render_frame_stats, draw_options.GetStatsCollectionLevel(), line);
        return;
    }

    const graphics::RenderPrimitiveLine<graphics::ScreenSpace> screen_space_line{
        TransformClipVertexToScreenSpace(render_config.GetViewport(), line.Start()),
        TransformClipVertexToScreenSpace(render_config.GetViewport(), line.End())};

    Rasterize(render_config, render_state, render_frame_stats, draw_options, screen_space_line, fragment_shader);
}

// Triangle - Clip Space
template <>
void amit::render::cpu::Rasterizer::Rasterize(
    const graphics::RenderConfig&                                       render_config,
    graphics::RenderState&                                              render_state,
    graphics::RenderFrameStats&                                         render_frame_stats,
    graphics::DrawOptions&                                              draw_options,
    const amit::graphics::RenderPrimitiveTriangle<graphics::ClipSpace>& triangle,
    const FragmentShader&                                               fragment_shader) const
{
    if (IsFullyOutsideClipVolume(triangle))
    {
        graphics::ScopedDrawCallStats draw_stats_scope(
            render_frame_stats, draw_options.GetStatsCollectionLevel(), triangle);
        return;
    }

    const graphics::RenderPrimitiveTriangle<graphics::ScreenSpace> screen_space_triangle{
        TransformClipVertexToScreenSpace(render_config.GetViewport(), triangle.VertA()),
        TransformClipVertexToScreenSpace(render_config.GetViewport(), triangle.VertB()),
        TransformClipVertexToScreenSpace(render_config.GetViewport(), triangle.VertC())};

    Rasterize(render_config, render_state, render_frame_stats, draw_options, screen_space_triangle, fragment_shader);
}
