#pragma once
#include <SDL3/SDL_storage.h>
#include <filesystem>

class Filesystem {
public:
    Filesystem() = default;
    static std::filesystem::path GetUserPath();
};