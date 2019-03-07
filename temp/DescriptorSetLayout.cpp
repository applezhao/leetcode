#include "DescriptorSetLayout.h"
#include "LogicalDevice.h"

#include <iostream>
DescriptorSetLayout::DescriptorSetLayout(LogicalDevice* logicalDevice)
{
	this->logicalDevice = logicalDevice;
}


DescriptorSetLayout::~DescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(logicalDevice->vkHandle() , handle, nullptr);
}

void DescriptorSetLayout::AddDescriptorBinding(const VkDescriptorType& descriptor_type,
	const VkShaderStageFlags& shader_stage, const uint32_t& descriptor_binding_loc)
{
	bindingsData.emplace(descriptor_binding_loc, VkDescriptorSetLayoutBinding{
		descriptor_binding_loc,
		descriptor_type,
		1,
		VkShaderStageFlags(shader_stage),
		nullptr
	});
}

const VkDescriptorSetLayout & DescriptorSetLayout::vkHandle()
{
	if (!ready)
	{
		size_t num_bindings = static_cast<uint32_t>(bindingsData.size());
		std::vector<VkDescriptorSetLayoutBinding> bindings_vec;
		for (const auto& entry : bindingsData) {
			bindings_vec.push_back(VkDescriptorSetLayoutBinding(entry.second));
		}

		VkDescriptorSetLayoutCreateInfo set_layout_create_info = {};// vk_descriptor_set_layout_create_info_base;
		set_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		set_layout_create_info.bindingCount = static_cast<uint32_t>(num_bindings);
		set_layout_create_info.pBindings = bindings_vec.data();

		VkResult result = vkCreateDescriptorSetLayout(logicalDevice->vkHandle(), 
			&set_layout_create_info, nullptr, &handle);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor set layout!");
		}
		else
		{
			std::cout << "descriptor set layout is created " << num_bindings << std::endl;
		}
		ready = true;
	}
	return handle;
}
