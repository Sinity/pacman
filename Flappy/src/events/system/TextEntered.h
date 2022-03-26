#pragma once
#include <ecs/ecs.h>
#include <SFML/Window.hpp>

struct TextEntered : Event<TextEntered> {
	TextEntered(sf::Event::TextEvent text) :
			text(text) {
	}

	sf::Event::TextEvent text;
};

