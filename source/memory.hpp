//
// Created by Simon on 2019-04-04.
//

#ifndef RABBIT2_MEMORY_HPP
#define RABBIT2_MEMORY_HPP

#include "utilities.hpp"

#include <cstdlib>

namespace Rabbit
{

constexpr size_t CACHE_LINE_SIZE{ 64 };

// Define utility function to allocate aligned memory
template <typename T, size_t Alignment = CACHE_LINE_SIZE>
T* AllocateAligned(size_t num_elements)
{
    const size_t size{ DivideUp(num_elements * sizeof(T), Alignment) * Alignment };
#ifndef __APPLE__
    return static_cast<T*>(aligned_alloc(Alignment, size));
#else
    T* ptr{ nullptr };
    if (!posix_memalign(reinterpret_cast<void**>(&ptr), Alignment, size))
    {
        return ptr;
    }
    else
    {
        throw std::runtime_error("Error in aligned memory allocation\n");
    }
#endif
}

// Define utility function to free aligned memory
template <typename T>
void FreeAligned(T* mem)
{
    std::free(mem);
}

} // Rabbit namespace

#endif //RABBIT2_MEMORY_HPP
