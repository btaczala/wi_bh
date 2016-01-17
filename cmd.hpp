#ifndef CMD_HPP
#define CMD_HPP

#include <tuple>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <experimental/optional>

#include "log.hpp"

namespace cmd {
typedef std::tuple<std::string, std::string, std::int16_t, std::int32_t>
    CommandLineOptions;

namespace __details {

template <typename T> T from_string(const std::string &v);

template <> std::string from_string(const std::string &s) { return s; }

template <> std::int16_t from_string(const std::string &v) {
    return std::stoi(v);
}

template <typename T>
std::experimental::optional<T> findValue(const std::vector<std::string> &args,
                                         const std::string &name) {
    LOG("Searching for " << name);
    auto iter = std::find_if(args.begin(), args.end(),
                             [name](const std::string &arg) -> bool {
                                 return arg.find(name) != std::string::npos;
                             });
    if (iter == args.end()) {
        LOG("Param " << name << " not found");
        return std::experimental::optional<T>();
    }

    const std::string str = *iter;
    LOG("Param " << str);
    auto searchIter = str.find(name);
    searchIter += name.size() + 1;

    const std::string value = str.substr(searchIter);

    if (value.empty()) {
        return std::experimental::optional<T>();
    }

    return std::experimental::optional<T>{(from_string<T>(value))};
}

template <typename T> T def(const std::experimental::optional<T> &val, T &&t) {
    if (val)
        return *val;
    return t;
}

} // __details

CommandLineOptions parse(const std::vector<std::string> &arguments) {
    auto file = __details::findValue<std::string>(arguments, "file");
    //if (!file)
        //throw std::runtime_error("Needed param --file missing");

    auto operation = __details::findValue<std::string>(arguments, "operation");
    if (!operation)
        throw std::runtime_error("Needed param --operation missing");

    return std::make_tuple(
        *file, *operation,
        __details::def(__details::findValue<std::int16_t>(arguments, "threads"),
                       static_cast<std::int16_t>(0)),
        __details::def(__details::findValue<std::int16_t>(arguments, "threads"),
                       static_cast<std::int16_t>(0)));
}

} // cmd

#endif /* CMD_HPP */
