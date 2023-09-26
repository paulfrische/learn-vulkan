#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>
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

    std::vector<const char*> enabledDeviceExtensions;
    enabledDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    std::shared_ptr<VulkanContext> context = std::make_shared<VulkanContext>(extensionNames, enabledDeviceExtensions);
    VkSurfaceKHR surface;
    SDL_Vulkan_CreateSurface(window, context->m_instace, &surface);
    {
        VulkanSwapchain swapchain(context, surface, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);

        while (handleMessage()) {
            // render with vulkan
        }
    }
    vkDestroySurfaceKHR(context->m_instace, surface, nullptr);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
