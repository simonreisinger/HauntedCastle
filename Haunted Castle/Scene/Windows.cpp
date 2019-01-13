#include "Windows.hpp"

Windows::Windows(Shader* shader)
{
	const string displayFile = "windows/windows.dae";

	Geometry::init(displayFile, shader);
}

Windows::~Windows()
{
	
}
