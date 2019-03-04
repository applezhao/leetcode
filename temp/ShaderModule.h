#pragma once
#include <vulkan\vulkan.h>
#include <vector>
class LogicalDevice;
class ShaderModule
{
public:
	ShaderModule(LogicalDevice* logicalDevice, const std::string& filename, const VkShaderStageFlagBits& stages);
	~ShaderModule();

	const VkShaderModule& vkHandle() { return shaderModule; }
	const VkShaderStageFlagBits& StageBits() { return stages; }
	const VkShaderModuleCreateInfo& CreateInfo() { return createInfo; }
	const VkPipelineShaderStageCreateInfo& PipelineInfo() { return pipelineInfo; }

private:
	LogicalDevice* logicalDevice;
	VkShaderStageFlagBits stages{ VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM };
	VkPipelineShaderStageCreateInfo pipelineInfo{};
	VkShaderModuleCreateInfo createInfo{};
	VkShaderModule shaderModule{ VK_NULL_HANDLE };

private:
	std::vector<char> __readfile(const std::string& filename);
};

