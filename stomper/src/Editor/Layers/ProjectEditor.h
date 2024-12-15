#pragma once
#include "Project.h"
#include "Editor/EditorConfig.h"
#include <Window.h>

enum class ProjectEditorState
{
	NoProject,
	EditingProject
};

class ProjectEditor
{
public:
	ProjectEditor(Window& editorWindow, Window& gameWindow, EditorConfig& editorConf);
	void Tick();
	void HandleEvents(SDL_Event* e);
private:
	void DrawMenuBar();
	void HandleCallback();
	void NewProject();
	void LoadProject();
	void SaveProject();
private:
	Project m_project;
	ProjectEditorState m_editorState;

	EditorConfig& m_editorConfig;
	Window& m_editorWindow;
	Window& m_gameWindow;
};