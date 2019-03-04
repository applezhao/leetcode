#pragma once
#include <vulkan\vulkan.h>

class Instance;
class GLFWwindow;
class PhysicalDevice;
class SurfaceKHR
{
public:
	SurfaceKHR(Instance* instance, void* window, int w, int h);
	~SurfaceKHR();

	const VkSurfaceKHR& vkHandle() { return surface; }

	bool isPresentSupported(PhysicalDevice* physicalDevice);
	int getWidth() { return width; }
	int getHeight() { return height; }
private:
	VkSurfaceKHR surface;
	

	GLFWwindow* window;
	Instance* instance;
	int width;
	int height;
};

