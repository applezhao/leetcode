#include "RenderPass.h"
#include "LogicalDevice.h"
#include <iostream>
RenderPass::RenderPass(LogicalDevice * logicalDevice, const VkRenderPassCreateInfo & create_info)
{
	this->logicalDevice = logicalDevice;
	this->createInfo = create_info;
	if(vkCreateRenderPass(this->logicalDevice->vkHandle(), &create_info, nullptr, &handle)!=VK_SUCCESS)
		throw std::runtime_error("failed to create render pass!");
	std::cout << "render pass is created!" << std::endl;
}

RenderPass::~RenderPass()
{
	Destroy();
}

void RenderPass::SetupBeginInfo(const VkClearValue * clear_values, const size_t num_values, const VkExtent2D & render_area)
{
	beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	beginInfo.renderPass = handle;
	beginInfo.renderArea = VkRect2D{ VkOffset2D{ 0, 0 }, VkExtent2D{ render_area } };

	clearValues = std::vector<VkClearValue>{ clear_values, clear_values + num_values };
	beginInfo.clearValueCount = clearValues.size();
	beginInfo.pClearValues = clearValues.data();
}

void RenderPass::UpdateBeginInfo(const VkFramebuffer & current_framebuffer)
{
	beginInfo.framebuffer = current_framebuffer;
}

void RenderPass::Destroy()
{
	vkDestroyRenderPass(this->logicalDevice->vkHandle(), handle, nullptr);
}

void RenderPass::Recreate()
{
	vkCreateRenderPass(this->logicalDevice->vkHandle(), &createInfo, nullptr, &handle);
}
