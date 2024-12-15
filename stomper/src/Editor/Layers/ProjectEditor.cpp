#include "ProjectEditor.h"
#include "ImGui/imgui.h"
#include <string>
#include <spdlog/spdlog.h>

#define UNI_CTRL (e->key.mod & SDL_KMOD_GUI || e->key.mod & SDL_KMOD_CTRL)
static SDL_DialogFileFilter g_filter = {"Stomper Project","mproj"};
static bool g_newProjectSignal   = false;
static bool g_loadProjectSignal  = false;
static std::string g_projectPath = "";

static void NewProjectCallback(void *userdata, const char * const *filelist, int filter)
{
	if (!filelist[0])
		return;
	g_projectPath = filelist[0];
	g_newProjectSignal = true;
}

static void LoadProjectCallback(void *userdata, const char * const *filelist, int filter)
{
	if (!filelist[0])
		return;
	g_projectPath = filelist[0];
	g_loadProjectSignal = true;
}

ProjectEditor::ProjectEditor(Window& editorWindow, Window& gameWindow, EditorConfig& editorConf)
	: m_editorWindow(editorWindow)
	, m_gameWindow(gameWindow)
	, m_editorConfig(editorConf)
	, m_editorState(ProjectEditorState::NoProject)
{
	if (!m_editorConfig.project.empty())
	{
		m_project.Load(m_editorConfig.project);
		m_editorState = ProjectEditorState::EditingProject;
	}
}

void ProjectEditor::Tick()
{
	DrawMenuBar();
	HandleCallback();
}

void ProjectEditor::HandleEvents(SDL_Event *e)
{
	switch (e->type)
	{
		case SDL_EVENT_KEY_DOWN:
			if (e->key.key == SDLK_S && UNI_CTRL && m_project.IsInitialized())
			{
				SaveProject();
			}
	}
}

void ProjectEditor::DrawMenuBar()
{
	if(ImGui::BeginMainMenuBar()) 
	{
		if(ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("Open"))
			{	
				LoadProject();
			}
			if(ImGui::MenuItem("Save","Ctrl-S",nullptr, m_editorState == ProjectEditorState::EditingProject))
			{
				SaveProject();
			}
			if(ImGui::MenuItem("New","Ctrl-N"))
			{
				NewProject();
			}
			if(ImGui::MenuItem("Quit","Alt-F4"))
			{
				SDL_Event e;
				e.type = SDL_EVENT_QUIT;
				SDL_PushEvent(&e);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ProjectEditor::HandleCallback()
{
	if (g_newProjectSignal)
	{
		auto p = fs::path(g_projectPath);
		m_project.New(p.filename().string(),p.parent_path());
		g_newProjectSignal = false;
		g_projectPath = "";
	}
	if (g_loadProjectSignal)
	{
		auto p = fs::path(g_projectPath);
		m_project.Load(g_projectPath);
		g_loadProjectSignal = false;
		g_projectPath = "";
	}
}

void ProjectEditor::NewProject()
{
	SDL_ShowSaveFileDialog(&NewProjectCallback,nullptr,static_cast<SDL_Window *>(m_editorWindow),&g_filter,1,nullptr);
}

void ProjectEditor::LoadProject()
{
	SDL_ShowOpenFileDialog(&NewProjectCallback,nullptr,static_cast<SDL_Window *>(m_editorWindow),&g_filter,1,nullptr,false);
}

void ProjectEditor::SaveProject()
{
	m_project.Save(m_editorConfig.project);
}

