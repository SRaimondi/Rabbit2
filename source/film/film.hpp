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

    const Spectrumf& operator()(unsigned int row, unsigned int column) const noexcept;

    Spectrumf& operator()(unsigned int row, unsigned int column) noexcept;

    void WritePNG(const std::string& filename) const;

private:
    const unsigned int width;
    const unsigned int height;
    std::vector<Spectrumf> raster;
};

} // Rabbit namespace

#endif //RABBIT2_FILM_HPP
