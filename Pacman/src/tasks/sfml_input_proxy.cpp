#include "sfml_input_proxy.h"
#include "../events/system_events.h"

SFMLInputProxy::SFMLInputProxy(ECS& engine, sf::RenderWindow& window) :
		Task(engine),
		window(window),
		lastMousePosition(sf::Mouse::getPosition()) {
	window.setKeyRepeatEnabled(false);
}

void SFMLInputProxy::update() {
	auto currentEvent = sf::Event();
	while(window.pollEvent(currentEvent)) {
		switch(currentEvent.type) {
			case sf::Event::Closed:
				ecs.events.emplace<ApplicationClosed>();
		        break;
			case sf::Event::KeyPressed:
				ecs.events.emplace<KeyPressed>(currentEvent.key);
		        break;
			case sf::Event::KeyReleased:
				ecs.events.emplace<KeyReleased>(currentEvent.key);
		        break;
			case sf::Event::TextEntered:
				ecs.events.emplace<TextEntered>(currentEvent.text);
		        break;
			case sf::Event::MouseButtonPressed:
				ecs.events.emplace<MouseButtonPressed>(currentEvent.mouseButton);
		        break;
			case sf::Event::MouseButtonReleased:
				ecs.events.emplace<MouseButtonReleased>(currentEvent.mouseButton);
		        break;
			case sf::Event::MouseMoved: {
				auto currMousePos = sf::Vector2i{currentEvent.mouseMove.x, currentEvent.mouseMove.y};
				auto delta = currMousePos - lastMousePosition;
				auto worldDelta = window.mapPixelToCoords(currMousePos) - window.mapPixelToCoords(lastMousePosition);
				ecs.events.emplace<MouseMoved>(currentEvent.mouseMove, delta, worldDelta);
				lastMousePosition = currMousePos;
				break;
			}
			case sf::Event::MouseWheelMoved:
				ecs.events.emplace<MouseWheelMoved>(currentEvent.mouseWheel);
		        break;
			default:
				ecs.events.emplace<UnknownSFMLEvent>(currentEvent);
		}
	}
}
