#include "SurfaceKHR.h"

#include "Instance.h"
#include "PhysicalDevice.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <iostream>
#include <set>
SurfaceKHR::SurfaceKHR(Instance* instance, void* window, int w, int h):
window(reinterpret_cast<GLFWwindow*>(window)),
instance(instance),
width(w),
height(h)
{
	if (glfwCreateWindowSurface(this->instance->vkHandle(), this->window, nullptr, &surface) != VK_SUCCESS)
	{
		std::cout << "Surfaces are not supported on current physical device!" << std::endl;
	}
	std::cout << "surface is created!" << std::endl;
}


SurfaceKHR::~SurfaceKHR()
{
	vkDestroySurfaceKHR(instance->vkHandle(), surface, nullptr);
}

bool SurfaceKHR::isPresentSupported(PhysicalDevice* physicalDevice)
{
	VkBool32 presentSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice->vkHandle(), physicalDevice->queueFamilyIndex,
		surface, &presentSupport);
	return presentSupport;
}

void SurfaceKHR::updateWindowSize()
{
	glfwGetFramebufferSize(window, &width, &height);
}

