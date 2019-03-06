#pragma once
#include <glm\glm.hpp>
#include <vulkan\vulkan.h>
#include <vector>

struct VertexTightData {
	glm::vec2 i0_pos;
	glm::vec3 i1_color;
};

struct VertexTightDataInfo
{
	std::vector<int> locations;
	std::vector<VkFormat> formats;
	std::vector<int> offsets;
	int itemCount;

	void resize(int itemCount)
	{
		this->itemCount = itemCount;
		locations.resize(itemCount);
		formats.resize(itemCount);
		offsets.resize(itemCount);
	}

	void setInfo(int idx, int loc, VkFormat format, int offset)
	{
		locations[idx] = loc;
		formats[idx] = format;
		offsets[idx] = offset;
	}

	VkVertexInputBindingDescription getBindingDescription(int dataItemSize) 
	{
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = dataItemSize;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() 
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(itemCount);
		for (int i = 0; i < itemCount; i++)
		{
			attributeDescriptions[i].binding = 0;
			attributeDescriptions[i].location = locations[i];
			attributeDescriptions[i].format = formats[i];
			attributeDescriptions[i].offset = offsets[i];
		}
		return attributeDescriptions;
	}
};

class Polygon
{
public:
	Polygon();
	~Polygon();
};

