#include "Chair1.hpp"

Chair1::Chair1(Shader* shader)
{
	const string displayFile = "woodchair1/woodchair1.dae";

	Geometry::init(displayFile, shader);
}

Chair1::~Chair1()
{
	
}

