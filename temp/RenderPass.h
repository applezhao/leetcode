#pragma once
#include <vulkan\vulkan.h>
#include <vector>
class LogicalDevice;
class RenderPass
{
public:
	RenderPass(LogicalDevice* logicalDevice, const VkRenderPassCreateInfo& create_info);
	~RenderPass();

	/** Must be setup before trying to use the VkRenderPassBeginInfo object attached to this class. Will cause validation layer and rendering errors otherwise. */
	void SetupBeginInfo(const VkClearValue* clear_values, const size_t num_values, const VkExtent2D& render_area);
	/** Call each frame or when changing a framebuffer to ensure the correct framebuffer is rendered to.*/
	void UpdateBeginInfo(const VkFramebuffer& current_framebuffer);
	/**Effectively resets this object back to a base state.*/
	void Destroy();
	/**Recreates object after a call to destroy, using pre-existing VkRenderPassCreateInfo that we have stored.*/
	void Recreate();

	const VkRenderPass& vkHandle() { return handle; }
	const VkRenderPassCreateInfo& CreateInfo() { return createInfo; }
	/** This is the object you will need to retrieve inside renderpasses, when calling vkCmdBeginRenderpass. */
	const VkRenderPassBeginInfo& BeginInfo() { return beginInfo; }

private:
	LogicalDevice* logicalDevice;
	VkRenderPass handle;
	VkRenderPassCreateInfo createInfo;
	VkRenderPassBeginInfo beginInfo;
	std::vector<VkClearValue> clearValues;
};

