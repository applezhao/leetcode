#include "DeviceResource.h"
#include "utils.h"


DeviceResource::DeviceResource(const std::string& appName, const std::string& engineName, int width, int height)
{
	//instance
	instance = vk::su::createInstance(appName, engineName, vk::su::getInstanceExtensions());
	vk::UniqueDebugReportCallbackEXT debugReportCallback = vk::su::createDebugReportCallback(instance);

	//physical device
	std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
	physicalDevice = physicalDevices[0];

	vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

	//surface
	surfaceData = vk::su::SurfaceData(instance, appName, appName, vk::Extent2D(width, height));
	
	//logical device
	std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = vk::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevice, surfaceData.surface);
	device = vk::su::createDevice(physicalDevice, graphicsAndPresentQueueFamilyIndex.first, vk::su::getDeviceExtensions());

	//command pool
	commandPool = vk::su::createCommandPool(device, graphicsAndPresentQueueFamilyIndex.first);
	//command buffers
	commandBuffers = device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1));

	//command queue
	graphicsQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.first, 0);
	presentQueue = device->getQueue(graphicsAndPresentQueueFamilyIndex.second, 0);
}


DeviceResource::~DeviceResource()
{
}
