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

const Spectrumf& Film::operator()(unsigned int row, unsigned int column) const noexcept
{
    assert(row < height && column < width);
    return raster[row * width + column];
}

Spectrumf& Film::operator()(unsigned int row, unsigned int column) noexcept
{
    assert(row < height && column < width);
    return raster[row * width + column];
}

void Film::WritePNG(const std::string& filename) const
{
    std::vector<unsigned char> uchar_raster(width * height * 3, 0);
    for (unsigned int i = 0; i != width * height; i++)
    {
        const Spectrumf color{ raster[i].Clamp(0.f, 1.f) };
        uchar_raster[3 * i] = static_cast<unsigned char>(255 * color.r);
        uchar_raster[3 * i + 1] = static_cast<unsigned char>(255 * color.g);
        uchar_raster[3 * i + 2] = static_cast<unsigned char>(255 * color.b);
    }

    // Write image
    stbi_flip_vertically_on_write(1);
    if (!stbi_write_png(filename.c_str(), width, height, 3, uchar_raster.data(), 0))
    {
        std::ostringstream error_string;
        error_string << "Error writing PNG image " << filename << "\n";
        throw std::runtime_error(error_string.str());
    }
    stbi_flip_vertically_on_write(0);
}

} // Rabbit namespace
