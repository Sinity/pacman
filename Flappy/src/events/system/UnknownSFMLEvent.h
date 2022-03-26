#pragma once
#include <ecs/ecs.h>
#include <SFML/Window.hpp>

struct UnknownSFMLEvent : Event<UnknownSFMLEvent> {
	UnknownSFMLEvent(sf::Event event) :
			event(event) {
	}

	sf::Event event;
};
