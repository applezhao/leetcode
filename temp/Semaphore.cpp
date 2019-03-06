#include "Semaphore.h"
#include "LogicalDevice.h"
#include<iostream>
Semaphore::Semaphore(LogicalDevice * logicalDevice)
{
	this->logicalDevice = logicalDevice;
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	
	if (vkCreateSemaphore(logicalDevice->vkHandle(), &semaphoreInfo, nullptr, &handle) != VK_SUCCESS)
		throw std::runtime_error("failed to create semaphore!");
	std::cout << "semaphore is created!" << std::endl;
}

Semaphore::~Semaphore()
{
	vkDestroySemaphore(logicalDevice->vkHandle(), handle, nullptr);
}
