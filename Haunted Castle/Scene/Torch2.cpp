#include "Torch2.hpp"

Torch2::Torch2(Shader* shader)
{
	const string displayFile = "torch2/torch2.dae";

	Geometry::init(displayFile, shader);
}

Torch2::~Torch2()
{
	
}
