#include "Knight1.hpp"

Knight1::Knight1(Shader* shader)
{
	const string displayFile = "knight1/knight1.dae";

	Geometry::init(displayFile, shader);
}

Knight1::~Knight1()
{
	
}
