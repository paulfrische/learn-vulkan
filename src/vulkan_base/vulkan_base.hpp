#pragma once
#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

struct VulkanQueue {
    VkQueue queue;
    std::uint32_t familyIndex;
};

class VulkanContext {
public:
    VulkanContext(std::vector<const char*> instanceExtensions, std::vector<const char*> deviceExtensions);

    VkInstance m_instace;
    VkPhysicalDevice m_physicalDevice;
    VkPhysicalDeviceProperties m_physicalDeviceProperties;
    VkDevice m_device;
    VulkanQueue m_queue;

    ~VulkanContext();

private:
    void initVulkanInstance(std::vector<const char*> necessaryExtensions);
    void selectPhysicalDevice();
    void createLogicalDevice(std::vector<const char*> deviceExtensions);
};
