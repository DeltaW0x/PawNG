#include "Editor.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl3.h"
#include "ImGui/imgui_impl_sdlgpu3.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <SDL3_shadercross/SDL_shadercross.h>

Editor::Editor()
    : m_gpuDevice(SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_SPIRV,true)
    , m_editorWindow("Stomper", 1280, 720,SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY)
    , m_gameWindow("Stomper - Viewport", 1280, 720,SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY)
    , m_editorRenderer(m_editorWindow,m_gpuDevice,SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM)
    , m_gameRenderer(m_gameWindow,m_gpuDevice)
    , m_projectEditor(m_editorWindow,m_gameWindow,m_editorConfig)
{
    if (!SDL_ShaderCross_Init()) {
        throw std::runtime_error(std::format("Failed to initialize ShaderCross: {}",SDL_GetError()));
    }
    m_gpuDevice.ClaimWindow(m_editorWindow);
    m_gpuDevice.ClaimWindow(m_gameWindow);
    SetupImGui();
    LoadEditorConfig();

}

void Editor::Run() {
    while (m_running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            m_projectEditor.HandleEvents(&e);
            switch (e.type) {
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                case SDL_EVENT_QUIT:
                    m_running = false;
                    break;
                default:
                    break;
            }
        }
        TickEditor();
        RenderEditor();
        TickGame();
        RenderGame();
    }
    Quit();
}

void Editor::SetupImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForOther(static_cast<SDL_Window*>(m_editorWindow));
    ImGui_ImplSDLGPU_InitInfo init_info = {};
    init_info.GpuDevice = static_cast<SDL_GPUDevice*>(m_gpuDevice);
    init_info.ColorTargetFormat = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU_Init(&init_info);
}

void Editor::TickEditor() {
    ImGui_ImplSDLGPU_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
    m_projectEditor.Tick();
}

void Editor::TickGame() {
   
}

void Editor::RenderEditor() {
    ImGui::Render();
    CommandBuffer cmd = m_editorRenderer.StartRenderingPipeline();
    Imgui_ImplSDLGPU_PrepareDrawData(ImGui::GetDrawData(),static_cast<SDL_GPUCommandBuffer *>(cmd));
    SDL_GPUColorTargetInfo info = {
        .texture = static_cast<SDL_GPUTexture*>(m_editorRenderer.GetColorTarget()),
        .clear_color = {1.0f,1.0f,1.0f,1.0f},
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };
    RenderPass rp = cmd.BeginRenderPass(info);
    ImGui_ImplSDLGPU_RenderDrawData(ImGui::GetDrawData(),static_cast<SDL_GPUCommandBuffer *>(cmd),static_cast<SDL_GPURenderPass*>(rp));
    cmd.EndRenderPass(rp);
    m_editorRenderer.PresentFrame();
}

void Editor::RenderGame() {
    CommandBuffer cmd = m_gameRenderer.StartRenderingPipeline();
    SDL_GPUColorTargetInfo info = {
        .texture = static_cast<SDL_GPUTexture*>(m_gameRenderer.GetColorTarget()),
        .clear_color = {0.0f,1.0f,1.0f,1.0f},
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };
    RenderPass rp = cmd.BeginRenderPass(info);
    cmd.EndRenderPass(rp);
    m_gameRenderer.PresentFrame();
}

void Editor::LoadEditorConfig()
{
    spdlog::info("Loading editor config from {0}", (m_filesystem.GetUserPath() / "editor_config.json").string());
    if (!exists((m_filesystem.GetUserPath() / "editor_config.json")))
    {
        SaveEditorConfig();
        return;
    }
    json j = json::parse(m_filesystem.ReadFileToString(m_filesystem.GetUserPath() / "editor_config.json"));
    m_editorConfig = j.get<EditorConfig>();
}

void Editor::SaveEditorConfig()
{
    json j = m_editorConfig;
    try
    {
        std::ofstream of;
        of.exceptions(std::ifstream::badbit);
        of.open(m_filesystem.GetUserPath() / "editor_config.json");
        of << j.dump(4);
        of.close();
    }
    catch (const std::ifstream::failure& e)
    {
        spdlog::error("Failed to load project file: {0}", e.what());
    }
    catch (const json::exception& e)
    {
        spdlog::error("Failed to parse project file: {0}", e.what());
    }
}


void Editor::Quit() {
    m_gpuDevice.WaitForIdle();
    SDL_ShaderCross_Quit();
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU_Shutdown();
    ImGui::DestroyContext();
    m_editorRenderer.Quit();
    m_gameRenderer.Quit();
    m_gpuDevice.ReleaseWindow(m_editorWindow);
    m_gpuDevice.ReleaseWindow(m_gameWindow);
}
