#pragma once
#include <ecs/ecs.h>
#include <SFML/Graphics.hpp>

struct GUITextComponent : public Component<GUITextComponent> {
    sf::Text text;
};
