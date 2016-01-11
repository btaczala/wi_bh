#include <iostream>
#include <vector>
#include <tuple>
#include <thread>
#include <map>
#include <functional>
#include <sstream>
#include <future>

#include "cmd.hpp"
#include "files.hpp"
#include "log.hpp"


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

void lines(const std::string &buff, int16_t numberOfThreads) {
    using namespace std;

    std::vector<std::future<std::int64_t>> tasks{
        static_cast<std::size_t>(numberOfThreads)};
    std::int64_t total = 0;
    for (int i = 0; i < numberOfThreads; ++i) {
        std::string part;
        std::future<std::int64_t> f =
            std::async(std::launch::async, [](string buff) -> std::int64_t {
                int number =
                    std::count_if(buff.begin(), buff.end(),
                                  [](char c) -> bool { return c == '\n'; });
                return number;
            }, buff);
        tasks[i] = std::move(f);
    }

    for (const auto& f: tasks) {
        f.wait();
    }

    for(auto &f: tasks) {
        total += f.get();
    }

    std::cout << "Lines: " << total/numberOfThreads << std::endl;
}

void histogram(std::string &&histogram, int16_t numberOfThreads) {

    std::stringstream ss(histogram);
    std::string buff;
    std::vector<std::string> vec;
    std::map<std::string, std::int64_t> hist;
    while(ss >> buff) {
        vec.push_back(buff);
    }

    for (const auto& s: vec) {
        hist[s]++;
    }

    std::multimap<std::int64_t, std::string> mmap;

    std::transform(hist.begin(), hist.end(), std::inserter(mmap, mmap.begin()), 
            [](const std::pair<std::string, std::int64_t> &p) {
                return std::make_pair(p.second, p.first);
            });

    std::cout << "Size = " << vec.size() << std::endl;
}

int main(int argc, char *argv[]) {
    const std::vector<std::string> arguments (argv + 1, argv + argc);

    int retVal = EXIT_SUCCESS;

    if (arguments.size() == 0) {
        std::cout << "nope " << std::endl;
    }

    const std::map < std::string,
        std::function<void(std::string &&, std::int16_t)>> operations = {
            {"lines", lines}, {"histogram", histogram}};

    try {
        auto args = cmd::parse(arguments);
        auto filename = std::get<0>(args);

        LOG("Checking if file " << filename << " exists");
        if (!fs::exists(filename)) {
            throw std::runtime_error("File does not exists");
        }

        std::int16_t nrThreads = std::get<3>(args);
        if (nrThreads == 0) {
          nrThreads = std::thread::hardware_concurrency();
        }
        operations.at(std::get<1>(args))(fs::readFile(filename), nrThreads);

    } catch (const std::exception &ex) {
        LOG("Exception " << ex.what());
        retVal = EXIT_FAILURE;
    }
    return retVal;
}
