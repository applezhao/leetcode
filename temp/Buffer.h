#pragma once
#include <vulkan\vulkan.h>
class LogicalDevice;
class PhysicalDevice;
class CommandPool;
class Buffer
{
public:
	Buffer(LogicalDevice* logicalDevice, int size, VkBufferCreateFlags usage, VkSharingMode sharingmode);
	~Buffer();
	const VkBuffer& vkHandle() { return handle; }

	VkCommandBuffer& copyDataFromStageBuffer(Buffer* stageBuffer, VkDeviceSize size, CommandPool* commandPool);
private:
	LogicalDevice* logicalDevice;
	VkBuffer handle;
};

class MemoryAlloc
{
public:
	MemoryAlloc(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, Buffer* buffer, VkMemoryPropertyFlags properties);
	~MemoryAlloc();
	const VkDeviceMemory& vkHandle() { return handle; }

	void bindBufferMemory();

	void copyDataFromMemoryToGpu(void* vertexData, size_t size);

	

private:
	uint32_t __findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	PhysicalDevice* physicalDevice;
	LogicalDevice* logicalDevice;
	Buffer* buffer;
	VkDeviceMemory handle;
};

