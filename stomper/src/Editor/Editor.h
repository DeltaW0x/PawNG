#pragma once
#include "EditorConfig.h"
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

    GPUDevice m_gpuDevice;
    Window    m_editorWindow;
    Window    m_gameWindow;

    Renderer m_editorRenderer;
    Renderer m_gameRenderer;

    Filesystem m_filesystem;
    EditorConfig m_editorConfig;
    ProjectEditor m_projectEditor;
};