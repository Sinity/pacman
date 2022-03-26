#pragma once
#include <ecs/ecs.h>
#include <SFML/Window.hpp>

struct ApplicationClosedEvent : public Event<ApplicationClosedEvent> {
};

