#include "RenderPassBase.h"
#include "DeviceResource.h"
#include "utils.h"
using namespace std;
int main()
{
	string appName = "testvkhpp";
	string engineName = "vulkanHpp";
	HWND window = vk::su::initializeWindow(appName, engineName, 500, 500);
	return 0;
}