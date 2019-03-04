#pragma once
#include <vulkan\vulkan.h>
#include <vector>
class SurfaceKHR;
class PhysicalDevice;
class LogicalDevice;
class SwapChain
{
public:
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		VkSurfaceFormatKHR pickFormat() 
		{
			if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
				return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			}

			for (const auto& availableFormat : formats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}

			return formats[0];
		}
		VkPresentModeKHR pickPresentMode()
		{
			VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

			for (const auto& availablePresentMode : presentModes) {
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return availablePresentMode;
				}
				else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
					bestMode = availablePresentMode;
				}
			}

			return bestMode;
		}
	};

	SwapChain(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, SurfaceKHR* surface);
	~SwapChain();
	//void recreate(SurfaceKHR* surface);

	const VkSwapchainKHR& vkHandle() { return swapchain; }
	const VkFormat& getImageFormat() { return swapChainImageFormat; }
	const VkExtent2D& getExtent() { return extent2D; }
	const std::vector<VkImage> getImages() { return swapChainImages; }
	const std::vector<VkImageView> getImageViews() { return swapChainImageViews; }
private:
	SwapChainSupportDetails swapchainSupportDetails;
	void getSwapChainSupportDetails();
	bool isSwapChainSupported();
	std::vector<const char*> deviceExtensions;

	VkExtent2D getSwapExtent();

	void createImageViews();
private:
	PhysicalDevice* physicalDevice;
	LogicalDevice* logicalDevice;
	SurfaceKHR* surface;
public:
	VkSwapchainKHR swapchain;

	VkSurfaceFormatKHR surfaceFormat;
	VkPresentModeKHR presentMode;
	VkExtent2D extent2D;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;

	std::vector<VkImageView> swapChainImageViews;
};

//void recreateSwapChianAndSurface(SwapChain* swapchain, SurfaceKHR* surface);

