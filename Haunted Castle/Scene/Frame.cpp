#include "Frame.hpp"

Frame::Frame(Shader* shader)
{
	const string displayFile = "door/door.dae";

	Geometry::init(displayFile, shader);
}

Frame::~Frame()
{
	
}
