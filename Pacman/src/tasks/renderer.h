#pragma once
#include <ecs/ecs.h>
#include <SFML/Graphics.hpp>

using namespace EECS;

class Renderer : public Task<Renderer> {
public:
	Renderer(ECS& engine, sf::RenderWindow& window);
	void update() override;

private:
    void renderSprites();
    void renderText();

    //mainly for rendering into it
	sf::RenderWindow& window;

    //color of background
	sf::Color fillColor;
};
