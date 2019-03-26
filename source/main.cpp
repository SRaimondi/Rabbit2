#include <iostream>

#include "opencl/error.hpp"

int main()
{
    try
    {
        OPENCL_CHECK(CL_INVALID_ARG_SIZE);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}