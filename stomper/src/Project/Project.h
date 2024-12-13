#pragma once
#include "SQlite/sqlite3.h"
#include <tuple>
#include <filesystem>
#include <SDL3/SDL.h>
namespace fs = std::filesystem;


class Project {
public:
    Project();
    ~Project();

    bool New(const std::string& name,const fs::path &path);
    bool Load(const fs::path& path);
    bool Save(const fs::path& path);

public:
    std::string name;
private:
    sqlite3* m_db;
};

inline void to_json(json& j, const Project& p) {
    j = json
    { 
        {"project_name", p.name}
    };
}

inline void from_json(const json& j, Project& p) {
    j.at("project_name").get_to(p.name);
}