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

class BaseApplication {
public:
	BaseApplication(int width, int height) :WIDTH(width), HEIGHT(height) {}
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
private:
	void initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);
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
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		delete classSwapChain;
		delete classLogicalDevice;
		delete classPhysicalDevice;
		delete classSurfaceKHR;
		delete classInstance;
		glfwDestroyWindow(window);
		glfwTerminate();
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
	}
	void __createSwapChain()
	{
		classSwapChain = new SwapChain(classPhysicalDevice, classLogicalDevice, classSurfaceKHR);
		swapChain = classSwapChain->vkHandle();
		swapChainImages = classSwapChain->getImages();
		swapChainImageFormat = classSwapChain->getImageFormat();
		swapChainExtent = classSwapChain->getExtent();
		swapChainImageViews = classSwapChain->getImageViews();
	}
};

#endif /* BaseApplication_h */
