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
		m_editorWindow.SetTitle("Stomper - " + m_project.name);
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
		m_project.New(g_projectPath);
		m_editorConfig.project = g_projectPath;
		g_newProjectSignal = false;
		g_projectPath = "";
		m_editorState = ProjectEditorState::EditingProject;
	}
	if (g_loadProjectSignal)
	{
		m_project.Load(g_projectPath);

		m_editorWindow.SetPosition(float2(m_project.editorWindowConf.position[0], m_project.editorWindowConf.position[1]));
		m_editorWindow.SetSize(float2(m_project.editorWindowConf.dimension[0], m_project.editorWindowConf.dimension[1]));
		m_gameWindow.SetPosition(float2(m_project.gameWindowConf.position[0], m_project.gameWindowConf.position[1]));
		m_gameWindow.SetSize(float2(m_project.gameWindowConf.dimension[0], m_project.gameWindowConf.dimension[1]));

		m_editorConfig.project = g_projectPath;

		g_loadProjectSignal = false;
		g_projectPath = "";
		m_editorState = ProjectEditorState::EditingProject;
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
	m_project.editorWindowConf.dimension[0] = m_editorWindow.GetSize().x;
	m_project.editorWindowConf.dimension[1] = m_editorWindow.GetSize().y;
	m_project.editorWindowConf.position[0]  = m_editorWindow.GetPosition().x;
	m_project.editorWindowConf.position[1]  = m_editorWindow.GetPosition().y;
	m_project.editorWindowConf.monitor      = m_editorWindow.GetDisplay();

	m_project.gameWindowConf.dimension[0] = m_gameWindow.GetSize().x;
	m_project.gameWindowConf.dimension[1] = m_gameWindow.GetSize().y;
	m_project.gameWindowConf.position[0]  = m_gameWindow.GetPosition().x;
	m_project.gameWindowConf.position[1]  = m_gameWindow.GetPosition().y;
	m_project.gameWindowConf.monitor      = m_gameWindow.GetDisplay();

	m_project.Save(m_editorConfig.project);
}

