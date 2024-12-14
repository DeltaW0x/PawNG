#pragma once
#include "EditorConfig.h"
#include "EditorRenderer/EditorRenderer.h"
#include "Layers/ProjectEditor.h"
#include <Renderer.h>
#include <Filesystem/Filesystem.h>

class Editor
{
public:
    Editor();
    void Run();

private:
    void Quit();
    void SetupImGui();
    void TickEditor();
    void TickGame();
    void RenderEditor();
    void RenderGame();

    void LoadEditorConfig();
    void SaveEditorConfig();
private:
    bool m_running = true;

    Filesystem m_filesystem;
    EditorConfig m_editorConfig;
    ProjectEditor* m_projectEditor;

    EditorRenderer* m_editorRenderer;
    Renderer* m_gameRenderer;

    SDL_GPUDevice* m_device;
    SDL_Window*    m_editorWindow;
    SDL_Window*    m_gameWindow;
};