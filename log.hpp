#ifndef LOG_HPP
#define LOG_HPP

#ifdef NO_DEBUG
#define LOG(a)
#else
#define LOG(a) std::cout << __FILE__ << "@" << __LINE__ << " " << a << std::endl;
#endif


#endif /* LOG_HPP */
