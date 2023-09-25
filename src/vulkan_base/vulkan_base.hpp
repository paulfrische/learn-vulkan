#pragma once
#include <vector>
#include <vulkan/vulkan.h>

class VulkanContext {
public:
    VulkanContext(std::vector<const char*> necessaryExtensions);

    VkInstance m_instace;
    VkPhysicalDevice m_physicalDevice;
private:
    void initVulkanInstance(std::vector<const char*> necessaryExtensions);
    void selectPhysicalDevice();
};
