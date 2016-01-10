#include <iostream>
#include <vector>
#include <tuple>
#include <thread>
#include <map>
#include <functional>

#include "cmd.hpp"
#include "files.hpp"

// task is read a huge file
// and do operation: 
// * count number of lines in file
// * create a histogram 
//
// program have to support command line arguments: 
// * --file=/path/to/file
// * --operation={lines|histogram}
// * --threads={0..100} where 0 is the auto detected value
// * --hist-max={0..100} max number of histogram entries
//

void lines(std::string && buff) {
}

void histogram(std::string &&histogram) {
}

int main(int argc, char *argv[]) {
    const std::vector<std::string> arguments (argv + 1, argv + argc);

    const std::map < std::string,
        std::function<void(std::string &&)>> operations = {
            {"lines", lines}, {"histogram", histogram}};

    try {
        auto args = cmd::parse(arguments);

        std::int16_t nrThreads = std::get<3>(args);
        if (nrThreads == 0) {
          nrThreads = std::thread::hardware_concurrency();
        }
        operations.at(std::get<1>(args))(fs::readFile(std::get<0>(args)));

    } catch (const std::exception &ex) {
    }
    return 0;
}
