#pragma once
#include <vulkan\vulkan.h>
#include <map>
class LogicalDevice;
class DescriptorSetLayout;
class DescriptorPool;
class DescriptorSet
{
public:
	DescriptorSet(LogicalDevice* logicalDevice, int swapchainImageSize, DescriptorPool* pool, DescriptorSetLayout* setLayout);
	~DescriptorSet();
	const VkDescriptorSet& vkHandle() { return handle; }
	void addDescriptorInfo(VkDescriptorBufferInfo info, const VkDescriptorType& descr_type, const size_t& item_binding_idx);

	void update();
private:
	VkDescriptorSet handle;
	LogicalDevice* logicalDevice;
	DescriptorPool* pool;
	DescriptorSetLayout* setLayout;

	std::map<size_t, VkWriteDescriptorSet> writeDescriptors;

	std::map<size_t, VkDescriptorBufferInfo> bufferInfos;
	std::map<size_t, VkDescriptorImageInfo> imageInfos;
	std::map<size_t, VkBufferView> bufferViews;
};

