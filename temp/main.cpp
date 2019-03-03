//
//  main.cpp
//  vklearnxcode
//
//  Created by Yanli Zhao on 28/02/2019.
//  Copyright © 2019 Yanli Zhao. All rights reserved.
//



#include "BaseApplication.h"
#include <iostream>

int main() {
    
    BaseApplication app(800, 600);
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() <<std::endl;
        return EXIT_FAILURE;
    }
//    glfwInit();
//
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
//
//    uint32_t extensionCount = 0;
//    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//
//    std::cout << extensionCount << " extensions supported" << std::endl;
//
//    glm::mat4 matrix;
//    glm::vec4 vec;
//    auto test = matrix * vec;
//
//    while(!glfwWindowShouldClose(window)) {
//        glfwPollEvents();
//    }
//
//    glfwDestroyWindow(window);
//
//    glfwTerminate();
    
    return 0;
}
