#include "ShaderModule.h"

#include "LogicalDevice.h"

#include <fstream>
#include <iostream>
ShaderModule::ShaderModule(LogicalDevice* logicalDevice, const std::string& filename, const VkShaderStageFlagBits & stages)
{
	this->logicalDevice = logicalDevice;
	this->stages = stages;
	std::vector<char> code = __readfile(filename);

	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if (vkCreateShaderModule(logicalDevice->vkHandle(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
	std::cout << "shader module is created!" << std::endl;
	pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineInfo.stage = stages;
	pipelineInfo.module = shaderModule;
	pipelineInfo.pName = "main";
}

ShaderModule::~ShaderModule()
{
	vkDestroyShaderModule(logicalDevice->vkHandle(), shaderModule, nullptr);
}

std::vector<char> ShaderModule::__readfile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}
