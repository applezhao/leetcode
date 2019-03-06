#include "FrameBuffer.h"
#include "LogicalDevice.h"
#include <iostream>
FrameBuffer::FrameBuffer(LogicalDevice * logicalDevice, const VkFramebufferCreateInfo & create_info)
{
	this->logicalDevice = logicalDevice;
	this->createInfo = create_info;
	
	if (vkCreateFramebuffer(logicalDevice->vkHandle(), &create_info, nullptr, &handle) != VK_SUCCESS)
		throw std::runtime_error("failed to create framebuffer!");

	std::cout << "frame buffer is created!" << std::endl;
}

FrameBuffer::~FrameBuffer()
{
	Destroy();
}

void FrameBuffer::Destroy()
{
	vkDestroyFramebuffer(logicalDevice->vkHandle(), handle, nullptr);
}
