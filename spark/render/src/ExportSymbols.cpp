// Force the exports of symbols in the library. Include the files that are not already included in a cpp.
// Else, the symbols are not exported and it fails to link, searching for the ctor and dtor of the interfaces.

#include "spark/render/Backend.h"
#include "spark/render/GraphicsAdapter.h"
#include "spark/render/GraphicsDevice.h"
#include "spark/render/Shader.h"
#include "spark/render/Surface.h"
