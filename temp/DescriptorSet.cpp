#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "LogicalDevice.h"


DescriptorSet::DescriptorSet(LogicalDevice* logicalDevice, 
	int swapchainImageSize, DescriptorPool* pool, DescriptorSetLayout* setLayout)
{
	this->logicalDevice = logicalDevice;
	this->pool = pool;
	this->setLayout = setLayout;

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool->vkHandle();
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &setLayout->vkHandle();

	vkAllocateDescriptorSets(logicalDevice->vkHandle(), &allocInfo, &handle);


}

void DescriptorSet::update()
{
	std::vector<VkWriteDescriptorSet> write_descriptors;
	for (const auto& entry : writeDescriptors) 
	{
		write_descriptors.emplace_back(entry.second);
		write_descriptors.back().dstSet = handle;
		if (entry.second.pBufferInfo != nullptr) {
			write_descriptors.back().pBufferInfo = entry.second.pBufferInfo;
		}
		if (entry.second.pImageInfo != nullptr) {
			write_descriptors.back().pImageInfo = entry.second.pImageInfo;
		}
		if (entry.second.pTexelBufferView != nullptr) {
			write_descriptors.back().pTexelBufferView = entry.second.pTexelBufferView;
		}
	}

	vkUpdateDescriptorSets(logicalDevice->vkHandle(), 
		static_cast<uint32_t>(write_descriptors.size()), write_descriptors.data(), 0, nullptr);
}

void DescriptorSet::addDescriptorInfo(VkDescriptorBufferInfo info,
	const VkDescriptorType& descr_type, const size_t& item_binding_idx)
{
	bufferInfos[item_binding_idx] = info;
	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = handle;
	descriptorWrite.dstBinding = item_binding_idx;
	descriptorWrite.dstArrayElement = 0;

	descriptorWrite.descriptorType = descr_type;
	descriptorWrite.descriptorCount = 1;

	descriptorWrite.pBufferInfo = &bufferInfos[item_binding_idx];
	descriptorWrite.pImageInfo = nullptr; // Optional
	descriptorWrite.pTexelBufferView = nullptr; // Optional

	writeDescriptors.emplace(item_binding_idx, std::move(descriptorWrite));
}


DescriptorSet::~DescriptorSet()
{
	vkFreeDescriptorSets(logicalDevice->vkHandle(), pool->vkHandle(), 1, &handle);
}
