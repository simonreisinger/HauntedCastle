#include "Wardrobe.hpp"

Wardrobe::Wardrobe(Shader* shader)
{
	const string displayFile = "wardrobe/wardrobe.dae";

	Geometry::init(displayFile, shader);
}

Wardrobe::~Wardrobe()
{
	
}
