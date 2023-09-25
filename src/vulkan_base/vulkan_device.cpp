#include "vulkan_base.hpp"
#include <cstdint>
#include <iostream>
#include <spdlog/spdlog.h>
#include <stdint.h>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

VulkanContext::VulkanContext(std::vector<const char*> instanceExtensions, std::vector<const char*> deviceExtensions)
{
    spdlog::debug("construct vulkan context");
    initVulkanInstance(instanceExtensions);
    selectPhysicalDevice();
    createLogicalDevice(deviceExtensions);
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

void VulkanContext::createLogicalDevice(std::vector<const char*> deviceExtensions)
{
    // query queues
    std::uint32_t queueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropertiesCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        m_physicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());

    std::uint32_t selectedFamilyIndex;
    spdlog::info("found {} queue families", queueFamilyProperties.size());
    for (std::uint32_t i = 0; i < queueFamilyProperties.size(); ++i) {
        VkQueueFamilyProperties queueFamily = queueFamilyProperties.at(i);
        if (queueFamily.queueCount > 0) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                selectedFamilyIndex = i;
                spdlog::info("found graphics capable queue family.");
                break;
            }
        }
    }

    float priorities[] = { 1.0f };
    VkDeviceQueueCreateInfo queueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    queueCreateInfo.queueFamilyIndex = selectedFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = priorities;

    VkPhysicalDeviceFeatures enabledFeatures = {};

    VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.enabledExtensionCount = deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    createInfo.pEnabledFeatures = &enabledFeatures;

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        spdlog::error("failed to create logical device");

    // Acquire queues
    m_queue.familyIndex = selectedFamilyIndex;
    vkGetDeviceQueue(m_device, selectedFamilyIndex, 0, &m_queue.queue);
}

VulkanContext::~VulkanContext()
{
    spdlog::debug("vulkan context cleanup start.");
    if (vkDeviceWaitIdle(m_device) != VK_SUCCESS)
        spdlog::error("error while awaiting.");
    vkDestroyDevice(m_device, nullptr);
    vkDestroyInstance(m_instace, nullptr);
    spdlog::debug("vulkan context cleanup finished.");
}
