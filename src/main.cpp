#include <vector>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <spdlog/spdlog.h>

#include "vulkan_base/vulkan_base.hpp"

bool handleMessage()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
        }
    }

    return true;
}

int main()
{
#ifdef DEBUG
    spdlog::set_level(spdlog::level::debug);
#endif
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        spdlog::error("couldn't init sdl: {}", SDL_GetError());
        return 1;
    }

    auto window = SDL_CreateWindow(
        "learn vulkan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1240, 720, SDL_WINDOW_VULKAN);
    if (!window) {
        spdlog::error("couldn't create window: {}", SDL_GetError());
        return 1;
    }

    unsigned int extensionCount;
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);

    std::vector<const char*> extensionNames(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames.data());

    VulkanContext context(extensionNames, std::vector<const char*>(0));

    while (handleMessage()) {
        // render with vulkan
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
