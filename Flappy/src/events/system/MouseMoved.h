#pragma once
#include <ecs/ecs.h>
#include <SFML/Window.hpp>

struct MouseMoved : public Event<MouseMoved> {
	MouseMoved(sf::Event::MouseMoveEvent where, sf::Vector2i delta, sf::Vector2f worldDelta) :
			where(where),
			delta(delta),
			worldDelta(worldDelta) {
	}

	sf::Event::MouseMoveEvent where;
	sf::Vector2i delta;
	sf::Vector2f worldDelta;
};

