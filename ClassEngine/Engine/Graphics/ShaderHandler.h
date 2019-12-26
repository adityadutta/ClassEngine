#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <sstream>
#include <map>
#include <vector>
#include <memory>
#include <glew.h>

#include "../Core/Debug.h"

class ShaderHandler
{
public:
	ShaderHandler(const ShaderHandler&) = delete; //Copy constructor
	ShaderHandler(ShaderHandler&&) = delete;//move constructor
	ShaderHandler operator=(const ShaderHandler&) = delete;
	ShaderHandler operator=(ShaderHandler&&) = delete;

	static ShaderHandler* GetInstance();
	void OnDestroy();
	void CreateProgram(const std::string &shaderName_, const std::string &vertexShaderFilename_, const std::string &fragmentShaderFilename_);

	static const GLuint GetShader(const std::string &shaderName_);
private:
	ShaderHandler();
	~ShaderHandler();

	static std::unique_ptr<ShaderHandler> shaderInstance;
	friend std::default_delete<ShaderHandler>;

	static std::map<std::string, GLuint> programs;

	std::string ReadShader(const std::string filename_);
	GLuint CreateShader(GLenum shaderType_, const std::string &source_, const std::string &shaderName_);
};
#endif //!SHADERHANDLER_H
