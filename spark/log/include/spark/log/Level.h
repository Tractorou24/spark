#pragma once

namespace spark::log
{
    /**
     * \brief An enum representing the different levels of logging, if any.
     */
    enum class Level
    {
        Trace,
        Info,
        Debug,
        Warning,
        Error,
        Critical,
        Off
    };
}
