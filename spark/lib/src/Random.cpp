#include "spark/lib/Random.h"

#include <chrono>

namespace spark::lib
{
    std::random_device Random::s_randomDevice;
    std::mt19937 Random::s_generator(s_randomDevice() ^ (
                                         static_cast<std::mt19937::result_type>(std::chrono::duration_cast<std::chrono::seconds>(
                                              std::chrono::system_clock::now().time_since_epoch()
                                             ).count()) +
                                         static_cast<std::mt19937::result_type>(std::chrono::duration_cast<std::chrono::microseconds>(
                                              std::chrono::high_resolution_clock::now().time_since_epoch()
                                             ).count())));
}
