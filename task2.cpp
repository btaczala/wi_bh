#include <iostream>
#include "cmd.hpp"
#include "exprtk/exprtk.hpp"
#include "calculus.hpp"

template <typename T> using Function2D = std::vector<std::pair<T, T>>;

template <typename T> struct Function {

    Function(const std::string &function) : expression(function) {
        symbols.add_variable("x", current_variable);
        symbols.add_constants();
        expr.register_symbol_table(symbols);
        parser.compile(expression, expr);
    }

    T operator()(T t) {
        current_variable = t;
        return expr.value();
    }

    const std::string expression;
    exprtk::symbol_table<T> symbols;
    exprtk::expression<T> expr;
    exprtk::parser<T> parser;
    T current_variable;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Function2D<T> &t) {
    std::for_each(t.begin(), t.end(), [&os](auto v) {
        os << v.first << " " << v.second << std::endl;
    });
    return os;
}

int main(int argc, char *argv[]) {
    using namespace std;
    const vector<string> arguments(argv + 1, argv + argc);

    double from = -5.0f;
    double to = 5.0f;
    double delta = 0.01;
    double res = 10.0f / delta;

    std::size_t samples = static_cast<std::size_t> (res);

    try {
        //auto opts = cmd::parse(arguments);

        const std::string operation = "x";

        Function<double> f{operation};
        Function2D<double> representation {samples};

        double d = from;
        std::transform(representation.begin(), representation.end(), representation.begin(),
                [&d, delta, &f](auto ) -> std::pair<double, double> {
            auto ret = std::make_pair(d, f(d));
            d += delta;
            return ret;
        });

        auto integral = calculus::trapezoid<double>(representation);

        std::cout << representation << std::endl;
        std::cout << std::endl;
        std::cout << integral << std::endl;


    } catch (const std::exception &ex) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
