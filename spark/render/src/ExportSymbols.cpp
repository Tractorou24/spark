// Force the exports of symbols in the library. Include the files that are not already included in a cpp.
// Else, the symbols are not exported and it fails to link, searching for the ctor and dtor of the interfaces.

#include "spark/render/GraphicsAdapter.h"
