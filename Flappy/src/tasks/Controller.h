#pragma once
#include <ecs/ecs.h>
#include <SFML/Graphics.hpp>
#include "../states/PushdownAutomata.h"

using namespace EECS;

class Controller : public Task<Controller> {
public:
	explicit Controller(ECS& engine);
	void update() override;

private:
    PushdownAutomata states;
    sf::RenderWindow window;
};
