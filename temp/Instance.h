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

class Instance
{
public:
    Instance(const VkApplicationInfo* info, bool enableValidation);
    ~Instance();
    
    const VkInstance& vkHandle() const noexcept;
    
private:
    void checkApiVersionSupport(VkApplicationInfo* info);
    void enableValidationLayer();
    bool checkValidationLayerSupported();
    void enableExtensions();

    VkInstance handle;
    VkInstanceCreateInfo createInfo;
    bool enableValidLayer;
};

#endif /* Instance_h */
