#pragma once
#include <vulkan\vulkan.h>
class LogicalDevice;
class FrameBuffer
{
public:
	FrameBuffer(LogicalDevice* logicalDevice, const VkFramebufferCreateInfo& create_info);
	~FrameBuffer();

	const VkFramebuffer& vkHandle() { return handle; }
	void Destroy();
private:
	VkFramebuffer handle;
	LogicalDevice* logicalDevice;
	VkFramebufferCreateInfo createInfo;
};

