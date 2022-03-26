#pragma once
#include <SFML/System.hpp>
#include <ecs/ecs.h>

struct PositionComponent : public Component<PositionComponent> {
	explicit PositionComponent(float x = 0, float y = 0) : position(x,y) { }

    //Point(vector with fixed orgin at (0, 0) with this Entity's position in world
	sf::Vector2f position;
};
