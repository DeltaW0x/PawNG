#pragma once
#include "SQlite/sqlite3.h"
#include <tuple>
#include <filesystem>
#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

struct WindowConfig
{
    bool maximized        = false;
    uint32_t position[2]  = { 0, 0 };
    uint32_t dimension[2] = { 1280,720 };
    SDL_DisplayID monitor = SDL_GetPrimaryDisplay();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(WindowConfig, maximized, position, dimension, monitor)
};

class Project 
{
public:
    Project();
    ~Project();

    bool New(const std::string& name,const fs::path &path);
    bool Load(const fs::path& path);
    bool Save(const fs::path& path);

    bool IsInitialized() const
    {
        return m_initialized;
    }
public:
    std::string name;
    WindowConfig editorWindowConf;
    WindowConfig gameWindowConf;

private:
    sqlite3* m_db;
    bool m_initialized;
};

void to_json(json& j, const Project& p);
void from_json(const json& j, Project& p);
