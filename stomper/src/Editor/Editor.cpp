#include "Editor.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl3.h"
#include "ImGui/imgui_impl_sdlgpu3.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <SDL3_shadercross/SDL_shadercross.h>

Editor::Editor() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(std::format("Failed to initialize SDL: {}",SDL_GetError()));
    }
    m_editorWindow = SDL_CreateWindow("Stomper", 1280, 720,SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!m_editorWindow) {
        throw std::runtime_error(std::format("Failed to initialize editor window: {}",SDL_GetError()));
    }
    m_gameWindow = SDL_CreateWindow("Stomper - Viewport", 1280, 720,SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!m_gameWindow) {
        throw std::runtime_error(std::format("Failed to initialize game window: {}",SDL_GetError()));
    }
    m_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
    if (!m_device) {
        throw std::runtime_error(std::format("Failed to initialize GPUDevice: {}",SDL_GetError()));
    }
    if (!SDL_ClaimWindowForGPUDevice(m_device,m_editorWindow)) {
        throw std::runtime_error(std::format("Failed to claim editor window: {}",SDL_GetError()));
    }
    if (!SDL_ClaimWindowForGPUDevice(m_device,m_gameWindow)) {
        throw std::runtime_error(std::format("Failed to claim game window: {}",SDL_GetError()));
    }
    if (!SDL_ShaderCross_Init()) {
        throw std::runtime_error(std::format("Failed to initialize ShaderCross: {}",SDL_GetError()));
    }

    SetupImGui();
    LoadEditorConfig();
    
    m_editorRenderer = new EditorRenderer(m_editorWindow, m_device,
            SDL_GetGPUSwapchainTextureFormat(m_device, m_editorWindow));
    m_gameRenderer = new Renderer(m_gameWindow, m_device,
            SDL_GetGPUSwapchainTextureFormat(m_device, m_gameWindow));

    m_projectEditor = new ProjectEditor(m_editorWindow, m_gameWindow, &m_editorConfig);
}

void Editor::Run() {
    while (m_running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
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
    ImGui_ImplSDL3_InitForOther(m_editorWindow);
    ImGui_ImplSDLGPU_InitInfo init_info = {};
    init_info.GpuDevice = m_device;
    init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(m_device,m_editorWindow);
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU_Init(&init_info);
}

void Editor::TickEditor() {
    ImGui_ImplSDLGPU_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
    m_projectEditor->Tick();
}

void Editor::TickGame() {
   
}

void Editor::RenderEditor() {
    ImGui::Render();
    SDL_GPUCommandBuffer* cmd = m_editorRenderer->StartRenderingPipeline();
    Imgui_ImplSDLGPU_PrepareDrawData(ImGui::GetDrawData(),cmd);
    SDL_GPUColorTargetInfo info = {
        .texture = m_editorRenderer->GetColorTarget(),
        .clear_color = {1.0f,1.0f,1.0f,1.0f},
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };
    SDL_GPURenderPass* rp = SDL_BeginGPURenderPass(cmd,&info,1,nullptr);
    ImGui_ImplSDLGPU_RenderDrawData(ImGui::GetDrawData(),cmd,rp);
    SDL_EndGPURenderPass(rp);
    m_editorRenderer->PresentFrame();
}

void Editor::RenderGame() {
    SDL_GPUCommandBuffer* cmd = m_gameRenderer->StartRenderingPipeline();
    SDL_GPUColorTargetInfo info = {
        .texture = m_gameRenderer->GetColorTarget(),
        .clear_color = {0.0f,1.0f,1.0f,1.0f},
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };
    SDL_GPURenderPass* rp = SDL_BeginGPURenderPass(cmd,&info,1,nullptr);
    SDL_EndGPURenderPass(rp);
    m_gameRenderer->PresentFrame();
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
    SDL_ShaderCross_Quit();
    SDL_WaitForGPUIdle(m_device);
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU_Shutdown();
    ImGui::DestroyContext();
    m_editorRenderer->Quit();
    m_gameRenderer->Quit();
    SDL_ReleaseWindowFromGPUDevice(m_device,m_editorWindow);
    SDL_ReleaseWindowFromGPUDevice(m_device,m_gameWindow);
    SDL_DestroyGPUDevice(m_device);
    SDL_DestroyWindow(m_editorWindow);
    SDL_DestroyWindow(m_gameWindow);
    SDL_Quit();
    delete m_projectEditor;
    delete m_editorRenderer;
    delete m_gameRenderer;
}
