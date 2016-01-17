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

using namespace std;

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

void lines_singleThread( string &&buff) {
    cout << "Lines = " << count_if(buff.begin(), buff.end(),
                                   [](char c) -> bool { return c == '\n'; })
         << endl;
}

void lines(string &&buff, int16_t numberOfThreads) {
    if (numberOfThreads == 1) {
        lines_singleThread(move(buff));
        return;
    }

    vector<future<int64_t>> tasks{
        static_cast<size_t>(numberOfThreads)};
    int64_t total = 0;
    for (int i = 0; i < numberOfThreads; ++i) {
        future<int64_t> f = async(launch::async, [](string buff) -> int64_t {
            return count_if(buff.begin(), buff.end(),
                            [](char c) -> bool { return c == '\n'; });
        }, buff.substr(0, buff.size() / numberOfThreads));
        tasks[i] = move(f);
    }

    for (auto& f: tasks) {
        f.wait();
        total += f.get();
    }

    cout << "Lines: " << total << endl;
}

void histogram(string &&histogram, int16_t numberOfThreads) {

    stringstream ss(histogram);
    string buff;
    vector<string> vec;
    map<string, int64_t> hist;
    while(ss >> buff) {
        vec.push_back(buff);
    }

    for (const auto& s: vec) {
        hist[s]++;
    }

    multimap<int64_t, string> mmap;

    transform(hist.begin(), hist.end(), inserter(mmap, mmap.begin()), 
            [](const pair<string, int64_t> &p) {
                return make_pair(p.second, p.first);
            });

    cout << "Size = " << vec.size() << endl;
}

int main(int argc, char *argv[]) {
    const vector<string> arguments (argv + 1, argv + argc);

    int retVal = EXIT_SUCCESS;

    if (arguments.size() == 0) {
        cout << "nope " << endl;
    }

    const map < string,
        function<void(string &&, int16_t)>> operations = {
            {"lines", lines}, {"histogram", histogram}};

    try {
        auto args = cmd::parse(arguments);
        auto filename = get<0>(args);

        LOG("Checking if file " << filename << " exists");
        if (!fs::exists(filename)) {
            throw runtime_error("File does not exists");
        }

        int16_t nrThreads = get<3>(args);
        if (nrThreads == 0) {
          nrThreads = thread::hardware_concurrency();
        }
        operations.at(get<1>(args))(fs::readFile(filename), nrThreads);

    } catch (const exception &ex) {
        LOG("Exception " << ex.what());
        retVal = EXIT_FAILURE;
    }
    return retVal;
}
