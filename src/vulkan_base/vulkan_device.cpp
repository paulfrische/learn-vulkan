#include "vulkan_base.hpp"
#include <cstdint>
#include <iostream>
#include <spdlog/spdlog.h>
#include <stdint.h>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

VulkanContext::VulkanContext(std::vector<const char*> necessaryExtensions)
{
    spdlog::debug("construct vulkan context");
    initVulkanInstance(necessaryExtensions);
    selectPhysicalDevice();
}

void VulkanContext::initVulkanInstance(std::vector<const char*> necessaryExtensions)
{
    // layers
    std::uint32_t layerPropertyCount;
    if (vkEnumerateInstanceLayerProperties(&layerPropertyCount, nullptr))
        spdlog::error("failed to retrieve layer count.");

    std::vector<VkLayerProperties> layerProperties(layerPropertyCount);
    if (vkEnumerateInstanceLayerProperties(&layerPropertyCount, layerProperties.data()))
        spdlog::error("failed to retrieve layers.");

    for (const auto layer : layerProperties) {
        spdlog::info("Layer found '{}': {}", layer.layerName, layer.description);
    }

    std::vector<const char*> enabledLayers {};
#ifdef DEBUG
    enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif // DEBUG

    // extensions
    std::uint32_t extensionPropertyCount;
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, nullptr) != VK_SUCCESS)
        spdlog::error("failed to retrieve extension count.");

    std::vector<VkExtensionProperties> extensionProperties(extensionPropertyCount);
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, extensionProperties.data())
        != VK_SUCCESS)
        spdlog::error("failed to retrieve extensions.");

    for (const auto extension : extensionProperties) {
        spdlog::info("Extension found '{}'", extension.extensionName);
    }

    VkApplicationInfo applicationInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
    applicationInfo.pApplicationName = "Learn Vulkan";
    applicationInfo.pEngineName = "YEEEETGINE";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    applicationInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledLayerCount = enabledLayers.size();
    createInfo.ppEnabledLayerNames = enabledLayers.data();
    createInfo.enabledExtensionCount = necessaryExtensions.size();
    createInfo.ppEnabledExtensionNames = necessaryExtensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &m_instace) != VK_SUCCESS)
        spdlog::error("failed to create vulkan instance.");
    else
        spdlog::debug("created vkInstance");
}

void VulkanContext::selectPhysicalDevice()
{
    std::uint32_t deviceCount = 0;
    if (vkEnumeratePhysicalDevices(m_instace, &deviceCount, nullptr) != VK_SUCCESS)
        spdlog::error("failed to retrieve physical device count.");

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    if (vkEnumeratePhysicalDevices(m_instace, &deviceCount, physicalDevices.data()) != VK_SUCCESS)
        spdlog::error("failed to retrieve physical devices.");

    spdlog::info("found {} physical device(s)", deviceCount);
    for (const auto device : physicalDevices) {
        VkPhysicalDeviceProperties properties = {};
        vkGetPhysicalDeviceProperties(device, &properties);
        spdlog::info("found device {}", properties.deviceName);
    }

    m_physicalDevice = physicalDevices.at(0);
    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);

    spdlog::info("selected {}", m_physicalDeviceProperties.deviceName);
}
