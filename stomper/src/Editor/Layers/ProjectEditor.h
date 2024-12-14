#pragma once
#include "Project.h"
#include "Editor/EditorConfig.h"
#include <SDL3/SDL.h>

enum class ProjectEditorState
{
	NoProject,
	EditingProject
};

class ProjectEditor
{
public:
	ProjectEditor(SDL_Window* editorWindow, SDL_Window* gameWindow,EditorConfig* editorConf);
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

	EditorConfig* m_editorConfig;
	SDL_Window* const m_editorWindow;
	SDL_Window* const m_gameWindow;
};