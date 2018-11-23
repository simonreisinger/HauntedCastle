#include "Frame.hpp"

Frame::Frame(Shader* shader)
{
	const string displayFile = "frame/frame.dae";

	Geometry::init(displayFile, shader);
}

Frame::~Frame()
{
	
}
