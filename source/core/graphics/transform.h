#ifndef CORE_GRAPHICS_TRANSFORM_H_
#define CORE_GRAPHICS_TRANSFORM_H_

#include <limits>

#include "matrix_4x4.h"
#include "projection.h"
#include "vector4.h"
#include "viewport.h"
#include "core/graphics_common/graphics_common.h"

namespace amit::graphics
{
    struct ModelTransform
    {
        maths::Vector3 translation;
        maths::Vector3 rotation_ruler_radians;
        maths::Vector3 scale;
    };

    struct CameraView
    {
        maths::Vector3 position;
        maths::Vector3 look_at;
        maths::Vector3 up;
    };

    struct TransformMatrices
    {
        maths::Matrix4x4 model_matrix;
        maths::Matrix4x4 view_matrix;
        maths::Matrix4x4 projection_matrix;
        maths::Matrix4x4 model_view_projection_matrix;
    };

    inline maths::Matrix4x4 MakeModelMatrix(const ModelTransform& /*transform*/)
    {
        return maths::Matrix4x4::GetIdentityMatrix();
    }

    inline maths::Matrix4x4 MakeViewMatrix(const CameraView& camera_view)
    {
        return maths::Matrix4x4::CreateLookAtMatrix(camera_view.position, camera_view.look_at, camera_view.up);
    }

    inline TransformMatrices MakeTransformMatrices(const ModelTransform&     model_transform,
                                                   const CameraView&         camera_view,
                                                   const PerspectiveFrustum& perspective_frustum)
    {
        TransformMatrices result;

        result.model_matrix                 = MakeModelMatrix(model_transform);
        result.view_matrix                  = MakeViewMatrix(camera_view);
        result.projection_matrix            = MakePerspectiveLH(perspective_frustum);
        result.model_view_projection_matrix = result.projection_matrix * result.view_matrix * result.model_matrix;
        return result;
    }

    inline graphics::VertexAttributes<graphics::ClipSpace> TransformLocalVertexToClipSpace(
        const graphics::VertexAttributes<graphics::LocalSpace>& local_vertex,
        const graphics::TransformMatrices&                      transform_matrices)
    {
        const graphics::LocalSpace::Position& local_position = local_vertex.position;
        const maths::Vector4                  clip_position  = transform_matrices.model_view_projection_matrix.Mul(
            maths::Vector4{local_position.x, local_position.y, local_position.z, 1.0f});

        return graphics::VertexAttributes<graphics::ClipSpace>{.position = graphics::ClipSpace::Position{clip_position},
                                                               .color    = local_vertex.color,
                                                               .uv       = local_vertex.uv};
    }

    inline graphics::RenderPrimitiveTriangle<graphics::ClipSpace> TransformLocalTriangleToClipSpace(
        const graphics::RenderPrimitiveTriangle<graphics::LocalSpace>& local_triangle,
        const graphics::TransformMatrices&                             transform_matrices)
    {
        return {TransformLocalVertexToClipSpace(local_triangle.VertA(), transform_matrices),
                TransformLocalVertexToClipSpace(local_triangle.VertB(), transform_matrices),
                TransformLocalVertexToClipSpace(local_triangle.VertC(), transform_matrices)};
    }

    inline maths::Vector3 TransformClipSpaceToNdcSpace(const maths::Vector4& attribute)
    {
        const float* const clip_components = attribute.AsFloatArray();
        const float        clip_w          = clip_components[3];

        maths::Vector3 ndc_space_position{};
        if (std::abs(clip_w) > std::numeric_limits<float>::epsilon())
        {
            const float inv_clip_w = 1.0f / clip_w;
            ndc_space_position     = maths::Vector3{
                clip_components[0] * inv_clip_w, clip_components[1] * inv_clip_w, clip_components[2] * inv_clip_w};
        }

        return ndc_space_position;
    }

    inline maths::Vector3 TransformNdcSpaceToScreenSpace(const Viewport& viewport, const maths::Vector3& ndc_position)
    {
        const float x_screen_space =
            viewport.Origin().x + (ndc_position.x() + 1.0f) * 0.5f * static_cast<float>(viewport.GetWidth().value);
        const float y_screen_space =
            viewport.Origin().y + (1.0f - ndc_position.y()) * 0.5f * static_cast<float>(viewport.GetHeight().value);

        return maths::Vector3{
            x_screen_space,
            y_screen_space,
            ndc_position.z()  // Preserve depth for depth testing in screen space.
        };
    }

}  // namespace amit::graphics

#endif  // CORE_GRAPHICS_TRANSFORM_H_