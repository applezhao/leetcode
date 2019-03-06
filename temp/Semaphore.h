#pragma once
#include <vulkan\vulkan.h>
class LogicalDevice;
class Semaphore
{
public:
	Semaphore(LogicalDevice* logicalDevice);
	~Semaphore();

	const VkSemaphore& vkHandle() { return handle; }
private:
	LogicalDevice* logicalDevice;
	VkSemaphore handle;
};

