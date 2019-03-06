#include "PipelineLayout.h"

#include "LogicalDevice.h"
#include <iostream>
PipelineLayout::PipelineLayout(LogicalDevice * logicalDevice)
{
	this->logicalDevice = logicalDevice;
	Create();
}

PipelineLayout::~PipelineLayout()
{
	Destroy();
}

void PipelineLayout::Destroy()
{
	vkDestroyPipelineLayout(logicalDevice->vkHandle(), handle, nullptr);
}

void PipelineLayout::Create()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(logicalDevice->vkHandle(), &pipelineLayoutInfo, nullptr, &handle) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
	std::cout << "pipeline layout is created!" << std::endl;
}
