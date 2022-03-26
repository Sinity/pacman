#pragma once
#include <vector>
#include <ecs/ecs.h>
#include <SFML/System.hpp>

struct MovementComponent : public Component<MovementComponent> {
	explicit MovementComponent(float mass = 1.0f) : mass(mass) { }

	void addTemporalForce(sf::Vector2f force) {
		resultantForce += force;
	}

	void addPersistentForce(sf::Vector2f force) {
		persistentForces.push_back(force);
	}

    //forces to be applied every physics update
	std::vector<sf::Vector2f> persistentForces;

    //for temporal forces that will work only during single update(impulses)
    sf::Vector2f resultantForce = {0, 0};

    float mass = 1.0f;

    //object's velocity depends on it, should be kept in sync with PositionComponent outside physics code
    //set to PositionComponent on start or object will be ridicously fast and 'dissapear'
	sf::Vector2f oldPosition = {0, 0};
};
