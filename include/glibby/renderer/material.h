#pragma once

namespace glibby
{
	struct PipelineInfo
	{
		const char* vertShaderPath;
		const char* fragShaderPath;
	};

	class Material
	{
	public:
		Material();
		virtual ~Material();
	};
}