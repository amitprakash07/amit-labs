#include "cpu_rasterizer_pipeline.h"

#include <iostream>
#include <string>

#include "core/common/perf_stats.h"
#include "core/cpu_renderer/rasterizer.h"
#include "core/graphics/image_2d.h"
#include "core/graphics/image_generator.h"
#include "core/graphics/render_context.h"
#include "core/graphics/texture.h"
#include "core/graphics/text_overlay.h"
#include "core/graphics/transform.h"
#include "core/maths/geometry/point.h"

namespace amit::cpu_rasterizer
{
    void RasterizeInScreenSpace(const graphics::Viewport& viewport)
    {
        auto make_vertex_in_screen_space = [&viewport](const geometry::Point3D& position, const graphics::Rgb8& color) {
            const float uv_x = position.x / static_cast<float>(viewport.GetWidth().value);
            const float uv_y = position.y / static_cast<float>(viewport.GetHeight().value);

            return graphics::VertexAttributes<graphics::ScreenSpace>{
                .position = graphics::ScreenSpace::Position{position}, .color = color, .uv = {uv_x, uv_y}};
        };

        graphics::RenderPrimitiveTriangle<graphics::ScreenSpace> screen_space_triangle_1{
            make_vertex_in_screen_space(geometry::Point3D{500u, 100u, 10u}, graphics::kRgb8ColorRed),
            make_vertex_in_screen_space(geometry::Point3D{300u, 500u, 10u}, graphics::kRgb8ColorGreen),
            make_vertex_in_screen_space(geometry::Point3D{700u, 500u, 10u}, graphics::kRgb8ColorBlue)};

        graphics::RenderPrimitiveTriangle<graphics::ScreenSpace> screen_space_triangle_2{
            make_vertex_in_screen_space(geometry::Point3D{200u, 200u, 20u}, graphics::kRgb8ColorYellow),
            make_vertex_in_screen_space(geometry::Point3D{200u, 550u, 20u}, graphics::kRgb8ColorRed),
            make_vertex_in_screen_space(geometry::Point3D{500u, 550u, 20u}, graphics::kRgb8ColorYellow)};

        std::cout << "Rasterization started\n";

        graphics::RenderFrame   render_frame(viewport);
        graphics::RenderOutput& render_output = render_frame.GetRenderOutput();

        graphics::Image2D<graphics::Rgb8> checker_board =
            graphics::ImageGenerator::GetCheckerBoard({.value = 200}, {.value = 200}, {.value = 8});
        graphics::Texture<graphics::Rgb8> checker_texture(checker_board);

        auto color_shader = [&render_output](const render::cpu::RasterizedFragment& raster_fragment) {
            graphics::ImageCoordinate image_coordinate{raster_fragment.coordinate.x, raster_fragment.coordinate.y};
            render_output.GetColorBuffer().SetImageData(image_coordinate, raster_fragment.color.ToRgb8());
        };

        auto checkerboard_shader = [&render_output,
                                    &checker_texture](const render::cpu::RasterizedFragment& raster_fragment) {
            graphics::ImageCoordinate image_coordinate{raster_fragment.coordinate.x, raster_fragment.coordinate.y};

            if (raster_fragment.fragment_kind == render::cpu::RasterizedFragment::Kind::BoundingBox)
            {
                render_output.GetColorBuffer().SetImageData(image_coordinate, raster_fragment.color.ToRgb8());
                return;
            }

            render_output.GetColorBuffer().SetImageData(image_coordinate, checker_texture.Sample(raster_fragment.uv));
        };

        graphics::DrawOptions draw_options(
            graphics::PrimitiveDrawMode::kSolid, graphics::DrawDebugFlag::kNone, graphics::StatsCollectionLevel::kDraw);

        render::cpu::Rasterizer triangle_rasterizer;
        triangle_rasterizer.Rasterize(render_frame.GetRenderConfig(),
                                      render_frame.GetRenderState(),
                                      render_frame.GetRenderFrameStats(),
                                      draw_options,
                                      screen_space_triangle_2,
                                      checkerboard_shader);

        triangle_rasterizer.Rasterize(render_frame.GetRenderConfig(),
                                      render_frame.GetRenderState(),
                                      render_frame.GetRenderFrameStats(),
                                      draw_options,
                                      screen_space_triangle_1,
                                      color_shader);

        std::cout << "Rasterization Finished\n";

        std::string overlay_stats = render_frame.GetRenderFrameStats().ToOverlayString();
        std::string detailed_stats;
        if (draw_options.ShouldCollectDetailedDrawStats())
        {
            detailed_stats = StatsCollector::GetInstance().GetAllStatsAsString();
        }

        graphics::ColorBuffer& color_buffer = render_output.GetColorBuffer();
        graphics::TextOverlay::Render(overlay_stats, 10, 20, graphics::kRgb8ColorWhite, color_buffer);

        image::WriteColorBufferToPPM(color_buffer, "output.ppm");
        image::WriteColorBufferToPPM(checker_board, "checker_board.ppm");

        graphics::Image2D<graphics::Rgb8> checker_board_8_8 =
            graphics::ImageGenerator::GetCheckerBoard({.value = 16}, 8, 8);
        image::WriteColorBufferToPPM(checker_board_8_8, "checker_board_8_8.ppm");

        std::cout << "Color buffer saved to disk\n";

        std::cout << overlay_stats;
        std::cout << detailed_stats;
    }

    void RasterizeGraphicsPipeline(const graphics::Viewport& viewport)
    {
        auto make_vertex_in_local_space =
            [](const geometry::Point3D& position, const graphics::Rgb8& color, const graphics::UVCoordinate& uv) {
                return graphics::VertexAttributes<graphics::LocalSpace>{
                    .position = graphics::LocalSpace::Position{position}, .color = color, .uv = uv};
            };

        // Local-space geometry matching data/assets/models/textured_quad.obj.
        graphics::RenderPrimitiveTriangle<graphics::LocalSpace> local_space_triangle_1{
            make_vertex_in_local_space(geometry::Point3D{-0.5f, -0.5f, 0.0f}, graphics::kRgb8ColorRed, {0.0f, 0.0f}),
            make_vertex_in_local_space(geometry::Point3D{0.5f, -0.5f, 0.0f}, graphics::kRgb8ColorGreen, {1.0f, 0.0f}),
            make_vertex_in_local_space(geometry::Point3D{0.5f, 0.5f, 0.0f}, graphics::kRgb8ColorBlue, {1.0f, 1.0f})};

        graphics::RenderPrimitiveTriangle<graphics::LocalSpace> local_space_triangle_2{
            make_vertex_in_local_space(geometry::Point3D{-0.5f, -0.5f, 0.0f}, graphics::kRgb8ColorRed, {0.0f, 0.0f}),
            make_vertex_in_local_space(geometry::Point3D{0.5f, 0.5f, 0.0f}, graphics::kRgb8ColorBlue, {1.0f, 1.0f}),
            make_vertex_in_local_space(geometry::Point3D{-0.5f, 0.5f, 0.0f}, graphics::kRgb8ColorYellow, {0.0f, 1.0f})};

        std::cout << "Graphics pipeline started\n";

        graphics::ModelTransform model_transform{.translation            = maths::Vector3{0.0f, 0.0f, 0.0f},
                                                 .rotation_ruler_radians = maths::Vector3{0.0f, 0.0f, 0.0f},
                                                 .scale                  = maths::Vector3{1.0f, 1.0f, 1.0f}};

        graphics::TransformMatrices transform_matrices{
            .model_matrix                 = graphics::MakeModelMatrix(model_transform),
            .view_matrix                  = maths::Matrix4x4::GetIdentityMatrix(),
            .projection_matrix            = maths::Matrix4x4::GetIdentityMatrix(),
            .model_view_projection_matrix = maths::Matrix4x4::GetIdentityMatrix()};

        transform_matrices.model_view_projection_matrix =
            transform_matrices.projection_matrix * transform_matrices.view_matrix * transform_matrices.model_matrix;

        const graphics::RenderPrimitiveTriangle<graphics::ClipSpace> clip_space_triangle_1 =
            graphics::TransformLocalTriangleToClipSpace(local_space_triangle_1, transform_matrices);
        const graphics::RenderPrimitiveTriangle<graphics::ClipSpace> clip_space_triangle_2 =
            graphics::TransformLocalTriangleToClipSpace(local_space_triangle_2, transform_matrices);

        graphics::RenderFrame   render_frame(viewport);
        graphics::RenderOutput& render_output = render_frame.GetRenderOutput();

        graphics::Image2D<graphics::Rgb8> checker_board =
            graphics::ImageGenerator::GetCheckerBoard({.value = 200}, {.value = 200}, {.value = 8});
        graphics::Texture<graphics::Rgb8> checker_texture(checker_board);

        auto checkerboard_shader = [&render_output,
                                    &checker_texture](const render::cpu::RasterizedFragment& raster_fragment) {
            graphics::ImageCoordinate image_coordinate{raster_fragment.coordinate.x, raster_fragment.coordinate.y};

            if (raster_fragment.fragment_kind == render::cpu::RasterizedFragment::Kind::BoundingBox)
            {
                render_output.GetColorBuffer().SetImageData(image_coordinate, raster_fragment.color.ToRgb8());
                return;
            }

            render_output.GetColorBuffer().SetImageData(image_coordinate, checker_texture.Sample(raster_fragment.uv));
        };

        auto color_shader = [&render_output](const render::cpu::RasterizedFragment& raster_fragment) {
            graphics::ImageCoordinate image_coordinate{raster_fragment.coordinate.x, raster_fragment.coordinate.y};
            render_output.GetColorBuffer().SetImageData(image_coordinate, raster_fragment.color.ToRgb8());
        };

        graphics::DrawOptions draw_options(
            graphics::PrimitiveDrawMode::kSolid, graphics::DrawDebugFlag::kNone, graphics::StatsCollectionLevel::kDraw);

        render::cpu::Rasterizer triangle_rasterizer;
        triangle_rasterizer.Rasterize(render_frame.GetRenderConfig(),
                                      render_frame.GetRenderState(),
                                      render_frame.GetRenderFrameStats(),
                                      draw_options,
                                      clip_space_triangle_1,
                                      color_shader);

        triangle_rasterizer.Rasterize(render_frame.GetRenderConfig(),
                                      render_frame.GetRenderState(),
                                      render_frame.GetRenderFrameStats(),
                                      draw_options,
                                      clip_space_triangle_2,
                                      checkerboard_shader);

        std::cout << "Graphics pipeline finished\n";

        std::string overlay_stats = render_frame.GetRenderFrameStats().ToOverlayString();
        std::string detailed_stats;
        if (draw_options.ShouldCollectDetailedDrawStats())
        {
            detailed_stats = StatsCollector::GetInstance().GetAllStatsAsString();
        }

        graphics::ColorBuffer& color_buffer = render_output.GetColorBuffer();
        graphics::TextOverlay::Render(overlay_stats, 10, 20, graphics::kRgb8ColorWhite, color_buffer);

        image::WriteColorBufferToPPM(color_buffer, "output_graphics_pipeline.ppm");

        std::cout << "Graphics pipeline color buffer saved to disk\n";
        std::cout << overlay_stats;
        std::cout << detailed_stats;
    }

}  // namespace amit::cpu_rasterizer
