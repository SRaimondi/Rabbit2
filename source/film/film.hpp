//
// Created by Simon on 2019-04-02.
//

#ifndef RABBIT2_FILM_HPP
#define RABBIT2_FILM_HPP

#include "spectrum.hpp"

#include <vector>
#include <string>

namespace Rabbit
{

// Film storage class, pixel (0, 0) is bottom left
class Film
{
public:
    Film(unsigned int width, unsigned int height);

    // Access element at given index
    const Spectrumf& operator()(unsigned int pixel_x, unsigned int pixel_y) const noexcept;

    Spectrumf& operator()(unsigned int pixel_x, unsigned int pixel_y) noexcept;

    // Get size of the film
    unsigned int Width() const noexcept
    {
        return width;
    }

    unsigned int Height() const noexcept
    {
        return height;
    }

    // Write Film to .png file
    void WritePNG(const std::string& filename) const;

private:
    const unsigned int width;
    const unsigned int height;
    std::vector<Spectrumf> raster;
};

} // Rabbit namespace

#endif //RABBIT2_FILM_HPP
