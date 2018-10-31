#include "Torch1.hpp"

Torch1::Torch1(Shader* shader)
{
	const string displayFile = "torch1/torch1.dae";

	Geometry::init(displayFile, shader);
}

Torch1::~Torch1()
{
	
}
