#pragma once
#include "SQlite/sqlite3.h"
#include <filesystem>

namespace fs = std::filesystem;
class Project {
public:
    Project();
    ~Project();

    bool New(const std::string& name,const fs::path &path);
    bool Load(const fs::path& path);
    bool Save(const fs::path& path);

private:
    std::string GetCurrentTimestamp();
private:
    std::string m_name;
    std::string m_lastModified;
    sqlite3* m_db;
};
