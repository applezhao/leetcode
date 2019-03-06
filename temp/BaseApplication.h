//
//  BaseApplication.h
//  vklearnxcode
//
//  Created by Yanli Zhao on 03/03/2019.
//  Copyright © 2019 Yanli Zhao. All rights reserved.
//

#ifndef BaseApplication_h
#define BaseApplication_h

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>



#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>

#include "Instance.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "SurfaceKHR.h"
#include "SwapChain.h"
#include "ShaderModule.h"
#include "PipelineLayout.h"
#include "RenderPass.h"
#include "GraphicsPipeline.h"
#include "FrameBuffer.h"
#include "CommandPool.h"
#include "Semaphore.h"
#include "Fence.h"

#include "Polygon.h"
#include "Buffer.h"
const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<VertexTightData> vertices = {
	{ { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
	{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
	{ { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } }
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};

class BaseApplication {
public:
	BaseApplication(int width, int height) :WIDTH(width), HEIGHT(height), classCommandPool(nullptr){}
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
private:
	GLFWwindow* window;
	int WIDTH;
	int HEIGHT;

	VkApplicationInfo appInfo;

	Instance* classInstance;
	VkInstance instance;

	PhysicalDevice* classPhysicalDevice;
	VkPhysicalDevice physicalDevice;

	LogicalDevice* classLogicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	SurfaceKHR* classSurfaceKHR;
	VkSurfaceKHR surface;

	SwapChain* classSwapChain;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	RenderPass* classRenderPass;
	VkRenderPass renderPass;

	PipelineLayout* classPipelineLayout;
	VkPipelineLayout pipelineLayout;

	GraphicsPipeline* classGraphicsPipeline;
	VkPipeline graphicsPipeline;

	std::vector<FrameBuffer*> classFrameBuffers;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	CommandPool* classCommandPool;
	VkCommandPool commandPool;

	std::vector<Semaphore*> classImageAvailableSemaphores;
	std::vector<Semaphore*> classRenderFinishedSemaphores;
	std::vector<Fence*> classInFlightFences;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	Buffer* classVertexBuffer;
	VkBuffer vertexBuffer;
	MemoryAlloc* classVertexBufferMemory;
	VkDeviceMemory vertexBufferMemory;

	Buffer* classIndexBuffer;
	VkBuffer indexBuffer;
	MemoryAlloc* classIndexBufferMemory;
	VkDeviceMemory indexBufferMemory;

	size_t currentFrame;
private:
	void initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);
		currentFrame = 0;
	}
	void initVulkan() 
	{
		__createInstance();
		__createSurface();
		__pickPhysicalDevice();
		if (classSurfaceKHR->isPresentSupported(classPhysicalDevice) == false)
			std::cout << "present is not supported!!!" << std::endl;
		__createLogicalDevice();
		__createSwapChain();

		__createRenderPass();
		__createGraphicsPipeline();
		__createFrameBuffers();
		__createCommandPool();

		__createVertexBuffer();
		__createIndexBuffer();

		__createCommandBuffers();
		__createSemaphore();
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			drawFrame();
		}
		vkDeviceWaitIdle(device);
	}

	void cleanup()
	{
		__cleanupSwapchain();
		delete classIndexBuffer;
		delete classIndexBufferMemory;
		delete classVertexBuffer;
		delete classVertexBufferMemory;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			delete classImageAvailableSemaphores[i];
			delete classRenderFinishedSemaphores[i];
			delete classInFlightFences[i];
		}
		
		delete classCommandPool;
		delete classLogicalDevice;
		delete classPhysicalDevice;
		delete classSurfaceKHR;
		delete classInstance;
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void drawFrame() 
	{
		vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
		

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(device, swapChain,
			std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) //update swapchain as resized
		{
			__recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			std::cerr << "failed to acquire swap chain image!" << std::endl; 
		}

			
		std::cout << "acquire next image khr " << imageIndex << std::endl;
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &classCommandPool->commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(device, 1, &inFlightFences[currentFrame]);
		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		//presentation
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional
		result = vkQueuePresentKHR(presentQueue, &presentInfo);
		
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) 
		{
			__recreateSwapChain();
		}
		else if (result != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void __recreateSwapChain()
	{
		vkDeviceWaitIdle(device);
		__cleanupSwapchain();
		__createSwapChain();

		__createRenderPass();
		__createGraphicsPipeline();
		__createFrameBuffers();

		__createCommandBuffers();
	}

	void __cleanupSwapchain()
	{
		for (auto& framebuffer : swapChainFramebuffers)
		{
			if(framebuffer)
				delete framebuffer;
		}
		if (classCommandPool)
			classCommandPool->cleanCommandBuffers();

		if(classGraphicsPipeline)
			delete classGraphicsPipeline;
		if(classPipelineLayout)
			delete classPipelineLayout;
		if(classRenderPass)
			delete classRenderPass;
		if(classRenderPass)
			delete classSwapChain;
	}

	void __createSemaphore()
	{
		classImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		classRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		classInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			classImageAvailableSemaphores[i] = new Semaphore(classLogicalDevice);
			classRenderFinishedSemaphores[i] = new Semaphore(classLogicalDevice);
			classInFlightFences[i] = new Fence(classLogicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
			imageAvailableSemaphores[i] = classImageAvailableSemaphores[i]->vkHandle();
			renderFinishedSemaphores[i] = classRenderFinishedSemaphores[i]->vkHandle();
			inFlightFences[i] = classInFlightFences[i]->vkHandle();
		}
	}

	void __createInstance()
	{
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan window";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		classInstance = new Instance(&appInfo, true);
		instance = classInstance->vkHandle();
	}

	void __createSurface()
	{
		classSurfaceKHR = new SurfaceKHR(classInstance, window, WIDTH, HEIGHT);
		surface = classSurfaceKHR->vkHandle();
	}

	void __pickPhysicalDevice() 
	{
		classPhysicalDevice = new PhysicalDevice(classInstance);
		physicalDevice = classPhysicalDevice->vkHandle();
	}
	void __createLogicalDevice()
	{
		classLogicalDevice = new LogicalDevice(classPhysicalDevice, classInstance->isValidationEnabled());
		device = classLogicalDevice->vkHandle();
		graphicsQueue = classLogicalDevice->graphicsQueue;
		presentQueue = classLogicalDevice->presentQueue;
	}
	void __createSwapChain()
	{
		classSwapChain = new SwapChain(classPhysicalDevice, classLogicalDevice, classSurfaceKHR);
		swapChain = classSwapChain->vkHandle();
		classSwapChain->createImageViews();
		swapChainImages = classSwapChain->getImages();
		swapChainImageFormat = classSwapChain->getImageFormat();
		swapChainExtent = classSwapChain->getExtent();
		swapChainImageViews = classSwapChain->getImageViews();
	}

	void __createRenderPass()
	{
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		//layout location=0 out vec4 outColor
		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		//subpass depedency
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		classRenderPass = new RenderPass(classLogicalDevice, renderPassInfo);
		renderPass = classRenderPass->vkHandle();
	}


	void __createGraphicsPipeline()
	{
		classPipelineLayout = new PipelineLayout(classLogicalDevice);
		pipelineLayout = classPipelineLayout->vkHandle();

		std::vector<ShaderModule*> classShaderModules(2);
		classShaderModules[0] = new ShaderModule(classLogicalDevice, "C:\\workspace\\vulkan\\vulkanlearn\\vulkanlearn\\shader\\vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
		classShaderModules[1] = new ShaderModule(classLogicalDevice, "C:\\workspace\\vulkan\\vulkanlearn\\vulkanlearn\\shader\\frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
		VkPipelineShaderStageCreateInfo shaderStages[] = { classShaderModules[0]->PipelineInfo(),
			classShaderModules[1]->PipelineInfo() };

		//vertexinput info
		VertexTightDataInfo vDataInfo;
		vDataInfo.resize(2);
		vDataInfo.setInfo(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexTightData, i0_pos));
		vDataInfo.setInfo(1, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexTightData, i1_color));
		
		auto vertexBindingDesc = vDataInfo.getBindingDescription(sizeof(VertexTightData));
		auto vertexAttributeDesc = vDataInfo.getAttributeDescriptions();

		GraphicsPipelineInfo structGraphicsPipelineInfo(swapChainExtent);
		structGraphicsPipelineInfo.VertexInfo.vertexBindingDescriptionCount = 1;
		structGraphicsPipelineInfo.VertexInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDesc.size());
		structGraphicsPipelineInfo.VertexInfo.pVertexBindingDescriptions = &vertexBindingDesc;
		structGraphicsPipelineInfo.VertexInfo.pVertexAttributeDescriptions = vertexAttributeDesc.data();
		VkGraphicsPipelineCreateInfo pipelineInfo = structGraphicsPipelineInfo.GetPipelineCreateInfo();

		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		classGraphicsPipeline = new GraphicsPipeline(classLogicalDevice, pipelineInfo);
		graphicsPipeline = classGraphicsPipeline->vkHandle();

		delete classShaderModules[0];
		delete classShaderModules[1];
	}

	void __createFrameBuffers()
	{
		swapChainFramebuffers.resize(swapChainImageViews.size());
		classFrameBuffers.resize(swapChainImageViews.size());
		for (size_t i = 0; i < swapChainImageViews.size(); i++) 
		{
			VkImageView attachments[] = {
				swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			classFrameBuffers[i] = new FrameBuffer(classLogicalDevice, framebufferInfo);
			swapChainFramebuffers[i] = classFrameBuffers[i]->vkHandle();
		}
	}

	void __createCommandPool()
	{
		if (classCommandPool == nullptr)
		{
			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = classPhysicalDevice->queueFamilyIndex;
			classCommandPool = new CommandPool(classLogicalDevice, poolInfo);
			commandPool = classCommandPool->vkHandle();
		}
	}

	void __createVertexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
		__createBuffer(bufferSize, (void*)vertices.data(), classVertexBuffer, 
			classVertexBufferMemory, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		vertexBuffer = classVertexBuffer->vkHandle();
		vertexBufferMemory = classVertexBufferMemory->vkHandle();
	}

	void __createIndexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
		__createBuffer(bufferSize, (void*)indices.data(), classIndexBuffer, 
			classIndexBufferMemory, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
		indexBuffer = classIndexBuffer->vkHandle();
		indexBufferMemory = classIndexBufferMemory->vkHandle();
	}

	void __createBuffer(int bufferSize, void* data, Buffer*& buffer, MemoryAlloc*& memory, VkBufferUsageFlags usage)
	{
		Buffer* classStagingBuffer = new Buffer(classLogicalDevice, bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
		MemoryAlloc* classStagingBufferMemory = new MemoryAlloc(classPhysicalDevice, classLogicalDevice, classStagingBuffer,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		classStagingBufferMemory->bindBufferMemory();
		classStagingBufferMemory->copyDataFromMemoryToGpu(data, bufferSize);

		buffer = new Buffer(classLogicalDevice, bufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_SHARING_MODE_EXCLUSIVE);

		memory = new MemoryAlloc(classPhysicalDevice, classLogicalDevice, buffer,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		memory->bindBufferMemory();
		VkCommandBuffer copyCommandBuffer = buffer->copyDataFromStageBuffer(classStagingBuffer, bufferSize, classCommandPool);
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &copyCommandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);
		vkFreeCommandBuffers(device, commandPool, 1, &copyCommandBuffer);

		delete classStagingBuffer;
		delete classStagingBufferMemory;
	}

	void __createCommandBuffers()
	{
		classCommandPool->AllocateCmdBuffers(swapChainFramebuffers.size(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		for (size_t i = 0; i < classCommandPool->commandBuffers.size(); i++)
		{
			classCommandPool->StartSingleCmdBuffer(i);

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapChainExtent;

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(classCommandPool->commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(classCommandPool->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

			VkBuffer vertexBuffers[] = { classVertexBuffer->vkHandle() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(classCommandPool->commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(classCommandPool->commandBuffers[i], classIndexBuffer->vkHandle(), 0, VK_INDEX_TYPE_UINT16);

			//vkCmdDraw(classCommandPool->commandBuffers[i], 3, 1, 0, 0);
			vkCmdDrawIndexed(classCommandPool->commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
			vkCmdEndRenderPass(classCommandPool->commandBuffers[i]);

			if (vkEndCommandBuffer(classCommandPool->commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}
};

#endif /* BaseApplication_h */
