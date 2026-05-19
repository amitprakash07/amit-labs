#ifndef CORE_GRAPHICS_PROJECTION_H_
#define CORE_GRAPHICS_PROJECTION_H_

#include <cstdint>
#include <utility>

#include "matrix_4x4.h"
#include "utils.h"
#include "viewport.h"
#include "core/graphics_common/graphics_common.h"
#include "core/maths/geometry/point.h"

namespace amit::graphics
{
    struct PerspectiveFrustum
    {
        float fov_in_radians;
        float aspect_ratio;
        float near_plane;
        float far_plane;
    };

    inline PerspectiveFrustum kDefaultPerspectiveFrustum{.fov_in_radians = maths::ToRadians(60.0f),
                                                         .aspect_ratio   = 16.0f / 9.0f,
                                                         .near_plane     = 0.1f,
                                                         .far_plane      = 100.0f};

    inline maths::Matrix4x4 MakePerspectiveLH(const PerspectiveFrustum& frustum = kDefaultPerspectiveFrustum)
    {
        // TODO: Implement this function. For now, we just return the identity matrix.
        return maths::Matrix4x4::GetIdentityMatrix();
    }
}  // namespace amit::graphics

#endif