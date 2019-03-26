//
// Created by Simon on 2019-03-26.
//

#include "file_io.hpp"

#include <fstream>
#include <sstream>

namespace IO
{

std::string ReadFile(const std::string& filename)
{
    std::ifstream file{ filename };
    if (!file.is_open())
    {
        std::ostringstream error_string;
        error_string << "Could not open file: " << filename;
        throw std::runtime_error(error_string.str());
    }

    // Read file content
    // FIXME Handle error in file reading
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

} // namespace IO
