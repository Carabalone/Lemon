#pragma once
#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <set>

namespace lemon {

	typedef GLuint ShaderInfo;
	typedef GLint UniformInfo;
	
	struct UBOInfo {
		GLint bindingPoint;
		GLint index;
	};

	class Shader
	{
	public:
		Shader();
		void init();
		~Shader();
		void addShader(const std::string& sourcePath, const GLenum type);
		std::string shaderEnumToString(GLenum type);
		void linkAndCreate();
		void bind();
		void unbind();
	private:
		std::map<GLenum, ShaderInfo> shaders;
		std::map<std::string, UniformInfo> uniforms;
		std::map<std::string, UBOInfo> UBOs;
		static std::map<std::string, GLint> DeclaredUBOs;
		static int UBOBindingCounter;
		GLuint programID;
		std::string sourcePath;

		std::string readSource(const std::string& source);
		void checkCompilation(const ShaderInfo shaderID, const std::string& source);
		void checkLinkage();
	};

}

#endif // !__SHADER_H__

