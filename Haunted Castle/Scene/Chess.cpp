#include "Chess.hpp"

Chess::Chess(Shader* shader)
{
	const string displayFile = "chess/chess.dae";

	Geometry::init(displayFile, shader);
}

Chess::~Chess()
{
	
}

