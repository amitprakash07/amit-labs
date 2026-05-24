#ifndef CORE_GRAPHICS_VIEWPORT_H_
#define CORE_GRAPHICS_VIEWPORT_H_

#include <cstdint>
#include <utility>

#include "core/graphics_common/graphics_common.h"
#include "core/maths/geometry/point.h"
#include "vector3.h"

namespace amit::graphics
{
    class Viewport
    {
    public:
        Viewport() = delete;

        Viewport(geometry::Point3D origin, Width width, Height height)
            : origin_(origin)
            , width_(width)
            , height_(height)
        {
        }

        Viewport(Width width, Height height)
            : origin_()
            , width_(width)
            , height_(height)
        {
        }

        Width GetWidth() const
        {
            return width_;
        }

        Height GetHeight() const
        {
            return height_;
        }

        const geometry::Point3D& Origin() const
        {
            return origin_;
        }

        uint32_t ClampX(int32_t x) const
        {
            if (std::cmp_less(x, 0))
            {
                return 0;
            }

            if (std::cmp_greater(x, width_.value))
            {
                return width_.value;
            }

            return static_cast<uint32_t>(x);
        }

        uint32_t ClampY(int32_t y) const
        {
            if (std::cmp_less(y, 0))
            {
                return 0;
            }

            if (std::cmp_greater(y, height_.value))
            {
                return height_.value;
            }

            return static_cast<uint32_t>(y);
        }

        geometry::Point3D ClampToView(const geometry::Point3D& point) const
        {
            return geometry::Point3D{static_cast<float>(ClampX(static_cast<int32_t>(point.x))),
                                     static_cast<float>(ClampY(static_cast<int32_t>(point.y))),
                                     point.z};
        }

    private:
        geometry::Point3D origin_;
        Width             width_;
        Height            height_;
    };
}  // namespace amit::graphics

#endif  // CORE_GRAPHICS_VIEWPORT_H_