#pragma once

#include "spark/lib/Export.h"

#include "spark/base/Platforms.h"

#include <chrono>

namespace spark::lib
{
    #ifdef SPARK_OS_WINDOWS
    template <typename T>
    concept IsDuration = std::chrono::_Is_duration_v<std::remove_cvref_t<T>>;
    #else
    template <typename T>
    concept IsDuration = std::chrono::__is_duration<std::remove_cvref_t<T>>::value;
    #endif

    /**
     * \brief A class that can be used to measure time like a stopwatch
     */
    class SPARK_LIB_EXPORT Clock
    {
    public:
        /**
         * \brief Instantiates a new clock and starts it
         */
        Clock();
        ~Clock() = default;

        Clock(const Clock& other) = default;
        Clock(Clock&& other) noexcept = default;
        Clock& operator=(const Clock& other) = default;
        Clock& operator=(Clock&& other) noexcept = default;

        /**
         * \brief Gets the elapsed time since the clock start
         * \tparam T The unit of the returned time
         * \return The time since the clock has started for the specified unit
         */
        template <typename T> requires IsDuration<T>
        [[nodiscard]] float getElapsedTime() const;

        /**
         * \brief Restarts the clock
         */
        void restart();

        /**
         * \brief Restarts the clock and gets the elapsed time
         * \tparam T The unit of the returned time
         * \return The time since the clock has started for the specified unit
         */
        template <typename T> requires IsDuration<T>
        [[nodiscard]] float restart();

    private:
        std::chrono::steady_clock::time_point m_startTime;
    };
}

#include "spark/lib/impl/Clock.h"
