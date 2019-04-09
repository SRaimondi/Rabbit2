//
// Created by Simon on 2019-04-09.
//

#include "image_integrator.hpp"

namespace Rabbit
{

ImageIntegrator::ImageIntegrator(const Geometry::Point2ui& tile_size, unsigned int spp) noexcept
    : tile_size{ tile_size }, samples_per_pixel{ spp }
{}

void ImageIntegrator::RenderImage(const Scene& scene, const Camera& camera, Film& film) const noexcept
{

}

std::vector<Tile> ImageIntegrator::GenerateTiles(unsigned int image_width, unsigned int image_height) const noexcept
{
    // Compute number of tiles
    const unsigned int num_tiles_width{ DivideUp(image_width, tile_size.x) };
    const unsigned int num_tiles_height{ DivideUp(image_height, tile_size.y) };

    // Generate tiles
    std::vector<Tile> tiles;
    for (unsigned int tile_h = 0; tile_h != num_tiles_height; tile_h++)
    {
        for (unsigned int tile_w = 0; tile_w != num_tiles_width; tile_w++)
        {
            const Geometry::Point2ui start{ tile_w * tile_size.x, tile_h * tile_size.y };
            const Geometry::Point2ui end{ std::min(start.x + tile_size.x, image_width),
                                          std::min(start.y + tile_size.y, image_height) };
            tiles.emplace_back(start, end);
        }
    }

    return tiles;
}

} // Rabbit namespace

