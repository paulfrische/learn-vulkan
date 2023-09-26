#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

struct VulkanQueue {
    VkQueue queue;
    std::uint32_t familyIndex;
};

class VulkanContext {
public:
    VulkanContext(std::vector<const char*> instanceExtensions, std::vector<const char*> deviceExtensions);
    ~VulkanContext();

    VkInstance m_instace;
    VkPhysicalDevice m_physicalDevice;
    VkPhysicalDeviceProperties m_physicalDeviceProperties;
    VkDevice m_device;
    VulkanQueue m_queue;

private:
    void initVulkanInstance(std::vector<const char*> necessaryExtensions);
    void selectPhysicalDevice();
    void createLogicalDevice(std::vector<const char*> deviceExtensions);
};

class VulkanSwapchain {
public:
    VulkanSwapchain(std::shared_ptr<VulkanContext> context, VkSurfaceKHR surface, VkImageUsageFlags usage);
    ~VulkanSwapchain();

    VkSwapchainKHR m_swapchain;
    VkSurfaceKHR m_surface;
    std::shared_ptr<VulkanContext> m_context;
    std::vector<VkImage> m_images;
    std::uint32_t m_width;
    std::uint32_t m_height;
    VkFormat m_format;
};
