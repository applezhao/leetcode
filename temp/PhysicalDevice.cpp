#include "PhysicalDevice.h"
#include "Instance.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



PhysicalDevice::PhysicalDevice(Instance* instance)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->vkHandle(), &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance->vkHandle(), &deviceCount, devices.data());

	for (auto& d : devices)
	{
		if (__isDeviceSuitable(d))
		{
			physicalDevice = d;
			break;
		}
			
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}
const VkPhysicalDevice& PhysicalDevice::vkHandle()
{
	return physicalDevice;
}
PhysicalDevice::~PhysicalDevice()
{
	
}
//
//uint32_t PhysicalDevice::GetMemoryTypeIdx(const uint32_t& type_bitfield,
//	const VkMemoryPropertyFlags& property_flags, VkBool32* memory_type_found = nullptr)
//{
//	
//}
//uint32_t PhysicalDevice::GetQueueFamilyIndex(const VkQueueFlagBits& bitfield)
//{
//	
//}
bool PhysicalDevice::__isDeviceSuitable(VkPhysicalDevice device)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags&VK_QUEUE_GRAPHICS_BIT)
		{
			queueFamilyIndex = i;
			return true;
		}
		i++;
	}
	return false;
}
//void PhysicalDevice::getAttributes()
//{
//}
//void PhysicalDevice::retrieveQueueFamilyProperties()
//{
//}
