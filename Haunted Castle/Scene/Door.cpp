#include "Door.hpp"

Door::Door(Shader* shader)
{
	const string displayFile = "door/door.dae";

	Geometry::init(displayFile, shader);
}

Door::~Door()
{
	
}
