#include "Commode.hpp"

Commode::Commode(Shader* shader)
{
	//const string displayFile = "commode/commode.dae";
	const string displayFile = "chestOfDrawers/chestOfDrawers.dae";

	Geometry::init(displayFile, shader);
}

Commode::~Commode()
{
	
}

