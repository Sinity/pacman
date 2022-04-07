#pragma once
#include <vector>
#include <ecs/ecs.h>
#include <SFML/System.hpp>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct MovementComponent : public Component<MovementComponent> {
	MovementComponent(Direction direction = Direction::LEFT, float speed = 0.0f) : direction(direction), speed(speed) { }
    
    Direction direction;
    float speed;
};
