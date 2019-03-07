#include "DescriptorPool.h"
#include "LogicalDevice.h"


DescriptorPool::DescriptorPool(LogicalDevice* logicalDevice, int swapchainImageSize)
{
	this->logicalDevice = logicalDevice;
	this->descriptorCount = swapchainImageSize;

	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(descriptorCount);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(descriptorCount);

	if (vkCreateDescriptorPool(logicalDevice->vkHandle(), &poolInfo, nullptr, &handle) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}


DescriptorPool::~DescriptorPool()
{
	vkDestroyDescriptorPool(logicalDevice->vkHandle(), handle, nullptr);
}
