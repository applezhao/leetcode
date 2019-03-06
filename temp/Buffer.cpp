#include "Buffer.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "CommandPool.h"

Buffer::Buffer(LogicalDevice * logicalDevice, int size, VkBufferCreateFlags usage, VkSharingMode sharingmode)
{
	this->logicalDevice = logicalDevice;
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = sharingmode;

	if (vkCreateBuffer(logicalDevice->vkHandle(), &bufferInfo, nullptr, &handle) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer!");
	}
}

Buffer::~Buffer()
{
	vkDestroyBuffer(logicalDevice->vkHandle(), handle, nullptr);
}

VkCommandBuffer& Buffer::copyDataFromStageBuffer(Buffer* stageBuffer, VkDeviceSize size, CommandPool* commandPool)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool->vkHandle();
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(logicalDevice->vkHandle(), &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, stageBuffer->vkHandle(), handle, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);
	return commandBuffer;
}

MemoryAlloc::MemoryAlloc(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, Buffer* buffer, VkMemoryPropertyFlags properties)
{
	this->physicalDevice = physicalDevice;
	this->logicalDevice = logicalDevice;
	this->buffer = buffer;
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(logicalDevice->vkHandle(), buffer->vkHandle(), &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = __findMemoryType(memRequirements.memoryTypeBits, 
		properties);

	if (vkAllocateMemory(logicalDevice->vkHandle(), &allocInfo, nullptr, &handle) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}
}

MemoryAlloc::~MemoryAlloc()
{
	vkFreeMemory(logicalDevice->vkHandle(), handle, nullptr);
}

void MemoryAlloc::bindBufferMemory()
{
	vkBindBufferMemory(logicalDevice->vkHandle(), buffer->vkHandle(), handle, 0);
}

void MemoryAlloc::copyDataFromMemoryToGpu(void* vertexData, size_t size)
{
	void* data;
	vkMapMemory(logicalDevice->vkHandle(), handle, 0, size, 0, &data);
	memcpy(data, vertexData, size);
	vkUnmapMemory(logicalDevice->vkHandle(), handle);
}

uint32_t MemoryAlloc::__findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice->vkHandle(), &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}
