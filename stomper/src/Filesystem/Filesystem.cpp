#include "Filesystem.h"
#include <sstream>
#include <fstream>

std::filesystem::path Filesystem::GetUserPath(){
    return SDL_GetPrefPath("DeltaWorks","Stomper");
}

std::string Filesystem::ReadFileToString(const std::filesystem::path& path)
{
    std::ifstream in;
    in.exceptions(std::ifstream::badbit);
    in.open(path);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}
