#include "Filesystem.h"

std::filesystem::path Filesystem::GetUserPath(){
    return SDL_GetPrefPath("DeltaWorks","Stomper");
}