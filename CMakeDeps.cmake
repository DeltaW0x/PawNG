CPMAddPackage(
        NAME SDL3
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG main
        OPTIONS "SDL_TESTS OFF" "SDL_TEST_LIBRARY OFF" "SDL_STATIC ON" "SDL_SHARED OFF")

CPMAddPackage(
        NAME SDL_shadercross
        GITHUB_REPOSITORY libsdl-org/SDL_shadercross
        GIT_TAG main
        GIT_SUBMODULES_RECURSE ON
        OPTIONS "SDLSHADERCROSS_VENDORED ON" "SDLSHADERCROSS_CLI OFF" "SDLSHADERCROSS_STATIC ON" "SDLSHADERCROSS_SHARED OFF" "SDLSHADERCROSS_SPIRVCROSS_SHARED OFF")

CPMAddPackage(
        NAME json
        GITHUB_REPOSITORY nlohmann/json
        GIT_TAG v3.11.3)

CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        GIT_TAG v1.15.0)

CPMAddPackage(
        NAME DirectXMath
        GITHUB_REPOSITORY microsoft/DirectXMath
        GIT_TAG oct2024)
