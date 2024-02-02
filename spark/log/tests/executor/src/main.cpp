#include "spark/base/Platforms.h"
#include "spark/log/Logger.h"

#include <format>
#include <functional>
#include <iostream>
#include <map>

template <typename... Args>
const std::map<std::string, std::function<void(std::string_view, Args&&...)>> LOG_FN_MAP = {
    {"trace", spark::log::trace<Args...>},
    {"debug", spark::log::debug<Args...>},
    {"info", spark::log::info<Args...>},
    {"warning", spark::log::warning<Args...>},
    {"error", spark::log::error<Args...>},
    {"critical", spark::log::critical<Args...>}
};

/**
 * \brief Prints the help (usage) message to the console.
 */
void print_help()
{
#ifdef SPARK_OS_WINDOWS
    std::cout << "Usage: ./spark_log_executor.exe [log_level] [message_to_log]" << std::endl;
#else
    std::cout << "Usage: ./spark_log_executor [log_level] [message_to_log]" << std::endl;
#endif

    std::cout << "Log levels: [trace, debug, info, warn, error, critical]" << std::endl;
    std::cout << R"(Example: ./spark_log_executor info "Hello World!")" << std::endl;
}

int main(const int argc, char* argv[])
{
    // Check if the user gave enough arguments to the program.
    if (argc < 3)
    {
        std::cout << "Error: Not enough arguments!\n" << std::endl;
        print_help();
    }

    // Get the log level from the first argument. If the argument is invalid, print the help message and exit.
    if (!LOG_FN_MAP<>.contains(argv[1]))
    {
        std::cout << "Error: Invalid log level!\n" << std::endl;
        print_help();
    }

    // Log the message.
    std::invoke(LOG_FN_MAP<>.at(argv[1]), argv[2]);
    return 0;
}
