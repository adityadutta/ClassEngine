#include "ShaderHandler.h"

std::unique_ptr<ShaderHandler> ShaderHandler::shaderInstance = nullptr;
std::map<std::string, GLuint>  ShaderHandler::programs = std::map<std::string, GLuint>();

ShaderHandler* ShaderHandler::GetInstance()
{
	if (shaderInstance.get() == nullptr)
	{
		shaderInstance.reset(new ShaderHandler);
	}

	return shaderInstance.get();
}

void ShaderHandler::OnDestroy()
{
	if (!programs.empty())
	{
		for (auto entry : programs)
		{
			glDeleteProgram(entry.second);
		}
		programs.clear();
	}
}

void ShaderHandler::CreateProgram(const std::string& shaderName_, const std::string& vertexShaderFilename_,
	const std::string& fragmentShaderFilename_)
{
	std::string vertexShaderCode = ReadShader(vertexShaderFilename_);
	std::string fragmentShaderCode = ReadShader(fragmentShaderFilename_);

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, shaderName_);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, shaderName_);

	GLint linkResult = 0;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	if (!linkResult)
	{
		GLint logLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> programLog(logLength);
		glGetShaderInfoLog(program, logLength, NULL, &programLog[0]);
		std::string logString(programLog.begin(), programLog.end());
		Debug::Error("Shader program failed to link: " + shaderName_ + " . Error: \n" + logString, __FILE__, __LINE__);
		return;
	}

	programs[shaderName_] = program;
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

const GLuint ShaderHandler::GetShader(const std::string& shaderName_)
{
	if (programs.find(shaderName_) != programs.end())
	{
		return programs[shaderName_];
	}

	return 0;
}

ShaderHandler::ShaderHandler()
{

}

ShaderHandler::~ShaderHandler()
{
	OnDestroy();
}

std::string ShaderHandler::ReadShader(const std::string filename_)
{
	std::string shaderCode;
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);
	try
	{
		file.open(filename_.c_str());
		std::stringstream tmpStream;
		tmpStream << file.rdbuf();
		file.close();
		shaderCode = tmpStream.str();
	}
	catch (std::ifstream::failure error_)
	{
		Debug::Error("Failed to read shader file: " + filename_, __FILE__, __LINE__);
		return "";
	}

	return shaderCode;
}

GLuint ShaderHandler::CreateShader(GLenum shaderType_, const std::string& source_, const std::string& shaderName_)
{
	GLint compileResult = 0;
	GLuint shader = glCreateShader(shaderType_);
	const char* shaderCode = source_.c_str();
	const int shaderSize = source_.size();
	glShaderSource(shader, 1, &shaderCode, &shaderSize);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (!compileResult)
	{
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> shaderLog(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, &shaderLog[0]);
		std::string logString(shaderLog.begin(), shaderLog.end());
		Debug::Error("Shader failed to compile: " + shaderName_ + " . Error: \n" + logString, __FILE__, __LINE__);
		return 0;
	}

	return shader;
}
