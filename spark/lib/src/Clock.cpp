#include "spark/lib/Clock.h"

namespace spark::lib
{
    Clock::Clock()
        : m_startTime(std::chrono::steady_clock::now()) { }

    void Clock::restart()
    {
        m_startTime = std::chrono::steady_clock::now();
    }
}
