#pragma once
#include <ecs/ecs.h>
#include <SFML/Window.hpp>

struct MouseButtonReleased : public Event<MouseButtonReleased> {
	MouseButtonReleased(sf::Event::MouseButtonEvent button) :
			button(button) {
	}

	sf::Event::MouseButtonEvent button;
};

