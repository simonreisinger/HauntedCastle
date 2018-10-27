#pragma once

#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace cgue
{
	class Shader
	{
		// Constructor with two imput variables
		// eine Variable fuer den Code des Vertexshaders
		// eine Variable fuer den Code des Fragmentshaders
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		// Destructor
		~Shader();

		void useShader() const;
		GLuint programHandle;


	private:
		// Wird fuer jeden der beiden Shader einmal aufgerufen
		void loadShader(const std::string& shader, GLenum shaderType, GLuint& handle /*<--OUT*/);
		// haengt die beiden Shader-Objekts zu einem Programm zusammenhaengen
		void link();

		GLuint vertexHandle;
		GLuint fragmentHandle;

	};
}