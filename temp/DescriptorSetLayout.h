#pragma once
#include <vulkan\vulkan.h>
#include <map>
class LogicalDevice;
class DescriptorSetLayout
{
public:
	DescriptorSetLayout(LogicalDevice* logicalDevice);
	~DescriptorSetLayout();

	void AddDescriptorBinding(const VkDescriptorType& descriptor_type,
		const VkShaderStageFlags& shader_stage, const uint32_t& descriptor_binding_loc);

	const VkDescriptorSetLayout& vkHandle();

private:
	LogicalDevice* logicalDevice;
	std::map<size_t, VkDescriptorSetLayoutBinding> bindingsData;

	mutable bool ready{ false };
	mutable VkDescriptorSetLayout handle;
};

