#include "vulkan_base.hpp"
#include <algorithm>
#include <cstdint>
#include <memory>
#include <spdlog/spdlog.h>
#include <vector>
#include <vulkan/vulkan_core.h>

VulkanSwapchain::VulkanSwapchain(std::shared_ptr<VulkanContext> context, VkSurfaceKHR surface, VkImageUsageFlags usage)
    : m_context(context)
    , m_surface(surface)
{
    spdlog::debug("construct vulkan swapchain");
    /* m_context = context; */
    VkBool32 supportsPresent = 0;
    if (vkGetPhysicalDeviceSurfaceSupportKHR(
            m_context->m_physicalDevice, m_context->m_queue.familyIndex, m_surface, &supportsPresent)
        != VK_SUCCESS)
        spdlog::error("failed to retrieve surface support of queue");

    if (!supportsPresent)
        spdlog::error("graphics queue does not support present");

    std::uint32_t formatCount;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(context->m_physicalDevice, m_surface, &formatCount, nullptr) != VK_SUCCESS)
        spdlog::error("failed to retrieve count of supported surface formats");

    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(context->m_physicalDevice, m_surface, &formatCount, formats.data())
        != VK_SUCCESS)
        spdlog::error("failed to retrieve supported surface formats");

    if (formats.size() <= 0)
        spdlog::error("no surface formats available");

    VkSurfaceCapabilitiesKHR capabilities;
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_context->m_physicalDevice, m_surface, &capabilities) != VK_SUCCESS)
        spdlog::error("failed to retrieve surface capabilities");

    if (capabilities.currentExtent.width == 0xFFFFFFFF) {
        capabilities.currentExtent = capabilities.maxImageExtent;
    }

    if (capabilities.maxImageCount == 0)
        capabilities.maxImageCount = std::max(capabilities.minImageCount, (uint32_t)8);

    VkSwapchainCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
    createInfo.surface = m_surface;
    createInfo.minImageCount = std::max((uint32_t)3, capabilities.minImageCount);
    createInfo.imageFormat = formats.at(0).format;
    createInfo.imageColorSpace = formats.at(0).colorSpace;
    createInfo.imageExtent = capabilities.currentExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = usage;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    if (vkCreateSwapchainKHR(m_context->m_device, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS)
        spdlog::error("failed to create swapchain");

    std::uint32_t imageCount;
    if (vkGetSwapchainImagesKHR(m_context->m_device, m_swapchain, &imageCount, nullptr) != VK_SUCCESS)
        spdlog::error("failed to retrieve image count of swapchain");

    // Acquire swapchain images
    m_images.resize(imageCount);
    if (vkGetSwapchainImagesKHR(m_context->m_device, m_swapchain, &imageCount, m_images.data()) != VK_SUCCESS)
        spdlog::error("failed to aqcuire swapchain images");

    m_format = formats.at(0).format;
    m_width = capabilities.currentExtent.width;
    m_height = capabilities.currentExtent.height;
}

VulkanSwapchain::~VulkanSwapchain()
{
    if (vkDeviceWaitIdle(m_context->m_device) != VK_SUCCESS)
        spdlog::error("failed awaiting device idle");
    spdlog::debug("swapchain cleanup");
    vkDestroySwapchainKHR(m_context->m_device, m_swapchain, nullptr);
    spdlog::debug("swapchain cleanup finished");
}
