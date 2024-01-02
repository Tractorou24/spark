#pragma once

namespace spark::lib
{
    /**
     * \brief Helper structure to pass a list of lambdas to a function (like \ref std::visit)
     * \tparam Ts Types of the lambdas
     *
     * Example:
     * \code{.cpp}
     * std::variant<int, float, std::string> var;
     *
     * std::visit(overloaded{
     *     [](int i) { std::print("int: {}\n", i); },
     *     [](float f) { std::print("float: {}\n", f); },
     *     [](auto&& arg) { std::print("other: {}\n", arg); }
     * }, var);
     * \endcode
     */
    template <typename... Ts>
    struct overloaded : Ts...
    {
        using Ts::operator()...;
    };

    template <typename... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;
}
