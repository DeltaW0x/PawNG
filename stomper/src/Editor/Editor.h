#pragma once
#include "EditorRenderer/EditorRenderer.h"
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
private:
    bool m_running = true;

    Filesystem m_filesystem;
    EditorRenderer* m_editorRenderer;
    Renderer* m_gameRenderer;

    SDL_GPUDevice* m_device;
    static SDL_Window*    m_editorWindow;
    static SDL_Window*    m_gameWindow;
};