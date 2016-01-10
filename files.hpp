#ifndef FILES_HPP
#define FILES_HPP

#include <string>
#include <fstream>
#include <stdexcept>

namespace fs {

// Throws std::runtime_error if not present
std::string readFile(const std::string& filename) {
    std::ifstream in(filename.c_str(), std::ios::in);
    if (in) {
        std::string buff;
        buff.reserve(in.tellg());
        in.seekg(0);
        in.read(&buff[0], buff.size());
        in.close();
        return buff;
    }
    throw std::runtime_error("Unable to open the file");
}
}

#endif /* FILES_HPP */
