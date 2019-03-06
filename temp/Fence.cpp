#include "Fence.h"
#include "LogicalDevice.h"


Fence::Fence(LogicalDevice * logicalDevice, VkFenceCreateFlags flags)
{
	this->logicalDevice = logicalDevice;
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = flags;

	if(vkCreateFence(logicalDevice->vkHandle(), &fenceInfo, nullptr, &handle) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create fence!");
	}
}

Fence::~Fence()
{
	vkDestroyFence(logicalDevice->vkHandle(), handle, nullptr);
}
