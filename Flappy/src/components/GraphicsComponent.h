#pragma once
#include <ecs/ecs.h>
#include <SFML/Graphics.hpp>

struct GraphicsComponent : public Component<GraphicsComponent> {
	explicit GraphicsComponent(int plane = 0) : plane(plane) {}

	int plane = 0;  //Planes are drawn from high to low. INT_MAX is drawn first, INT_MIN last.
                    //Drawing order within a plane is undefined.
                    
    std::shared_ptr<sf::Texture> texture = nullptr; //for sprite
};
