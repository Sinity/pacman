#pragma once
#include <ecs/ecs.h>
#include <SFML/Window.hpp>

struct MouseButtonPressed : public Event<MouseButtonPressed> {
	MouseButtonPressed(sf::Event::MouseButtonEvent button) :
			button(button) {
	}

	sf::Event::MouseButtonEvent button;
};

