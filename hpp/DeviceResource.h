#pragma once
#include<vulkan/vulkan.hpp>

#include <iostream>
#include <map>
#include <vector>

class RenderPassBase;
class DeviceResource
{
public:
	DeviceResource(const std::string& appName, const std::string& engineName, int width, int height);
	~DeviceResource();

public:
	vk::UniqueInstance instance;
	vk::PhysicalDevice physicalDevice;
	vk::su::SurfaceData surfaceData;
	vk::UniqueDevice device;

	vk::UniqueCommandPool commandPool;
	std::vector<vk::UniqueCommandBuffer> commandBuffers;
	

	vk::Queue graphicsQueue;
	vk::Queue presentQueue;

	std::vector<vk::Framebuffer> frameBuffers;
	std::vector<vk::ImageView> depthStencilImageViews;

	std::vector<vk::Semaphore> imageAcquireSemaphores;
	std::vector<vk::Semaphore> presentationSemaphores;
	std::vector<vk::Fence> perFrameAcquisitionFences;
	std::vector<vk::Fence> perFrameCommandBufferFences;

	
};

