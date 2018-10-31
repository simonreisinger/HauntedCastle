#include "Room.hpp"

Room::Room(Shader* shader)
{
	const string displayFile = "room/room.dae";

	Geometry::init(displayFile, shader);
}

Room::~Room()
{
	
}
