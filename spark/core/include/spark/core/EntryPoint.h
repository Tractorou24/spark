#pragma once

#include "spark/core/Application.h"
#include "spark/core/SceneManager.h"
#include "spark/log/Logger.h"

extern std::unique_ptr<spark::core::Application> spark_main(const std::vector<std::string>& args);

// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
int main(const int argc, char* argv[])
{
    // Initialize spark features
    spark::log::info("Initialized Logger");

    // Get command line arguments
    std::vector<std::string> args;
    if (argc > 1)
    {
        args.reserve(argc - 1);
        for (int i = 0; i < argc; i++)
            args.emplace_back(argv[i]);
    }

    // Run application
    const auto app = spark_main(args);
    app->run();

    spark::core::SceneManager::UnregisterAllScenes();

    return 0;
}
