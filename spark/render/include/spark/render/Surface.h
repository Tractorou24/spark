#pragma once

#include "spark/render/Export.h"

namespace spark::render
{
    /**
     * \brief Represents a surface to render to.
     * A surface can be seen as a window or area on the screen where the renderer can draw to.
     *
     * \note The interface does not make any constraints on the surface to allow for portability. Surface instances are responsible for owning the handle.
     */
    class SPARK_RENDER_EXPORT ISurface
    {
    public:
        virtual ~ISurface() noexcept = default;
    };
}
