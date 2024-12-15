#include "Editor/Editor.h"
#include "Logging/Logger.h"
int main() {
	Logger logger(true, true);
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		spdlog::critical("Failed to initialize SDL: {0}", SDL_GetError());
	}
	Editor editor;
	editor.Run();
	SDL_Quit();
}
