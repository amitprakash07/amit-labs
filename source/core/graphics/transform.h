#ifndef CORE_GRAPHICS_TRANSFORM_H_
#define CORE_GRAPHICS_TRANSFORM_H_

#include <cmath>
#include <cstdint>
#include <limits>
#include <utility>

#include "matrix_4x4.h"
#include "projection.h"
#include "vector4.h"
#include "viewport.h"
#include "core/graphics_common/graphics_common.h"
#include "core/graphics_common/render_primitives.h"

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

    inline PixelCoordinate TransformNdcSpaceToScreenSpace(const Viewport& viewport, const maths::Vector3& ndc_position)
    {
        const float pixel_x =
            viewport.Origin().x + (ndc_position.x() + 1.0f) * 0.5f * static_cast<float>(viewport.GetWidth().value);
        const float pixel_y =
            viewport.Origin().y + (1.0f - ndc_position.y()) * 0.5f * static_cast<float>(viewport.GetHeight().value);

        return PixelCoordinate{
            .x = static_cast<std::uint32_t>(pixel_x),
            .y = static_cast<std::uint32_t>(pixel_y),
        };
    }

}  // namespace amit::graphics

#endif  // CORE_GRAPHICS_TRANSFORM_H_