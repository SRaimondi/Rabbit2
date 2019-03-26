//
// Created by Simon on 2019-03-26.
//

#ifndef RABBIT2_ERROR_HPP
#define RABBIT2_ERROR_HPP

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <string>

namespace OpenCL
{

// Convert an OpenCL error code to readable string
std::string ErrorCodeToString(cl_int err_code) noexcept;

// Check an OpenCL error code and throws an exception
void CheckOpenCLCode(cl_int err_code, const char* file, int line);

#define OPENCL_CHECK(status) ::OpenCL::CheckOpenCLCode(status, __FILE__, __LINE__)

} // OpenCL namespace

#endif //RABBIT2_ERROR_HPP
