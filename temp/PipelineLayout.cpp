#include "PipelineLayout.h"

#include "LogicalDevice.h"
#include "DescriptorSetLayout.h"
#include <iostream>
PipelineLayout::PipelineLayout(LogicalDevice * logicalDevice, int setLayoutCount, DescriptorSetLayout* descriptorSetLayout)
{
	this->logicalDevice = logicalDevice;
	this->setLayoutCount = setLayoutCount;
	this->descriptorSetLayout = descriptorSetLayout;
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
	pipelineLayoutInfo.setLayoutCount = setLayoutCount; // Optional
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout->vkHandle(); // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(logicalDevice->vkHandle(), &pipelineLayoutInfo, nullptr, &handle) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
	std::cout << "pipeline layout is created!" << std::endl;
}
