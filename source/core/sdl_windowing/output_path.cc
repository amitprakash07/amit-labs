#include "core/sdl_windowing/output_path.h"

#include <SDL3/SDL.h>

namespace amit::sdl_windowing
{
    std::string MakeOutputFilePath(const char* filename)
    {
        const char* name = (filename != nullptr && filename[0] != '\0') ? filename : "output";

        // SDL3: cached absolute path ending in a separator; do not free.
        const char* base_path = SDL_GetBasePath();
        if (base_path == nullptr)
        {
            return name;
        }

        return std::string(base_path) + name;
    }

}  // namespace amit::sdl_windowing
