#include "Project.h"
#include "Filesystem/Filesystem.h"
#include <fstream>
#include <spdlog/spdlog.h>


Project::Project() : m_db(nullptr) , name("unnamed_project"), m_initialized(false)
{

}

Project::~Project()
{
    sqlite3_close(m_db);
}

bool Project::New(const std::string& projectName, const fs::path &path)
{
    name = projectName;

    Save(path);
    
    if (sqlite3_open((path / "assets.db").string().c_str(), &m_db) != SQLITE_OK) 
    {
        spdlog::error("Failed to open project database file: {0}", sqlite3_errmsg(m_db));
        return false;
    }

    char *cmdError = 0;
    const char* sql_cmd = "CREATE TABLE ASSETS(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL);";
    
    if(sqlite3_exec(m_db, sql_cmd, nullptr, 0, &cmdError) != SQLITE_OK) 
    {
        spdlog::error("Failed to create assets database: {0}",cmdError);
        return false;
    }
    m_initialized = true;
    return true;
}

bool Project::Load(const fs::path &path)
{
    if (!exists(path)) 
    {
        spdlog::error("Failed to load project at {0}: file does not exist", path.string());
        return false;
    }
    if (sqlite3_open((path.parent_path() / "assets.db").string().c_str(), &m_db) != SQLITE_OK) 
    {
        spdlog::error("Failed to open project database file: {0}", sqlite3_errmsg(m_db));
        return false;
    }

    try 
    {
        json j = json::parse(Filesystem::ReadFileToString(path));
        *this = j.get<Project>();
    }
    catch(const std::ifstream::failure& e)
    {
        spdlog::error("Failed to load project file: {0}", e.what());
        return false;
    }
    catch (const json::exception& e)
    {
        spdlog::error("Failed to parse project file: {0}", e.what());
        return false;
    }
    m_initialized = true;
    return true;
}

bool Project::Save(const fs::path& path)
{
    json j = *this;

    try 
    {
        std::ofstream jsonFS;
        jsonFS.exceptions(std::fstream::failbit);
        jsonFS.open(path / fs::path(name + ".mproj"));
        jsonFS << j.dump(4);
        jsonFS.close();
    }
    catch (const json::exception& e)
    {
        spdlog::error("Failed to compile project file: {0}", e.what());
        return false;
    }
    catch(const std::ifstream::failure& e)
    {
        spdlog::error("Failed to save project file: {0}", e.what());
        return false;
    }
 
    if(sqlite3_close(m_db) != SQLITE_OK){
        spdlog::error("Failed to save project database file: {0}", sqlite3_errmsg(m_db));
        return false;
    }
    if (sqlite3_open((path / "assets.db").string().c_str(), &m_db) != SQLITE_OK) {
        spdlog::error("Failed to open project database file: {0}", sqlite3_errmsg(m_db));
        return false;
    }

    return true;
}

void to_json(json& j, const Project& p)
{
    j = json{ {"name",p.name},{"editor_window",p.editorWindowConf},{"game_window",p.gameWindowConf} };
}

void from_json(const json& j, Project& p)
{
    j.at("name").get_to(p.name);
    j.at("editor_window").get_to<WindowConfig>(p.editorWindowConf);
    j.at("game_window").get_to<WindowConfig>(p.gameWindowConf);
}

