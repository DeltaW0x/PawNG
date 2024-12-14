#include "ProjectEditor.h"
#include "ImGui/imgui.h"
#include <string>


ProjectEditor::ProjectEditor(SDL_Window* editorWindow, SDL_Window* gameWindow, EditorConfig* editorConf)
	: m_editorWindow(editorWindow)
	, m_gameWindow(gameWindow)
	, m_editorConfig(editorConf)
	, m_editorState(ProjectEditorState::NoProject)
{
	if (!m_editorConfig->project.empty())
	{
		m_project.Load(m_editorConfig->project);
		m_editorState = ProjectEditorState::EditingProject;
	}
}

void ProjectEditor::Tick()
{
	DrawMenuBar();

}

void ProjectEditor::DrawMenuBar()
{
	if(ImGui::BeginMainMenuBar()) 
	{
		if(ImGui::BeginMenu("File"))
		{

			if(ImGui::MenuItem("Open"))
			{	

			}
			if(ImGui::MenuItem("Save","Ctrl-S",nullptr, m_editorState == ProjectEditorState::EditingProject))
			{
				m_project.Save(m_editorConfig->project);
			}
			if(ImGui::MenuItem("New","Ctrl-N"))
			{
				
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

