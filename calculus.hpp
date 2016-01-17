#ifndef CALCULUS_HPP
#define CALCULUS_HPP

#include <vector>

namespace calculus {
template <typename T> using Fn = std::vector<std::pair<T, T>>;

template <typename T> Fn<T> trapezoid(const Fn<T> &t) {
    Fn<T> result;

    for (std::size_t index = 0; index < t.size() - 1; ++index) {
        auto p0 = t[index];
        auto p1 = t[index + 1];

        double area = p1.first - p0.first;
        area *= p1.second - p0.second;
        area *= 0.5;

        result.push_back(std::make_tuple(p1.first, area));
    }

    return result;
}
}

#endif /* CALCULUS_HPP */
