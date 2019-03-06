#pragma once
#include <vulkan\vulkan.h>
class LogicalDevice;
class Fence
{
public:
	Fence(LogicalDevice* logicalDevice, VkFenceCreateFlags flags);
	~Fence();

	const VkFence& vkHandle() { return handle; }
private:
	LogicalDevice* logicalDevice;
	VkFence handle;
};

