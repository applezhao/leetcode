#pragma once
#include<vulkan/vulkan.hpp>
class RenderPassBase
{
public:
	RenderPassBase();
	~RenderPassBase();

public:
	void init()
	{

	}
	void recordCommand()
	{

	}
private:
	void __createGraphicsPipeline()
	{

	}
	vk::UniquePipeline graphicsPipeline;
	vk::PipelineLayout pipelineLayout;
	std::vector<vk::ImageView> textureViews;
	std::vector<vk::Sampler> textureSamplers;
	std::vector<vk::Buffer> vbos;
	std::vector<vk::Buffer> ibos;
	std::vector<vk::Buffer> ubos;

	vk::ShaderModule vertexShaderModule;
	vk::ShaderModule fragmentShaderModule;
};

