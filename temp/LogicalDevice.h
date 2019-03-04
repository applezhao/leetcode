#pragma once
#include <vulkan\vulkan.h>
#include <vector>
class PhysicalDevice;
class LogicalDevice
{
public:
	LogicalDevice(PhysicalDevice* physicalDevice, bool enableValidation);
	~LogicalDevice()
	{
		vkDestroyDevice(logicalDevice, nullptr);
	}

	const VkDevice& vkHandle() { return logicalDevice; }
public:
	VkQueue graphicsQueue;
	VkQueue presentQueue;
private:
	VkDevice logicalDevice;
	std::vector<const char*> validationLayers;
	std::vector<const char*> deviceExtensions;
};

