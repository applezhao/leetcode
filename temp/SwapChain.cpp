#include "SwapChain.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "SurfaceKHR.h"
#include <iostream>
#include <set>
#include <algorithm>

SwapChain::SwapChain(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, SurfaceKHR* surface)
{
	this->physicalDevice = physicalDevice;
	this->logicalDevice = logicalDevice;
	this->surface = surface;
	deviceExtensions.clear();
	deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	getSwapChainSupportDetails();
	if (isSwapChainSupported() == false)
	{
		std::cout << "swapchain is not supported!" << std::endl;
	}

	surfaceFormat = swapchainSupportDetails.pickFormat();
	presentMode = swapchainSupportDetails.pickPresentMode();
	extent2D = getSwapExtent();

	uint32_t imageCount = swapchainSupportDetails.capabilities.minImageCount + 1;
	if (swapchainSupportDetails.capabilities.maxImageCount > 0 && imageCount > swapchainSupportDetails.capabilities.maxImageCount) {
		imageCount = swapchainSupportDetails.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface->vkHandle();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent2D;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

	createInfo.preTransform = swapchainSupportDetails.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(logicalDevice->vkHandle(), &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}
	std::cout << "swapchain is created!" << std::endl;
	vkGetSwapchainImagesKHR(logicalDevice->vkHandle(), swapchain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice->vkHandle(), swapchain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
}

VkExtent2D SwapChain::getSwapExtent()
{
	if (swapchainSupportDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return swapchainSupportDetails.capabilities.currentExtent;
	}
	else
	{
		surface->updateWindowSize();
		VkExtent2D actualExtent = { surface->getWidth() ,  surface->getHeight() };

		actualExtent.width = std::max(swapchainSupportDetails.capabilities.minImageExtent.width,
			std::min(swapchainSupportDetails.capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(swapchainSupportDetails.capabilities.minImageExtent.height,
			std::min(swapchainSupportDetails.capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

void SwapChain::createImageViews()
{
	swapChainImageViews.resize(swapChainImages.size());
	for (size_t i = 0; i < swapChainImages.size(); i++) 
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];

		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(logicalDevice->vkHandle(), &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}
}

void SwapChain::getSwapChainSupportDetails()
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->vkHandle(), surface->vkHandle(), 
		&swapchainSupportDetails.capabilities);
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice->vkHandle(), surface->vkHandle(), &formatCount, nullptr);

	if (formatCount != 0) {
		swapchainSupportDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice->vkHandle(), surface->vkHandle(), 
			&formatCount, swapchainSupportDetails.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->vkHandle(), surface->vkHandle(), &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		swapchainSupportDetails.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->vkHandle(), surface->vkHandle(),
			&presentModeCount, swapchainSupportDetails.presentModes.data());
	}
}

bool SwapChain::isSwapChainSupported()
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice->vkHandle(), nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice->vkHandle(), nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) 
	{
		requiredExtensions.erase(extension.extensionName);
	}
	bool extensionsSupported = requiredExtensions.empty();
	//
	bool swapChainAdequate = false;
	if (extensionsSupported) 
	{
		swapChainAdequate = !swapchainSupportDetails.formats.empty() && !swapchainSupportDetails.presentModes.empty();
	}

	return  extensionsSupported && swapChainAdequate;
}


SwapChain::~SwapChain()
{
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(logicalDevice->vkHandle(), imageView, nullptr);
	}
	vkDestroySwapchainKHR(logicalDevice->vkHandle(), swapchain, nullptr);
}
