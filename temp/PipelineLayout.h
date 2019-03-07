#pragma once
#include <vulkan\vulkan.h>
class LogicalDevice;
class DescriptorSetLayout;
class PipelineLayout
{
public:
	PipelineLayout(LogicalDevice* logicalDevice, int setLayoutCount, DescriptorSetLayout* descriptorSetLayout);
	~PipelineLayout();
	const VkPipelineLayout& vkHandle() { return handle; }
	void Destroy();
	void Create();

private:
	LogicalDevice* logicalDevice;
	VkPipelineLayout handle;

	int setLayoutCount;
	DescriptorSetLayout* descriptorSetLayout;
};

