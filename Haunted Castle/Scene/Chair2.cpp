#include "Chair2.hpp"

Chair2::Chair2(Shader* shader)
{
	const string displayFile = "woodchair2/woodchair2.dae";

	Geometry::init(displayFile, shader);
}

Chair2::~Chair2()
{
	
}
