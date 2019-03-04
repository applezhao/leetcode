//
//  Instance.h
//  vklearnxcode
//
//  Created by Yanli Zhao on 03/03/2019.
//  Copyright © 2019 Yanli Zhao. All rights reserved.
//

#ifndef Instance_h
#define Instance_h

#include <vulkan/vulkan.h>
#include <vector>
class Instance
{
public:
	Instance(const VkApplicationInfo* info, bool enableValidation);
	~Instance();

	const VkInstance& vkHandle() const noexcept;

	bool isValidationEnabled() {
		return enableValidLayer;
	}

private:
	void checkApiVersionSupport(VkApplicationInfo* info);
	void enableValidationLayer();
	bool checkValidationLayerSupported();
	void enableExtensions();

	VkInstance handle;
	VkInstanceCreateInfo createInfo;
	bool enableValidLayer;

	std::vector<const char*> validationLayers;
	std::vector<const char*> extensions;
};

#endif /* Instance_h */
