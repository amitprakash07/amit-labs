#ifndef CORE_GRAPHICS_TRANSFORM_H_
#define CORE_GRAPHICS_TRANSFORM_H_

#include "matrix_4x4.h"
#include "projection.h"
#include "vector4.h"
#include "viewport.h"
#include "core/graphics_common/graphics_common.h"
#include "core/graphics_common/render_primitives.h"

namespace amit::maths
{
    class Quaternion;
}

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

        std::string ToString() const
        {
            return std::format("Model Matrix:\n{}\nView Matrix:\n{}\nProjection Matrix:\n{}\nMVP Matrix:\n{}",
                               model_matrix.ToString(),
                               view_matrix.ToString(),
                               projection_matrix.ToString(),
                               model_view_projection_matrix.ToString());
        }
    };

    maths::Matrix4x4 MakeModelMatrix(const ModelTransform& transform);

    maths::Matrix4x4 MakeScaleMatrix(float x_scale, float y_scale, float z_scale);

    maths::Matrix4x4 MakeScaleMatrix(const maths::Vector3& scale_factor);

    maths::Matrix4x4 MakeWorldToViewTransform(const maths::Quaternion& camera_orientation,
                                              const maths::Vector3&    camera_position);

    maths::Matrix4x4 MakeLookAtMatrix(const maths::Vector3& eye,
                                      const maths::Vector3& target,
                                      const maths::Vector3& up_vector);

    maths::Matrix4x4 MakeViewMatrix(const CameraView& camera_view);

    TransformMatrices MakeTransformMatrices(const ModelTransform&     model_transform,
                                            const CameraView&         camera_view,
                                            const PerspectiveFrustum& perspective_frustum);

    VertexAttributes<ClipSpace> TransformLocalVertexToClipSpace(
        const VertexAttributes<LocalSpace>& local_vertex,
        const TransformMatrices&                      transform_matrices);

    RenderPrimitiveTriangle<ClipSpace> TransformLocalTriangleToClipSpace(
        const RenderPrimitiveTriangle<LocalSpace>& local_triangle,
        const TransformMatrices&                             transform_matrices);

    RenderPrimitiveLine<ClipSpace> TransformLocalLineToClipSpace(
        const RenderPrimitiveLine<LocalSpace>& local_line,
        const TransformMatrices&                         transform_matrices);

    maths::Vector3 TransformClipSpaceToNdcSpace(const maths::Vector4& attribute);

    maths::Vector3 TransformNdcSpaceToScreenSpace(const Viewport& viewport, const maths::Vector3& ndc_position);

}  // namespace amit::graphics

#endif  // CORE_GRAPHICS_TRANSFORM_H_