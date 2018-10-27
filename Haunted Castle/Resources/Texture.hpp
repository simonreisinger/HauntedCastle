#pragma once

#include <string>
#include <GL/glew.h>

#include "Const.hpp"

namespace cgue
{
	class Texture
	{
	public:
		Texture(const std::string& modelDir, const std::string& path);
		~Texture();

		void bind(int unit);

	private:
		GLuint handle;
	};
}