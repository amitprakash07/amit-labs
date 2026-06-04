#include "projection.h"

#include <cmath>

namespace amit::graphics
{
    maths::Matrix4x4 MakePerspectiveLH(const PerspectiveFrustum& frustum)
    {
        const float y_scale = 1.0f / std::tan(frustum.fov_in_radians * 0.5f);
        const float x_scale = y_scale / frustum.aspect_ratio;

        const float z_distance_scale = frustum.far_plane / (frustum.far_plane - frustum.near_plane);
        // D3D LH row-vector layout: w_clip = z_view, z_clip = z_view * A + B.
        const float z_offset = -frustum.near_plane * z_distance_scale;
        return maths::Matrix4x4{x_scale,
                                0.0f,
                                0.0f,
                                0.0f,
                                0.0f,
                                y_scale,
                                0.0f,
                                0.0f,
                                0.0f,
                                0.0f,
                                z_distance_scale,
                                z_offset,
                                0.0f,
                                0.0f,
                                1.0f,
                                0.0f};
    }
}  // namespace amit::graphics
