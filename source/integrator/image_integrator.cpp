//
// Created by Simon on 2019-04-09.
//

#include "image_integrator.hpp"
#include "sampling/sampler.hpp"
#include "light/light.hpp"

#include <thread>
#include <atomic>

namespace Rabbit
{

ImageIntegrator::ImageIntegrator(std::unique_ptr<const RayIntegratorInterface> ray_integrator,
                                 const Geometry::Point2ui& tile_size, unsigned int spp) noexcept
    : ray_integrator{ std::move(ray_integrator) }, tile_size{ tile_size }, samples_per_pixel{ spp }
{}

void ImageIntegrator::RenderImage(const Scene& scene, const CameraInterface& camera, Film& film) const noexcept
{
    // Generate tiles
    const std::vector<Tile> tiles{ GenerateTiles(film.Width(), film.Height()) };

    // Count how many threads we have to launch
    const unsigned int num_threads{ std::min(std::thread::hardware_concurrency(),
                                             static_cast<unsigned int>(tiles.size())) };

    // Atomic tile counter to synchronize access to tiles between threads
    std::atomic_uint next_tile_index{ 0 };

    // Launch threads
    std::vector<std::thread> threads;
    for (unsigned int thread_id = 0; thread_id != num_threads; thread_id++)
    {
        threads.emplace_back(
            [thread_id, &scene, &camera, &film, &tiles, &next_tile_index](const RayIntegratorInterface* integrator,
                                                                          unsigned int spp) -> void
            {
                // Random number generator
                Sampling::Sampler sampler{ thread_id };

                // Compute samples per dimension after rounding to perfect square
                const unsigned int spp_dim{ static_cast<unsigned int>(std::round(std::sqrt(spp))) };
                spp = spp_dim * spp_dim;
                std::vector<Geometry::Point2f> pixel_samples(spp);
                const float inv_num_samples{ 1.f / spp };

                // Get next tile to render
                unsigned int tile_to_render{ next_tile_index++ };
                while (tile_to_render < tiles.size())
                {
                    // Get reference to the tile for the thread
                    const Tile& current_tile{ tiles[tile_to_render] };
                    // Loop over tile domain
                    for (unsigned int pixel_y = current_tile.tile_start.y;
                         pixel_y != current_tile.tile_end.y; pixel_y++)
                    {
                        for (unsigned int pixel_x = current_tile.tile_start.x;
                             pixel_x != current_tile.tile_end.x; pixel_x++)
                        {
                            // Generate stratified samples for pixel
                            sampler.StratifiedSamples(pixel_samples, spp_dim);

                            // Compute pixel radiance
                            Spectrumf pixel_radiance{ 0.f };
                            for (const Geometry::Point2f& sample : pixel_samples)
                            {
                                // Generate ray
                                Geometry::Intervalf ray_interval{ Geometry::Ray::DefaultInterval() };
                                const Geometry::Ray ray{ camera.GenerateRayWorldSpace(
                                    Geometry::Point2ui{ pixel_x, pixel_y }, sample) };

                                // Compute incoming radiance
                                pixel_radiance += integrator->IncomingRadiance(ray, ray_interval, scene, sampler, 0);
                            }

                            // Set pixel radiance in film
                            film(pixel_x, pixel_y) = inv_num_samples * pixel_radiance;
                        }
                    }

                    // Go to next tile
                    tile_to_render = next_tile_index++;
                }
            }, ray_integrator.get(), samples_per_pixel);
    }

    // Wait for all threads to finish
    for (auto& thread : threads)
    {
        thread.join();
    }
}

const std::vector<Tile> ImageIntegrator::GenerateTiles(unsigned int image_width,
                                                       unsigned int image_height) const noexcept
{
    // Compute number of tiles
    const unsigned int num_tiles_width{ DivideUp(image_width, tile_size.x) };
    const unsigned int num_tiles_height{ DivideUp(image_height, tile_size.y) };

    // Generate tiles
    std::vector<Tile> tiles;
    tiles.reserve(num_tiles_width * num_tiles_height);
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

