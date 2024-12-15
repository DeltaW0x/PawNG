#include "Editor/Editor.h"
int main(){
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(std::format("Failed to initialize SDL: {}",SDL_GetError()));
    }
    Editor editor;
    editor.Run();
    SDL_Quit();
}
