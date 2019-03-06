#include "CommandPool.h"

#include "LogicalDevice.h"
#include<iostream>
CommandPool::CommandPool(LogicalDevice* logicalDevice, const VkCommandPoolCreateInfo& create_info)
{
	this->logicalDevice = logicalDevice;
	this->createInfo = create_info;
	if (vkCreateCommandPool(logicalDevice->vkHandle(), &create_info, nullptr, &handle) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool!");
	std::cout << "command [pool is created!" << std::endl;
}


CommandPool::~CommandPool()
{
	Destroy();
}

void CommandPool::ResetCmdPool(const VkCommandPoolResetFlagBits& command_pool_reset_flags)
{
	vkResetCommandPool(logicalDevice->vkHandle(), handle, command_pool_reset_flags);
}

void CommandPool::Destroy()
{
	vkFreeCommandBuffers(logicalDevice->vkHandle(), handle, commandBuffers.size(), commandBuffers.data());
	commandBuffers.clear();

	vkDestroyCommandPool(logicalDevice->vkHandle(), handle, nullptr);
}

void CommandPool::cleanCommandBuffers()
{
	vkFreeCommandBuffers(logicalDevice->vkHandle(), handle, commandBuffers.size(), commandBuffers.data());
	commandBuffers.clear();
}

void CommandPool::AllocateCmdBuffers(const uint32_t & num_buffers, const VkCommandBufferLevel& cmd_buffer_level) 
{

	//if (!commandBuffers.empty()) {
	//	return;
	//}

	commandBuffers.resize(num_buffers);
	VkCommandBufferAllocateInfo alloc_info = {};// vk_command_buffer_allocate_info_base;
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = handle;
	alloc_info.commandBufferCount = num_buffers;
	alloc_info.level = cmd_buffer_level;
	if (vkAllocateCommandBuffers(logicalDevice->vkHandle(), &alloc_info, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
	std::cout << "create command buffers " << num_buffers << std::endl;
}

VkCommandBuffer CommandPool::StartSingleCmdBuffer(int idx) 
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	beginInfo.pInheritanceInfo = nullptr;

	vkBeginCommandBuffer(commandBuffers[idx], &beginInfo);

	return commandBuffers[idx];
}

void CommandPool::EndSingleCmdBuffer(VkCommandBuffer& cmd_buffer, const VkQueue & queue) 
{
	//VkResult result = vkEndCommandBuffer(cmd_buffer);
	//VkAssert(result);

	//VkSubmitInfo submitInfo = {};
	//submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//submitInfo.commandBufferCount = 1;
	//submitInfo.pCommandBuffers = &cmd_buffer;

	//result = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	//VkAssert(result);
	//vkQueueWaitIdle(queue);

	//result = vkResetCommandBuffer(cmd_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	//VkAssert(result);
}
