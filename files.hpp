#ifndef FILES_HPP
#define FILES_HPP

#include <string>
#include <fstream>
#include <stdexcept>

namespace fs {

// Throws std::runtime_error if not present
std::string readFile(const std::string& filename) {
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
    if(!in) {
      throw std::runtime_error("Unable to open the file");
    }

    std::string buff;
    in.seekg(0, std::ios::end);
    buff.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&buff[0], buff.size());
    in.close();
    return buff;
}

bool exists(const std::string &filename) {
    if (filename.empty()) 
        return false;
    std::ifstream str {filename.c_str()};
    bool bRet = false;
    if (str.good()) {
        bRet = true;
    }
    str.close();
    return bRet;
}
}

#endif /* FILES_HPP */
