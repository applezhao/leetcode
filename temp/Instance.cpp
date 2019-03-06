//
//  Instance.cpp
//  vklearnxcode
//
//  Created by Yanli Zhao on 03/03/2019.
//  Copyright © 2019 Yanli Zhao. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Instance.h"

Instance::Instance(const VkApplicationInfo* info, bool enableValidation) :
	enableValidLayer(enableValidation),
	createInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO}
{
	VkApplicationInfo our_info = *info;

	if (!glfwVulkanSupported()) {
		std::cerr << "Vulkan is not supported on the current hardware!";
		throw std::runtime_error("Vulkan not supported!");
	}
	validationLayers.clear();
	validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");

	checkApiVersionSupport(&our_info);

	createInfo.pApplicationInfo = &our_info;
	createInfo.enabledLayerCount = 0;
	if(enableValidLayer)
	{
	    enableValidationLayer();
	}
	enableExtensions();
	if (vkCreateInstance(&createInfo, nullptr, &handle) != VK_SUCCESS)
		std::cerr << "failed to create instance!" << std::endl;

	std::cout << "instance is created!" << std::endl;
}

Instance::~Instance() {
	vkDestroyInstance(handle, nullptr);
}

const VkInstance& Instance::vkHandle() const noexcept {
	return handle;
}

void Instance::checkApiVersionSupport(VkApplicationInfo* info) {
	info->apiVersion = VK_MAKE_VERSION(1, 0, 0);
}

bool Instance::checkValidationLayerSupported()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	for (const auto& layerProperties : availableLayers)
	{
		std::cout << "layer:" << layerProperties.layerName << std::endl;
	}

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

void Instance::enableValidationLayer()
{
	if (checkValidationLayerSupported())
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		std::cout << "enable validation layer" << std::endl;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		std::cout << "no validation layer" << std::endl;
	}
}

void Instance::enableExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	//std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (enableValidLayer) {
		extensions.push_back("VK_EXT_debug_utils");
	}
	//for (auto& ii : extensions)
	//	std::cout << "exten:" << ii << std::endl;
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
}


