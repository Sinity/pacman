#pragma once
#include <ecs/ecs.h>
#include <SFML/Window.hpp>

struct KeyReleased : public Event<KeyReleased> {
	KeyReleased(sf::Event::KeyEvent key) :
			key(key) {
	}

	sf::Event::KeyEvent key;
};

