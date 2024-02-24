#include "Shader.h"
#include "FileUtils.h"
#include "Logger.h"
#include "utils.h"
#include <boost/algorithm/string.hpp> 
#include "loggerUtils.h"
#include "lemonConventions.h"

namespace lemon {

	int Shader::UBOBindingCounter = 0;
	std::map<std::string, GLint> Shader::DeclaredUBOs = std::map<std::string, GLint>();

	Shader::Shader() {
		sourcePath = conventions::UNSPECIFIED_SHADER_NAME;
	}

	void Shader::init() {
		programID = glCreateProgram();
	}

	Shader::~Shader() {
		glUseProgram(0);
		glDeleteProgram(programID);
	}

	std::string Shader::shaderEnumToString(GLenum type) {
		switch (type) {
			case (GL_VERTEX_SHADER):
				return "vertex shader";
			case (GL_FRAGMENT_SHADER):
				return "fragment shader";
			default:
				return "unknown";
		}
	}

	void Shader::addShader(const std::string& sourcePath, const GLenum type) {
		if (!fileExists(sourcePath)) {
			std::cerr << "Source path " << sourcePath << " does not contain any shaders";
		}

		if (shaders.find(type) != shaders.end()) {
			std::cout << "Shader type: " << shaderEnumToString(type) << " aready exists in shader program" << sourcePath << " \n";
		}

		this->sourcePath = sourcePath;
		std::string source = readSource(sourcePath);
		const char* code = source.c_str();

		ShaderInfo shaderID = glCreateShader(type);
		glShaderSource(shaderID, 1, &code, NULL);
		glCompileShader(shaderID);
		checkCompilation(shaderID, source);

		glAttachShader(programID, shaderID);
		shaders[type] = { shaderID };
	}

	// taken from https://www.khronos.org/opengl/wiki/Example/GLSL_Shader_Compile_Error_Testing
	void Shader::checkCompilation(const ShaderInfo shaderID, const std::string& source) {
		Logger& logger = Logger::getInstance();

		GLint isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

			logger.FATAL("Shader <" + sourcePath + "> failed to compile, error message: " + std::string(errorLog.begin(), errorLog.end()));
		
			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(shaderID); // Don't leak the shader.
			exit(EXIT_FAILURE);
		}

	}

	// TODO parse uniforms for shaders that dont come from files
	std::string Shader::readSource(const std::string& source) {
		std::string shaderCode;
		std::ifstream file(source);

		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << source << std::endl;
			exit(EXIT_FAILURE); // maybe in the future we want to not exit if cannot open the file
		}

		Logger& logger = Logger::getInstance();
		std::string line;
		bool inUBO = false;
		logger.INFO("Detected Uniforms for shader " + source + " :");
		while (std::getline(file, line)) {

			shaderCode += line + "\n";
			if (line.empty())
				continue;

			boost::algorithm::trim(line); // this trims in place
			if (!startsWith(line, "//") && line.find("uniform") != std::string::npos) {
				std::istringstream iss(line);
				std::vector<std::string> tokens{
					std::istream_iterator<std::string>{iss},
						std::istream_iterator<std::string>{}
				};

				// I'm assuming that uniform declarations are one liners, and that every uniform declaration that is not a one linear
				// is a UBO declaration.

				auto it = std::find(tokens.begin(), tokens.end(), "uniform");
				int iterateAmmount = 0;
				if (it != tokens.end()) {
					bool hasSemicolon = (std::find(line.begin(), line.end(), ';') != line.end());

					// uniform mat4 matrix;
					if (tokens.size() >= 3 && hasSemicolon)
						iterateAmmount = 2;

					// uniform matrixUBO
					else if (tokens.size() >= 2 && !hasSemicolon)
						iterateAmmount = 1;

					if (iterateAmmount > 0 && std::next(it, iterateAmmount) != tokens.end()) {
						std::string uniformName = *std::next(it, iterateAmmount);
						// Remove trailing semicolon, if any
						uniformName.erase(std::remove(uniformName.begin(), uniformName.end(), ';'), uniformName.end());

						if (iterateAmmount == 1) {
							if (DeclaredUBOs.find(uniformName) == DeclaredUBOs.end()) {
								DeclaredUBOs.emplace(uniformName, UBOBindingCounter++);
							}

							UBOs[uniformName] = { 0, DeclaredUBOs[uniformName] };
						}
						else if (iterateAmmount == 2) {
							uniforms[uniformName] = { -1 };
						}
						else {
							logger.WARN("Uniform declaration not recognized: " + line);
							continue;
						}

						std::string utype = iterateAmmount < 2 ? "UBO        " : "Uniform    ";
						logger.INFO("\t" + utype + ": " + uniformName);
						//std::cout << "uniform: " << uniformName << std::endl;
					}
				}
			}
		}

		return shaderCode;
	}

	void Shader::checkLinkage() {

		GLint isLinked = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);

			// The programID is useless now. So delete it.
			glDeleteProgram(programID);

			// Provide the infolog in whatever manner you deem best.
			// Exit with failure.
#ifdef DEBUG
			std::string logString(infoLog.begin(), infoLog.end());
			std::cerr << "[DEBUG] Shader linkage unsuccessful for " + programID << std::endl;
			std::cerr << "[DEBUG] " << logString;
#endif
			exit(EXIT_FAILURE);
		}

#ifdef DEBUG
		std::cout << "[DEBUG] Shader linkage successful for " + programID << std::endl;
#endif
	}

	void Shader::linkAndCreate() {
		glLinkProgram(programID);

		checkLinkage();

		auto& logger = Logger::getInstance();

		for (auto& [uniformName, id] : uniforms) {
			uniforms[uniformName] = glGetUniformLocation(programID, uniformName.c_str());


			if (uniforms[uniformName] < 0) {
				logger.WARN("Uniform \"" + uniformName + "\" not found!");
			}
			logger.TRACE("Uniform: " + uniformName + " location: " + std::to_string(uniforms[uniformName]));

		}

		for (auto& [UBOName, id] : UBOs) {
			UBOs[UBOName].index = glGetUniformBlockIndex(programID, UBOName.c_str());

			if (UBOs[UBOName].index <= 0) {
				logger.WARN("UBO \"" + UBOName + "\" not found!");
			}
			glUniformBlockBinding(programID, UBOs[UBOName].index, UBOs[UBOName].bindingPoint);

			std::string formattedInfo = formatUBOInfo(UBOName, UBOs[UBOName]);
			logger.TRACE("Declared UBO: " + formattedInfo);
		}

		for (auto& [type, shaderID] : shaders) {
			glDetachShader(programID, shaderID);
			glDeleteShader(shaderID);
		}
	}

	void Shader::bind() {
		glUseProgram(programID);
	}

	void Shader::unbind() {
		glUseProgram(0);
	}
}
