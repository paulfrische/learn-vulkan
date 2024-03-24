#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

class Engine {
public:
    Engine();
    ~Engine();

private:
    int width = 640;
    int height = 480;
    GLFWwindow* window = nullptr;

    vk::Instance instance = nullptr;

    void InitWindow();
    void MakeInstance();
};
