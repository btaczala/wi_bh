#ifndef CMD_HPP
#define CMD_HPP

#include <tuple>
#include <string>
#include <vector>

namespace cmd  {
typedef std::tuple<std::string, std::string, std::int16_t, std::int32_t>
    CommandLineOptions;

namespace __details {

template<typename T>
T findValue(const std::vector<std::string> &args, const std::string &name, T defaultValue)
{
  auto iter = std::find_if(args.begin(), args.end(),
                           [name](const std::string &arg) -> bool {
                             return arg.find(name) != std::string::npos;
                           });
    return defaultValue;
}

} // __details

CommandLineOptions parse(const std::vector<std::string> &arguments) {
  return std::make_tuple(__details::findValue(arguments, "file", ""),
                         __details::findValue(arguments, "operation", "lines"),
                         __details::findValue(arguments, "threads", 0),
                         __details::findValue(arguments, "hist-max", 0));
}

} // cmd

#endif /* CMD_HPP */
