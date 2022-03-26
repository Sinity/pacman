#pragma once
#include <ecs/ecs.h>
#include <SFML/Window.hpp>

struct KeyPressed : public Event<KeyPressed> {
	KeyPressed(sf::Event::KeyEvent key) :
			key(key) {
	}

	sf::Event::KeyEvent key;
};

