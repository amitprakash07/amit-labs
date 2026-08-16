#ifndef CORE_GRAPHICS_IMAGE_2D_H_
#define CORE_GRAPHICS_IMAGE_2D_H_

#include <algorithm>
#include <cstdint>

#include "core/graphics/pixel.h"

namespace amit::graphics
{
    using ImageCoordinate = amit::graphics::DiscreteCoordinate;

    template <typename ImageDataType>
    class Image2D
    {
    public:
        Image2D() = delete;

        Image2D(amit::graphics::Width width, amit::graphics::Height height);

        ~Image2D()
        {
            delete[] image_data_;
        }

        amit::graphics::Width GetWidth() const
        {
            return width_;
        }

        amit::graphics::Height GetHeight() const
        {
            return height_;
        }

        const ImageDataType& GetImageData(const ImageCoordinate& image_coordinate) const
        {
            if (image_coordinate.x >= width_.value || image_coordinate.y >= height_.value)
            {
                printf("It's wrong");
                static ImageDataType default_value{};
                return default_value;
            }

            uint64_t index = image_coordinate.y * width_.value + image_coordinate.x;
            return image_data_[index];
        }

        const ImageDataType* GetData() const
        {
            return image_data_;
        }

        void SetImageData(const ImageCoordinate& image_coordinate, const ImageDataType& data);

        void FillImage(ImageDataType image_data)
        {
            std::fill_n(
                image_data_, static_cast<size_t>(width_.value) * static_cast<size_t>(height_.value), image_data);
        }

        void FillTile(ImageDataType image_data, ImageCoordinate start, amit::graphics::TileSize tile_size)
        {
            if (start.x >= width_.value || start.y >= height_.value)
            {
                printf("It's wrong");
            }

            for (uint32_t y = start.y; y < start.y + tile_size.value && y < height_.value; ++y)
            {
                uint64_t index = y * width_.value + start.x;
                std::fill_n(image_data_ + index, static_cast<size_t>(tile_size.value), image_data);
            }
        }

    private:
        ImageDataType*         image_data_;  // Array to hold pixel colors by row-major order
        amit::graphics::Width  width_;
        amit::graphics::Height height_;
    };

    template <typename ImageDataType>
    amit::graphics::Image2D<ImageDataType>::Image2D(amit::graphics::Width width, amit::graphics::Height height)
        : image_data_(nullptr)
        , width_(width)
        , height_(height)
    {
        if (width_.value > 0 && height_.value > 0)
        {
            image_data_ = new ImageDataType[static_cast<size_t>(width_.value * height_.value)];
            memset(image_data_, 0, static_cast<size_t>(width_.value * height_.value) * sizeof(ImageDataType));
        }
    }

    template <typename ImageDataType>
    void amit::graphics::Image2D<ImageDataType>::SetImageData(const ImageCoordinate& image_coordinate,
                                                              const ImageDataType&   data)
    {
        if (image_coordinate.x >= width_.value || image_coordinate.y >= height_.value)
        {
            printf("It's wrong");
            return;
        }

        uint64_t index     = image_coordinate.y * width_.value + image_coordinate.x;
        image_data_[index] = data;
    }

    using ColorBuffer = Image2D<Rgb8>;
    using DepthBuffer = Image2D<float>;

}  // namespace amit::graphics

namespace amit::graphics
{
    void WriteColorBufferToPPM(const amit::graphics::ColorBuffer& color_buffer, const char* filename);
    void WriteDepthBufferToPPM(const amit::graphics::DepthBuffer& depth_buffer, const char* filename);
}  // namespace amit::graphics

#endif