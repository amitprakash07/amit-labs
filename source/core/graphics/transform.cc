#include "transform.h"

#include <limits>

#include "quaternion.h"

namespace amit::graphics
{
    maths::Matrix4x4 MakeModelMatrix(const ModelTransform& transform)
    {
        const maths::Matrix4x4 scale_matrix       = MakeScaleMatrix(transform.scale);
        const maths::Matrix4x4 translation_matrix = maths::Matrix4x4{transform.translation};

        // Matrix4x4 uses row-vector pre-multiply: local * scale * translation.
        return scale_matrix * translation_matrix;
    }

    maths::Matrix4x4 MakeScaleMatrix(float x_scale, float y_scale, float z_scale)
    {
        return maths::Matrix4x4{
            x_scale, 0.0f, 0.0f, 0.0f, 0.0f, y_scale, 0.0f, 0.0f, 0.0f, 0.0f, z_scale, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    }

    maths::Matrix4x4 MakeScaleMatrix(const maths::Vector3& scale_factor)
    {
        return MakeScaleMatrix(scale_factor.x(), scale_factor.y(), scale_factor.z());
    }

    maths::Matrix4x4 MakeWorldToViewTransform(const maths::Quaternion& camera_orientation,
                                              const maths::Vector3&    camera_position)
    {
        maths::Matrix4x4 camera_to_world{camera_orientation, camera_position};
        return camera_to_world.GetInverse();
    }

    maths::Matrix4x4 MakeLookAtMatrix(const maths::Vector3& eye,
                                      const maths::Vector3& target,
                                      const maths::Vector3& up_vector)
    {
        const maths::Vector3 forward     = (target - eye).CreateNormalized();
        const maths::Vector3 left        = up_vector.Cross(forward).CreateNormalized();
        const maths::Vector3 up          = forward.Cross(left);
        const maths::Vector3 translation = maths::Vector3{-left.Dot(eye), -up.Dot(eye), -forward.Dot(eye)};

        // Row-vector layout: translation lives in m_30, m_31, m_32 (bottom row).
        return maths::Matrix4x4{left.x(),
                                left.y(),
                                left.z(),
                                translation.x(),
                                up.x(),
                                up.y(),
                                up.z(),
                                translation.y(),
                                forward.x(),
                                forward.y(),
                                forward.z(),
                                translation.z(),
                                0.0f,
                                0.0f,
                                0.0f,
                                1.0f};
    }

    maths::Matrix4x4 MakeViewMatrix(const CameraView& camera_view)
    {
        return MakeLookAtMatrix(camera_view.position, camera_view.look_at, camera_view.up);
    }

    TransformMatrices MakeTransformMatrices(const ModelTransform&     model_transform,
                                            const CameraView&         camera_view,
                                            const PerspectiveFrustum& perspective_frustum)
    {
        TransformMatrices result;

        result.model_matrix      = MakeModelMatrix(model_transform);
        result.view_matrix       = MakeViewMatrix(camera_view);
        result.projection_matrix = MakePerspectiveLH(perspective_frustum);

        // Matrix4x4 uses row-vector pre-multiply: clip = local * model * view * projection.
        result.model_view_projection_matrix = result.model_matrix * result.view_matrix * result.projection_matrix;
        return result;
    }

    graphics::VertexAttributes<graphics::ClipSpace> TransformLocalVertexToClipSpace(
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

    graphics::RenderPrimitiveTriangle<graphics::ClipSpace> TransformLocalTriangleToClipSpace(
        const graphics::RenderPrimitiveTriangle<graphics::LocalSpace>& local_triangle,
        const graphics::TransformMatrices&                             transform_matrices)
    {
        return {TransformLocalVertexToClipSpace(local_triangle.VertA(), transform_matrices),
                TransformLocalVertexToClipSpace(local_triangle.VertB(), transform_matrices),
                TransformLocalVertexToClipSpace(local_triangle.VertC(), transform_matrices)};
    }

    graphics::RenderPrimitiveLine<graphics::ClipSpace> TransformLocalLineToClipSpace(
        const graphics::RenderPrimitiveLine<graphics::LocalSpace>& local_line,
        const graphics::TransformMatrices&                         transform_matrices)
    {
        return {
            TransformLocalVertexToClipSpace(local_line.Start(), transform_matrices),
            TransformLocalVertexToClipSpace(local_line.End(), transform_matrices),
        };
    }

    maths::Vector3 TransformClipSpaceToNdcSpace(const maths::Vector4& attribute)
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

    maths::Vector3 TransformNdcSpaceToScreenSpace(const Viewport& viewport, const maths::Vector3& ndc_position)
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
