#include "Desk.hpp"

Desk::Desk(Shader* shader)
{
	const string displayFile = "desk/desk.dae";

	Geometry::init(displayFile, shader);
}

Desk::~Desk()
{
	
}
