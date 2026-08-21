
#include <filesystem>
#include <iostream>
#include <string>

#include "cpu_rasterizer_pipeline.h"

#include "core/common/settings_registry.h"
#include "core/common/perf_stats.h"
#include "core/sdl_windowing/color_buffer_window.h"
#include "core/sdl_windowing/output_path.h"
#include "core/cpu_renderer/rasterizer.h"
#include "core/graphics/image_2d.h"
#include "core/graphics/image_generator.h"
#include "core/graphics/render_context.h"
#include "core/graphics/texture.h"
#include "core/graphics/text_overlay.h"
#include "core/graphics/transform.h"
#include "core/maths/geometry/point.h"

#include "core/graphics_asset/obj_adapter.h"
#include "core/graphics_common/mesh_data.h"

using namespace amit;

namespace amit::cpu_rasterizer
{
    void DemoTriangleRasterizeInScreenSpace(const graphics::Viewport& viewport)
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
            detailed_stats = common::StatsCollector::GetInstance().GetAllStatsAsString();
        }

        graphics::ColorBuffer& color_buffer = render_output.GetColorBuffer();
        graphics::TextOverlay::Render(overlay_stats, 10, 20, graphics::kRgb8ColorWhite, color_buffer);

        graphics::WriteColorBufferToPPM(color_buffer, "output.ppm");
        graphics::WriteColorBufferToPPM(checker_board, "checker_board.ppm");

        std::cout << "Color buffer saved to disk\n";
        sdl_windowing::ShowColorBufferWindow(color_buffer, "CPU Rasterizer - Screen Space Triangles");

        std::cout << overlay_stats;
        std::cout << detailed_stats;
    }

    void DemoTriangleRasterizeGraphicsPipeline(const graphics::Viewport& viewport)
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

        const graphics::CameraView camera_view{.position = maths::Vector3{0.0f, 0.0f, -10.0f},
                                               .look_at  = maths::Vector3{0.0f, 0.0f, 0.0f},
                                               .up       = maths::Vector3{0.0f, 1.0f, 0.0f}};

        const graphics::PerspectiveFrustum perspective_frustum{
            .fov_in_radians = maths::ToRadians(60.0f),
            .aspect_ratio =
                static_cast<float>(viewport.GetWidth().value) / static_cast<float>(viewport.GetHeight().value),
            .near_plane = 0.1f,
            .far_plane  = 100.0f};

        std::cout << "Frustum\n" << perspective_frustum.ToString() << "\n";

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

        {
            render::cpu::Rasterizer        triangle_rasterizer;
            const graphics::ModelTransform model_transform{.translation            = maths::Vector3{0.0f, 0.0f, 0.0f},
                                                           .rotation_ruler_radians = maths::Vector3{0.0f, 0.0f, 0.0f},
                                                           .scale                  = maths::Vector3{5.0f, 5.0f, 1.0f}};

            const graphics::TransformMatrices transform_matrices =
                graphics::MakeTransformMatrices(model_transform, camera_view, perspective_frustum);

            const graphics::RenderPrimitiveTriangle<graphics::ClipSpace> clip_space_triangle_1 =
                graphics::TransformLocalTriangleToClipSpace(local_space_triangle_1, transform_matrices);

            triangle_rasterizer.Rasterize(render_frame.GetRenderConfig(),
                                          render_frame.GetRenderState(),
                                          render_frame.GetRenderFrameStats(),
                                          draw_options,
                                          clip_space_triangle_1,
                                          color_shader);
        }

        {
            render::cpu::Rasterizer        triangle_rasterizer;
            const graphics::ModelTransform model_transform{.translation            = maths::Vector3{1.0f, 0.0f, -0.5f},
                                                           .rotation_ruler_radians = maths::Vector3{0.0f, 0.0f, 0.0f},
                                                           .scale                  = maths::Vector3{5.0f, 5.0f, 1.0f}};

            const graphics::TransformMatrices transform_matrices =
                graphics::MakeTransformMatrices(model_transform, camera_view, perspective_frustum);

            const graphics::RenderPrimitiveTriangle<graphics::ClipSpace> clip_space_triangle_2 =
                graphics::TransformLocalTriangleToClipSpace(local_space_triangle_2, transform_matrices);

            triangle_rasterizer.Rasterize(render_frame.GetRenderConfig(),
                                          render_frame.GetRenderState(),
                                          render_frame.GetRenderFrameStats(),
                                          draw_options,
                                          clip_space_triangle_2,
                                          checkerboard_shader);
        }

        std::cout << "Graphics pipeline finished\n";

        std::string overlay_stats = render_frame.GetRenderFrameStats().ToOverlayString();
        std::string detailed_stats;
        if (draw_options.ShouldCollectDetailedDrawStats())
        {
            detailed_stats = common::StatsCollector::GetInstance().GetAllStatsAsString();
        }

        graphics::ColorBuffer& color_buffer = render_output.GetColorBuffer();
        graphics::TextOverlay::Render(overlay_stats, 10, 20, graphics::kRgb8ColorWhite, color_buffer);

        graphics::WriteColorBufferToPPM(color_buffer, "output_graphics_pipeline.ppm");

        std::cout << "Graphics pipeline color buffer saved to disk\n";
        sdl_windowing::ShowColorBufferWindow(color_buffer, "CPU Rasterizer - Graphics Pipeline");
        std::cout << overlay_stats;
        std::cout << detailed_stats;
    }

    void DemoQuadObjModelSceneRasterizeGraphicsPipeline(const graphics::Viewport& viewport)
    {
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

        graphics::DrawOptions draw_options(
            graphics::PrimitiveDrawMode::kSolid, graphics::DrawDebugFlag::kNone, graphics::StatsCollectionLevel::kDraw);

        const graphics::CameraView camera_view{.position = maths::Vector3{0.0f, 0.0f, -10.0f},
                                               .look_at  = maths::Vector3{0.0f, 0.0f, 0.0f},
                                               .up       = maths::Vector3{0.0f, 1.0f, 0.0f}};

        const graphics::PerspectiveFrustum perspective_frustum{
            .fov_in_radians = maths::ToRadians(60.0f),
            .aspect_ratio =
                static_cast<float>(viewport.GetWidth().value) / static_cast<float>(viewport.GetHeight().value),
            .near_plane = 0.1f,
            .far_plane  = 100.0f};

        const std::filesystem::path texture_quad_obj = std::string(ASSET_DIR "/assets/models/textured_quad.obj");

        assets::ObjAdapter                           adapter;
        const std::optional<assets::ImportHandle> import_handle = adapter.ImportObj(texture_quad_obj);
        if (!import_handle.has_value())
        {
            std::cerr << "Failed to load OBJ mesh: " << texture_quad_obj.string() << '\n';
            return;
        }

        const graphics::MeshData& render_mesh = adapter.GetRenderMeshData(import_handle.value());

        std::vector<graphics::RenderPrimitiveTriangle<graphics::LocalSpace>> render_primitives =
            graphics::ConvertMeshDataToRenderPrimitives(render_mesh);

        const graphics::ModelTransform model_transform{.translation            = maths::Vector3{0.0f, 0.0f, 0.0f},
                                                       .rotation_ruler_radians = maths::Vector3{0.0f, 0.0f, 0.0f},
                                                       .scale                  = maths::Vector3{5.0f, 5.0f, 1.0f}};

        const graphics::TransformMatrices transform_matrices =
            graphics::MakeTransformMatrices(model_transform, camera_view, perspective_frustum);

        for (const graphics::RenderPrimitiveTriangle<graphics::LocalSpace>& local_space_triangle : render_primitives)
        {
            render::cpu::Rasterizer triangle_rasterizer;

            const graphics::RenderPrimitiveTriangle<graphics::ClipSpace> clip_space_triangle =
                graphics::TransformLocalTriangleToClipSpace(local_space_triangle, transform_matrices);

            triangle_rasterizer.Rasterize(render_frame.GetRenderConfig(),
                                          render_frame.GetRenderState(),
                                          render_frame.GetRenderFrameStats(),
                                          draw_options,
                                          clip_space_triangle,
                                          checkerboard_shader);
        }

        std::string overlay_stats = render_frame.GetRenderFrameStats().ToOverlayString();

        graphics::ColorBuffer& color_buffer = render_output.GetColorBuffer();
        graphics::TextOverlay::Render(overlay_stats, 10, 20, graphics::kRgb8ColorWhite, color_buffer);

        graphics::WriteColorBufferToPPM(color_buffer, "obj_quad_scene_output.ppm");

        std::cout << "Color buffer saved to disk\n";
        sdl_windowing::ShowColorBufferWindow(color_buffer, "CPU Rasterizer - Textured Quad OBJ");

        if (draw_options.ShouldCollectDetailedDrawStats())
        {
            std::cout << common::StatsCollector::GetInstance().GetAllStatsAsString() << '\n';
        }
    }

    void DemoCornellBoxObjModelSceneRasterizeGraphicsPipeline(const graphics::Viewport& viewport)
    {
        graphics::RenderFrame   render_frame(viewport);
        graphics::RenderOutput& render_output = render_frame.GetRenderOutput();

        auto color_shader = [&render_output](const render::cpu::RasterizedFragment& raster_fragment) {
            graphics::ImageCoordinate image_coordinate{raster_fragment.coordinate.x, raster_fragment.coordinate.y};
            render_output.GetColorBuffer().SetImageData(image_coordinate, raster_fragment.color.ToRgb8());
        };

        graphics::DrawOptions draw_options(
            graphics::PrimitiveDrawMode::kSolid, graphics::DrawDebugFlag::kNone, graphics::StatsCollectionLevel::kDraw);

        graphics::CameraView camera_view{.position = maths::Vector3{0.0f, 1.5f, 3.0f},
                                         .look_at  = maths::Vector3{0.0f, 1.0f, 0.0f},
                                         .up       = maths::Vector3{0.0f, 1.0f, 0.0f}};

        const graphics::PerspectiveFrustum perspective_frustum{
            .fov_in_radians = maths::ToRadians(60.0f),
            .aspect_ratio =
                static_cast<float>(viewport.GetWidth().value) / static_cast<float>(viewport.GetHeight().value),
            .near_plane = 0.1f,
            .far_plane  = 100.0f};

        const std::filesystem::path cornell_box_obj =
            std::string(ASSET_DIR "/assets/models/CornellBox-Original/CornellBox-Original.obj");

        assets::ObjAdapter                        adapter;
        const std::optional<assets::ImportHandle> import_handle = adapter.ImportObj(cornell_box_obj);
        if (!import_handle.has_value())
        {
            std::cerr << "Failed to load OBJ mesh: " << cornell_box_obj.string() << '\n';
            return;
        }

        const graphics::MeshData& render_mesh = adapter.GetRenderMeshData(import_handle.value());

        const std::vector<graphics::RenderPrimitiveTriangle<graphics::LocalSpace>> render_primitives =
            graphics::ConvertMeshDataToRenderPrimitives(render_mesh);

        const graphics::ModelTransform model_transform{.translation            = maths::Vector3{0.0f, 0.0f, 0.0f},
                                                       .rotation_ruler_radians = maths::Vector3{0.0f, 0.0f, 0.0f},
                                                       .scale                  = maths::Vector3{1.0f, 1.0f, 1.0f}};

        graphics::ColorBuffer& color_buffer = render_output.GetColorBuffer();

        float                          move_speed = 2.5f;
        std::string                    frame_stats_text;
        std::string                    save_status_text;
        bool                           request_save_ppm = false;
        app_settings::SettingsRegistry settings_registry;
        settings_registry.AddFloat("Move Speed", &move_speed, 0.1f, 20.0f);

        constexpr int kExportStatsPanelWidth = 320;

        auto rasterize_scene = [&]() {
            render_frame.GetRenderState().Clear();
            render_frame.GetRenderFrameStats().Reset();
            color_buffer.FillImage(graphics::kRgb8ColorBlack);

            const graphics::TransformMatrices transform_matrices =
                graphics::MakeTransformMatrices(model_transform, camera_view, perspective_frustum);

            for (const graphics::RenderPrimitiveTriangle<graphics::LocalSpace>& local_space_triangle :
                 render_primitives)
            {
                render::cpu::Rasterizer triangle_rasterizer;

                const graphics::RenderPrimitiveTriangle<graphics::ClipSpace> clip_space_triangle =
                    graphics::TransformLocalTriangleToClipSpace(local_space_triangle, transform_matrices);

                triangle_rasterizer.Rasterize(render_frame.GetRenderConfig(),
                                              render_frame.GetRenderState(),
                                              render_frame.GetRenderFrameStats(),
                                              draw_options,
                                              clip_space_triangle,
                                              color_shader);
            }

            frame_stats_text = render_frame.GetRenderFrameStats().ToOverlayString();
        };

        auto save_color_buffer_with_stats_panel = [&]() {
            const int scene_width  = static_cast<int>(color_buffer.GetWidth().value);
            const int scene_height = static_cast<int>(color_buffer.GetHeight().value);

            graphics::ColorBuffer export_buffer({.value = static_cast<std::uint32_t>(scene_width + kExportStatsPanelWidth)},
                                                {.value = static_cast<std::uint32_t>(scene_height)});
            export_buffer.FillImage(graphics::kRgb8ColorBlack);

            for (int y = 0; y < scene_height; ++y)
            {
                for (int x = 0; x < scene_width; ++x)
                {
                    export_buffer.SetImageData(
                        graphics::ImageCoordinate{static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(y)},
                        color_buffer.GetImageData(
                            graphics::ImageCoordinate{static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(y)}));
                }
            }

            graphics::TextOverlay::Render(frame_stats_text,
                                          static_cast<std::uint16_t>(scene_width + 10),
                                          20,
                                          graphics::kRgb8ColorWhite,
                                          export_buffer);

            const std::string ppm_path = sdl_windowing::MakeOutputFilePath("obj_cornell_box_scene_output.ppm");
            graphics::WriteColorBufferToPPM(export_buffer, ppm_path.c_str());
            save_status_text = std::string("Saved ") + ppm_path + " (scene + stats panel)";
            std::cout << save_status_text << '\n';
        };

        auto update_and_render = [&](const sdl_windowing::InteractiveInput& input) {
            const float step = move_speed * input.dt_seconds;

            const maths::Vector3 forward = (camera_view.look_at - camera_view.position).CreateNormalized();
            const maths::Vector3 right   = forward.Cross(camera_view.up).CreateNormalized();
            const maths::Vector3 up      = camera_view.up.CreateNormalized();

            maths::Vector3 delta{0.0f, 0.0f, 0.0f};
            if (input.move_forward)
            {
                delta += forward * step;
            }
            if (input.move_backward)
            {
                delta -= forward * step;
            }
            if (input.move_right)
            {
                delta += right * step;
            }
            if (input.move_left)
            {
                delta -= right * step;
            }
            if (input.move_up)
            {
                delta += up * step;
            }
            if (input.move_down)
            {
                delta -= up * step;
            }

            camera_view.position += delta;
            camera_view.look_at += delta;

            rasterize_scene();

            if (request_save_ppm)
            {
                save_color_buffer_with_stats_panel();
                request_save_ppm = false;
            }

            return true;
        };

        // Output panel shows live stats; append last save message when present.
        std::string output_panel_text;
        auto        refresh_output_text = [&]() {
            output_panel_text = frame_stats_text;
            if (!save_status_text.empty())
            {
                output_panel_text += "\n\n";
                output_panel_text += save_status_text;
            }
        };

        auto update_and_render_with_output = [&](const sdl_windowing::InteractiveInput& input) {
            const bool ok = update_and_render(input);
            refresh_output_text();
            return ok;
        };

        sdl_windowing::InteractiveUiState ui_state{.settings_registry = &settings_registry,
                                                   .output_text       = &output_panel_text,
                                                   .request_save_ppm  = &request_save_ppm,
                                                   .action_status     = &save_status_text,
                                                   .sidebar_width_px  = 320};

        sdl_windowing::RunInteractiveColorBufferLoop(
            color_buffer, "CPU Rasterizer - Cornell Box OBJ", update_and_render_with_output, &ui_state);

        if (draw_options.ShouldCollectDetailedDrawStats())
        {
            std::cout << common::StatsCollector::GetInstance().GetAllStatsAsString() << '\n';
        }
    }
}  // namespace amit::cpu_rasterizer
