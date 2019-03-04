#pragma once
#include <vulkan\vulkan.h>
#include <vector>
class Instance;
class PhysicalDevice
{
public:
	PhysicalDevice(Instance* instance);
	const VkPhysicalDevice& vkHandle();
	~PhysicalDevice();

	//uint32_t GetMemoryTypeIdx(const uint32_t& type_bitfield, 
	//	const VkMemoryPropertyFlags& property_flags, VkBool32* memory_type_found = nullptr);
	//uint32_t GetQueueFamilyIndex(const VkQueueFlagBits& bitfield);
	//void getAttributes();
	//void retrieveQueueFamilyProperties();

	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceProperties Properties;
	VkPhysicalDeviceFeatures Features;
	VkPhysicalDeviceMemoryProperties MemoryProperties;
	//VkPhysicalDeviceSubgroupProperties SubgroupProperties;
	std::vector<VkQueueFamilyProperties> queueFamilyProperties;
	uint32_t queueFamilyIndex;
private:
	bool __isDeviceSuitable(VkPhysicalDevice device);
};

