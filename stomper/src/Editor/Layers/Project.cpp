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

bool Project::New(const fs::path &path)
{
}

bool Project::Load(const fs::path &path)
{
}

bool Project::Save(const fs::path& path)
{
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

