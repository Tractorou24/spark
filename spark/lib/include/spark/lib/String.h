#pragma once

#include <span>
#include <string>

namespace spark::lib
{
    /**
     * \brief Joins the given list of strings \p strings with the given \p separator.
     * \param strings A \ref std::span of strings to join.
     * \param separator The separator to use between the strings.
     * \return A \ref std::string containing the joined strings.
     */
    [[nodiscard]] constexpr std::string join(const std::span<std::string> strings, const std::string& separator = "") noexcept
    {
        if (strings.empty())
            return "";

        std::string result = strings[0];
        for (std::size_t i = 1; i < strings.size(); ++i)
            result += separator + strings[i];
        return result;
    }
}
