#pragma once
#include <vulkan\vulkan.h>
#include <vector>
class LogicalDevice;
class CommandPool
{
public:
	CommandPool(LogicalDevice* logicalDevice, const VkCommandPoolCreateInfo& create_info);
	~CommandPool();

	const VkCommandPool& vkHandle() { return handle; }

	void ResetCmdPool(const VkCommandPoolResetFlagBits& command_pool_reset_flags);
	void Destroy();

	void cleanCommandBuffers();

	void AllocateCmdBuffers(const uint32_t & num_buffers, const VkCommandBufferLevel& cmd_buffer_level);
	VkCommandBuffer StartSingleCmdBuffer(int idx);
	void EndSingleCmdBuffer(VkCommandBuffer& cmd_buffer, const VkQueue & queue);

	std::vector<VkCommandBuffer> commandBuffers;
private:
	LogicalDevice* logicalDevice;
	VkCommandPool handle;

	VkCommandPoolCreateInfo createInfo;
};

