#pragma once
#include <vulkan\vulkan.h>
class LogicalDevice;
class DescriptorPool
{
public:
	DescriptorPool(LogicalDevice* logicalDevice, int swapchainImageSize);
	~DescriptorPool();
	const VkDescriptorPool& vkHandle() { return handle; }
private:
	LogicalDevice* logicalDevice;
	VkDescriptorPool  handle;
	int descriptorCount;
};


