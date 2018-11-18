#include "Frame.hpp"

Frame::Frame(Shader* shader)
{
	//const string displayFile = "frame/frame1.dae";
	const string displayFile = "frame/frame2.dae";

	Geometry::init(displayFile, shader);
}

Frame::~Frame()
{
	
}
