#include "Editor/Editor.h"
#include "Project/Project.h"
#include <SDL3_shadercross/SDL_shadercross.h>

#include "spdlog/spdlog.h"

int main(){
    //Editor editor;
    //editor.Run();
    SDL_ShaderCross_Init();
    SDL_GPUShaderFormat hlsl = SDL_ShaderCross_GetHLSLShaderFormats();
    SDL_GPUShaderFormat glsl = SDL_ShaderCross_GetSPIRVShaderFormats();

    if (hlsl & SDL_GPU_SHADERFORMAT_MSL) {
        spdlog::info("Supports cross compilation to MSL");
    }
}
