#pragma once

#ifndef __SHADER_MANAGER__
#define __SHADER_MANAGER__

#include "Shader.h"
#include <vector>

namespace lemon {

	class ShaderManager
	{
		std::vector<Shader> shaders;
	};

}

#endif // !__SHADER_MANAGER__