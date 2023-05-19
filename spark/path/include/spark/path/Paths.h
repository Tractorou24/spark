#pragma once

#include "spark/path/Export.h"

#include <filesystem>

namespace spark::path
{
    /**
     * \brief Gets the path of the game's main folder (where the executable is)
     * \return A std::filesystem path representing the game main folder
     */
    std::filesystem::path SPARK_PATH_EXPORT executable_path();

    /**
     * \brief Gets the path of the game's assets folder
     * \return A std::filesystem path representing the asset folder of the game
     */
    std::filesystem::path SPARK_PATH_EXPORT assets_path();
}
