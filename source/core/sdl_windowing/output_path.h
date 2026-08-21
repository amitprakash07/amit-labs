#ifndef CORE_SDL_WINDOWING_OUTPUT_PATH_H_
#define CORE_SDL_WINDOWING_OUTPUT_PATH_H_

#include <string>

namespace amit::sdl_windowing
{
    // Returns a path under the executable directory (tmp/<platform>/bin when run from the Labs build).
    // Falls back to the filename alone if the base path cannot be resolved.
    std::string MakeOutputFilePath(const char* filename);

}  // namespace amit::sdl_windowing

#endif
