#include "Coordinatesystem.hpp"

Coordinatesystem::Coordinatesystem(Shader* shader)
{
	const string displayFile = "Coordinatesystem/Coordinatesystem.dae";

	Geometry::init(displayFile, shader);
}

Coordinatesystem::~Coordinatesystem()
{
	
}

