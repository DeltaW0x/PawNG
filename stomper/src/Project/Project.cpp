#include "Project.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Project::Project() : m_db(nullptr) , m_name("")
{

}

Project::~Project()
{
    sqlite3_close(m_db);
}

bool Project::New(const std::string& name, const fs::path &path)
{
    m_name = name;
    m_lastModified = GetCurrentTimestamp();
    Save(path);
    
    if (sqlite3_open((path / "assets.db").c_str(), &m_db) != SQLITE_OK) {
        spdlog::error("Failed to open project database file: {0}", sqlite3_errmsg(m_db));
        return false;
    }

    char *cmdError = 0;
    const char* sql_cmd = "CREATE TABLE ASSETS(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL);";
    
    if(sqlite3_exec(m_db, sql_cmd, nullptr, 0, &cmdError) != SQLITE_OK) {
        spdlog::error("Failed to create assets database: {0}",cmdError);
        return false;
   }
   return true;
}

bool Project::Load(const fs::path &path)
{
    if (!exists(path)) {
        spdlog::error("Failed to load project at {0}: file does not exist", path.string());
        return false;
    }
    if (sqlite3_open((path.parent_path() / "assets.db").c_str(), &m_db) != SQLITE_OK) {
        spdlog::error("Failed to open project database file: {0}", sqlite3_errmsg(m_db));
        return false;
    }

    json j;
    try {
        std::ifstream ifs(path.string());
        std::stringstream ss;
        ss << ifs.rdbuf();
        j = json::parse(ss.str());

        m_name = j["project_name"].get<std::string>();
        m_lastModified = j["last_modified"].get<std::string>();

    }catch (std::exception &e) {
        spdlog::error("Failed to load project: {0}", e.what());
    }
    return true;
}

bool Project::Save(const fs::path& path)
{
    m_lastModified = GetCurrentTimestamp();

    json j;
    j["project_name"] = m_name;
    j["last_modified"] = m_lastModified;

#pragma region Json & Database write
    try {
        std::ofstream jsonFS(path / fs::path(m_name + ".mproj"));
        jsonFS << j.dump(4);
        jsonFS.close();
    }catch(std::exception& e) {
        spdlog::error("Failed to save project file: {0}", e.what());
        return false;
    }
    if(sqlite3_close(m_db) != SQLITE_OK){
        spdlog::error("Failed to save project database file: {0}", sqlite3_errmsg(m_db));
        return false;
    }
    if (sqlite3_open((path / "assets.db").c_str(), &m_db) != SQLITE_OK) {
        spdlog::error("Failed to open project database file: {0}", sqlite3_errmsg(m_db));
        return false;
    }
#pragma endregion
    return true;
}

std::string Project::GetCurrentTimestamp()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    return oss.str();
}
