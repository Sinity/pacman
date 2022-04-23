#pragma once
#include <vector>
#include <ecs/ecs.h>
#include <SFML/System.hpp>

enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct MovementComponent : public Component<MovementComponent> {
	MovementComponent(Direction direction = Direction::NONE, float speed = 0.0f) : direction(direction), speed(speed) { }
    
    Direction direction;
    float speed;
};
