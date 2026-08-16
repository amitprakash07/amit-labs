#include <cstring>

#include "image_2d.h"

void amit::graphics::WriteColorBufferToPPM(const amit::graphics::ColorBuffer& color_buffer, const char* filename)
{
    FILE*  file     = fopen(filename, "wb");
    int    result   = fprintf(file,
                         "P6\n%d %d\n255\n",
                         static_cast<int>(color_buffer.GetWidth().value),
                         static_cast<int>(color_buffer.GetHeight().value));
    size_t result_2 = fwrite(color_buffer.GetData(),
                             sizeof(amit::graphics::Rgb8),
                             static_cast<size_t>(color_buffer.GetWidth().value) * color_buffer.GetHeight().value,
                             file);
    result          = fclose(file);
    result;
    result_2;
}

void amit::graphics::WriteDepthBufferToPPM(const amit::graphics::DepthBuffer& depth_buffer, const char* filename)
{
    FILE*  file     = fopen(filename, "wb");
    int    result   = fprintf(file,
                         "P6\n%d %d\n255\n",
                         static_cast<int>(depth_buffer.GetWidth().value),
                         static_cast<int>(depth_buffer.GetHeight().value));
    size_t result_2 = fwrite(depth_buffer.GetData(),
                             sizeof(amit::graphics::Rgb8),
                             static_cast<size_t>(depth_buffer.GetWidth().value) * depth_buffer.GetHeight().value,
                             file);
    result          = fclose(file);
    result;
    result_2;
}