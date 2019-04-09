//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_IMAGE_INTEGRATOR_HPP
#define RABBIT2_IMAGE_INTEGRATOR_HPP

#include "scene/scene.hpp"
#include "camera/camera.hpp"
#include "film/film.hpp"

namespace Rabbit
{

// Tile in the image to render for a thread
struct Tile
{
    constexpr Tile(const Geometry::Point2ui& start, const Geometry::Point2ui& end) noexcept
        : tile_start{ start }, tile_end{ end }
    {}

    const Geometry::Point2ui tile_start;
    const Geometry::Point2ui tile_end;
};

class ImageIntegrator
{
public:
    ImageIntegrator(const Geometry::Point2ui& tile_size, unsigned int spp) noexcept;

    void RenderImage(const Scene& scene, const Camera& camera, Film& film) const noexcept;

private:
    std::vector<Tile> GenerateTiles(unsigned int image_width, unsigned int image_height) const noexcept;

    // Size of tiles to render
    const Geometry::Point2ui tile_size;
    // Number of samples for each pixel
    const unsigned int samples_per_pixel;
};

} // Rabbit namespace

#endif //RABBIT2_IMAGE_INTEGRATOR_HPP
