#pragma once
#include <vulkan\vulkan.h>
class LogicalDevice;
class PipelineLayout
{
public:
	PipelineLayout(LogicalDevice* logicalDevice);
	~PipelineLayout();
	const VkPipelineLayout& vkHandle() { return handle; }
	void Destroy();
	void Create();

private:
	LogicalDevice* logicalDevice;
	VkPipelineLayout handle;
};

