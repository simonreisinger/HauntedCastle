#include "Shader.hpp"

#include <iostream>
#include <fstream>

using namespace cgue;
using namespace std;

Shader::Shader(const string& vertexShader, const string& fragmentShader)
	: programHandle(0), vertexHandle(0), fragmentHandle(0)
{
	programHandle = glCreateProgram();

	if (programHandle == 0)
	{
		cout << "ERROR: Could not create Program." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	loadShader(vertexShader, GL_VERTEX_SHADER, vertexHandle /* <-- OUT */);
	loadShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentHandle /* <-- OUT */);

	link();
}

Shader::Shader(const string& vertexShader, const string& fragmentShader, const string& geometryShader)
	: programHandle(0), vertexHandle(0), fragmentHandle(0)
{
	programHandle = glCreateProgram();

	if (programHandle == 0)
	{
		cout << "ERROR: Could not create Program." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	loadShader(vertexShader, GL_VERTEX_SHADER, vertexHandle /* <-- OUT */);
	loadShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentHandle /* <-- OUT */);
	loadShader(geometryShader, GL_GEOMETRY_SHADER, geometryHandle /* <-- OUT */);
	
	link();
}

Shader::Shader(const string& vertexShader, const string& fragmentShader, const string& geometryShader, const std::string& computeShader)
	: programHandle(0), vertexHandle(0), fragmentHandle(0)
{
	programHandle = glCreateProgram();

	if (programHandle == 0)
	{
		cout << "ERROR: Could not create Program." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	loadShader(vertexShader, GL_VERTEX_SHADER, vertexHandle /* <-- OUT */);
	loadShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentHandle /* <-- OUT */);
	loadShader(geometryShader, GL_GEOMETRY_SHADER, geometryHandle /* <-- OUT */);
	loadShader(computeShader, GL_COMPUTE_SHADER, computeHandle /* <-- OUT */);

	link();
}

Shader::~Shader()
{
	glDeleteProgram(programHandle);
	glDeleteShader(vertexHandle);
	glDeleteShader(fragmentHandle);
	glDeleteShader(geometryHandle);
	glDeleteShader(computeHandle);
}

void Shader::useShader() const
{
	glUseProgram(programHandle);
}

void Shader::loadShader(const string shader, GLenum shaderType, GLuint& handle /*<-OUT*/)
{
	ifstream shaderFile(shader.c_str());

	if (!shaderFile.good())
	{
		cout << "ERROR: Load file '" << shader << "'." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	std::string coder = std::string(
		std::istreambuf_iterator<char>(shaderFile),
		std::istreambuf_iterator<char>());

	shaderFile.close();

	handle = glCreateShader(shaderType);

	if (handle == 0)
	{
		std::cout << "ERROR: Could not create Shader." << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	const char * codePtr = coder.c_str();
	glShaderSource(handle, 1, &codePtr, nullptr);
	glCompileShader(handle);

	GLint succeeded;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &succeeded);
	if (!succeeded)
	{
		GLint logSize;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

		GLchar* msg = new GLchar[logSize];
		glGetShaderInfoLog(handle, logSize, nullptr, msg);

		std::cout << "Msg:" << msg << std::endl;
		system("PAUSE");

		delete[] msg;

		exit(EXIT_FAILURE);
	}
}

void Shader::link()
{
	// combine vertexShader and fragmentShader to one shader program
	glAttachShader(programHandle, vertexHandle);
	glAttachShader(programHandle, fragmentHandle);
	if (geometryHandle != NULL)
	{
		glAttachShader(programHandle, geometryHandle);
	}
	if (computeHandle != NULL)
	{
		glAttachShader(programHandle, computeHandle);
	}

	glLinkProgram(programHandle);

	GLint succeeded;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &succeeded);
	if (!succeeded)
	{
		GLint logSize;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logSize);

		GLchar* msg = new GLchar[logSize];
		glGetProgramInfoLog(programHandle, logSize, nullptr, msg);

		std::cout << "msg:" << msg << std::endl;
		system("PAUSE");

		delete[] msg;

		exit(EXIT_FAILURE);
	}
}