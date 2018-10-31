#include "Commode.hpp"

Commode::Commode(Shader* shader)
{
	const string displayFile = "commode/commode.dae";

	Geometry::init(displayFile, shader);
}

Commode::~Commode()
{
	
}

