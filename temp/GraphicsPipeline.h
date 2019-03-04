#pragma once

#include <vulkan\vulkan.h>
class LogicalDevice;

struct GraphicsPipelineInfo 
{
	GraphicsPipelineInfo(VkExtent2D swapExtent);

	VkPipelineVertexInputStateCreateInfo VertexInfo;
	VkPipelineInputAssemblyStateCreateInfo AssemblyInfo;
	VkPipelineTessellationStateCreateInfo TesselationInfo;
	VkPipelineViewportStateCreateInfo ViewportInfo;
	VkPipelineRasterizationStateCreateInfo RasterizationInfo;
	VkPipelineMultisampleStateCreateInfo MultisampleInfo;
	VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
	VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
	VkPipelineDynamicStateCreateInfo DynamicStateInfo;

	/** This method returns a VkGraphicsPipelineCreateInfo struct with its internal state object pointers set
	*   to point to the members of this class. This is useful for short-cutting having to set them all yourself,
	*   but be aware of object lifetime and make sure the class instance being pointed to exists when using the
	*   returned VkGraphicsPipelineCreateInfo struct! Also, note that not all fields are filled: you MUST fill
	*   the following fields yourself:
	*   - renderPass
	*   - subpass
	*   - layout
	*   - stageCount
	*   - pStages
	*/
	VkGraphicsPipelineCreateInfo GetPipelineCreateInfo() const;
};

class GraphicsPipeline
{
public:
	GraphicsPipeline(LogicalDevice* logicalDevice, VkGraphicsPipelineCreateInfo create_info, VkPipeline handle);
	~GraphicsPipeline();

	const VkPipeline& vkHandle() { return handle; }
private:
	LogicalDevice* logicalDevice;
	VkPipeline handle;
	VkGraphicsPipelineCreateInfo createInfo;
};

