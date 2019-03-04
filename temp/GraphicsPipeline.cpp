#include "GraphicsPipeline.h"


GraphicsPipeline::~GraphicsPipeline()
{
}

GraphicsPipelineInfo::GraphicsPipelineInfo(VkExtent2D swapExtent)
{
	VertexInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInfo.vertexBindingDescriptionCount = 0;
	VertexInfo.pVertexBindingDescriptions = nullptr; // Optional
	VertexInfo.vertexAttributeDescriptionCount = 0;
	VertexInfo.pVertexAttributeDescriptions = nullptr; // Optional

	AssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	AssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	AssemblyInfo.primitiveRestartEnable = VK_FALSE;

	TesselationInfo = {};

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapExtent.width;
	viewport.height = (float)swapExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapExtent;

	ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	ViewportInfo.viewportCount = 1;
	ViewportInfo.pViewports = &viewport;
	ViewportInfo.scissorCount = 1;
	ViewportInfo.pScissors = &scissor;

	RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	RasterizationInfo.depthClampEnable = VK_FALSE;
	RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	RasterizationInfo.lineWidth = 1.0f;
	RasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	RasterizationInfo.depthBiasEnable = VK_FALSE;
	RasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
	RasterizationInfo.depthBiasClamp = 0.0f; // Optional
	RasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

	MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	MultisampleInfo.sampleShadingEnable = VK_FALSE;
	MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	MultisampleInfo.minSampleShading = 1.0f; // Optional
	MultisampleInfo.pSampleMask = nullptr; // Optional
	MultisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
	MultisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

	DepthStencilInfo = {};

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ColorBlendInfo.logicOpEnable = VK_FALSE;
	ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
	ColorBlendInfo.attachmentCount = 1;
	ColorBlendInfo.pAttachments = &colorBlendAttachment;
	ColorBlendInfo.blendConstants[0] = 0.0f; // Optional
	ColorBlendInfo.blendConstants[1] = 0.0f; // Optional
	ColorBlendInfo.blendConstants[2] = 0.0f; // Optional
	ColorBlendInfo.blendConstants[3] = 0.0f; // Optional

	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	DynamicStateInfo.dynamicStateCount = 2;
	DynamicStateInfo.pDynamicStates = dynamicStates;
}

VkGraphicsPipelineCreateInfo GraphicsPipelineInfo::GetPipelineCreateInfo() const {
	VkGraphicsPipelineCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	create_info.stageCount = 2;
	//create_info.pStages = shaderStages;
	create_info.pVertexInputState = &VertexInfo;
	create_info.pInputAssemblyState = &AssemblyInfo;
	create_info.pTessellationState = nullptr;//&TesselationInfo;
	create_info.pViewportState = &ViewportInfo;
	create_info.pRasterizationState = &RasterizationInfo;
	create_info.pMultisampleState = &MultisampleInfo;
	create_info.pDepthStencilState = nullptr;// &DepthStencilInfo;
	create_info.pColorBlendState = &ColorBlendInfo;
	create_info.pDynamicState = nullptr;//&DynamicStateInfo;
	return create_info;
}
