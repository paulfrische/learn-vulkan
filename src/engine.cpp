#include "engine.hpp"
#include <cstdint>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan_handles.hpp>

Engine::Engine()
{
    spdlog::debug("engine is created");
    spdlog::debug("width = {}, height = {}", width, height);
    InitWindow();
    MakeInstance();
}
Engine::~Engine()
{
    glfwTerminate(); // TODO: is this RAIIable?
    spdlog::debug("engine is destroyed");
}

void Engine::InitWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, "learn vulkan", nullptr, nullptr);
    if (window != nullptr) {
        spdlog::debug("successfully create a glfw window");
    } else {
        spdlog::error("creating a glfw window failed");
    }
}

void Engine::MakeInstance()
{

    std::uint32_t version = 0;
    if (vk::enumerateInstanceVersion(&version) != vk::Result::eSuccess) {
        spdlog::error("failed to query instance version");
    } else {
        spdlog::debug("found instance version {}", version);
    }
}
