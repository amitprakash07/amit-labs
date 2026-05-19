#ifndef CORE_GRAPHICS_TRANSFORM_H_
#define CORE_GRAPHICS_TRANSFORM_H_

#include <cstdint>
#include <utility>

#include "matrix_4x4.h"
#include "projection.h"
#include "vector4.h"
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

    struct TransformedVertexAttributes
    {
        VertexAttributes original;
        maths::Vector4   clip_space_position;
        maths::Vector3   ndc_space_position;
        PixelCoordinate  screen_space_position;
    };

    inline maths::Matrix4x4 MakeModelMatrix(const ModelTransform& transform)
    {
        // TODO: Implement this function.
        return maths::Matrix4x4::GetIdentityMatrix();
    }

    inline maths::Matrix4x4 MakeViewMatrix(const CameraView& camera_view)
    {
        // TODO: Implement this function.
        return maths::Matrix4x4::GetIdentityMatrix();
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

    inline TransformedVertexAttributes TransformVertexAttributes(const VertexAttributes&  attributes,
                                                                 const TransformMatrices& matrices)
    {
        // TODO: Implement this function.
        return TransformedVertexAttributes{};
    }

}  // namespace amit::graphics

#endif  // CORE_GRAPHICS_TRANSFORM_H_