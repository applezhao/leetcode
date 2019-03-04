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
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}
