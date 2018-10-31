#include "Knight2.hpp"

Knight2::Knight2(Shader* shader)
{
	const string displayFile = "knight2/knight2.dae";

	Geometry::init(displayFile, shader);
}

Knight2::~Knight2()
{
	
}
