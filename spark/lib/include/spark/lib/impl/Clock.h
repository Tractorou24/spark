#pragma once

namespace spark::lib
{
    template <typename T> requires IsDuration<T>
    float Clock::getElapsedTime() const
    {
        return std::chrono::duration<float, typename T::period>(std::chrono::steady_clock::now() - m_startTime).count();
    }

    template <typename T> requires IsDuration<T>
    float Clock::restart()
    {
        const auto elapsed = getElapsedTime<T>();
        m_startTime = std::chrono::steady_clock::now();
        return elapsed;
    }
}
