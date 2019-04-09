//
// Created by Simon on 2019-04-02.
//

#include "film.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

#include <cassert>
#include <sstream>

namespace Rabbit
{

Film::Film(unsigned int width, unsigned int height)
    : width{ width }, height{ height }, raster{ width * height }
{}

const Spectrumf& Film::operator()(unsigned int pixel_x, unsigned int pixel_y) const noexcept
{
    assert(pixel_y < Height() && pixel_x < Width());
    return raster[pixel_y * Width() + pixel_x];
}

Spectrumf& Film::operator()(unsigned int pixel_x, unsigned int pixel_y) noexcept
{
    assert(pixel_y < Height() && pixel_x < Width());
    return raster[pixel_y * Width() + pixel_x];
}

void Film::WritePNG(const std::string& filename) const
{
    std::vector<unsigned char> uchar_raster(Width() * Height() * 3, 0);
    for (unsigned int i = 0; i != Width() * Height(); i++)
    {
        const Spectrumf color{ raster[i].Clamp(0.f, 1.f) };
        uchar_raster[3 * i] = static_cast<unsigned char>(255 * color.r);
        uchar_raster[3 * i + 1] = static_cast<unsigned char>(255 * color.g);
        uchar_raster[3 * i + 2] = static_cast<unsigned char>(255 * color.b);
    }

    // Write image
    stbi_flip_vertically_on_write(1);
    if (!stbi_write_png(filename.c_str(), Width(), Height(), 3, uchar_raster.data(), 0))
    {
        std::ostringstream error_string;
        error_string << "Error writing PNG image " << filename << "\n";
        throw std::runtime_error(error_string.str());
    }
    stbi_flip_vertically_on_write(0);
}

} // Rabbit namespace
