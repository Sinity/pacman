#pragma once
#include <ecs/ecs.h>
#include <SFML/Window.hpp>

using namespace EECS;

struct ApplicationClosed : public Event<ApplicationClosed> {
};

struct KeyPressed : public Event<KeyPressed> {
    KeyPressed(sf::Event::KeyEvent key) :
        key(key) {
    }

    sf::Event::KeyEvent key;
};


struct KeyReleased : public Event<KeyReleased> {
    KeyReleased(sf::Event::KeyEvent key) :
        key(key) {
    }

    sf::Event::KeyEvent key;
};

struct MouseButtonPressed : public Event<MouseButtonPressed> {
    MouseButtonPressed(sf::Event::MouseButtonEvent button) :
        button(button) {
    }

    sf::Event::MouseButtonEvent button;
};

struct MouseButtonReleased : public Event<MouseButtonReleased> {
    MouseButtonReleased(sf::Event::MouseButtonEvent button) :
        button(button) {
    }

    sf::Event::MouseButtonEvent button;
};

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

struct MouseWheelMoved : public Event<MouseWheelMoved> {
    MouseWheelMoved(sf::Event::MouseWheelEvent event) :
        ticks(event.delta),
        x(event.x),
        y(event.y) {
    }

    int ticks;
    int x;
    int y;
};

struct TextEntered : Event<TextEntered> {
    TextEntered(sf::Event::TextEvent text) :
        text(text) {
    }

    sf::Event::TextEvent text;
};

struct UnknownSFMLEvent : Event<UnknownSFMLEvent> {
    UnknownSFMLEvent(sf::Event event) :
        event(event) {
    }

    sf::Event event;
};