#pragma once
#include "State.h"
#include <ecs/ecs.h>
#include <SFML/Graphics.hpp>

using namespace EECS;
struct ApplicationClosed;
struct MouseButtonPressed;
struct KeyPressed;
struct CollisionEvent;

class PlayState : public State, Receives<PlayState, ApplicationClosed, MouseButtonPressed, KeyPressed, CollisionEvent> {
public:
	PlayState(ECS& engine, sf::RenderWindow& window);
	bool receive(ApplicationClosed& closeRequest);
    bool receive(MouseButtonPressed& buttonPress);
    bool receive(KeyPressed& buttonPress);
    bool receive(CollisionEvent& collision);

private:
    void init();
    void cleanup();

    void loadResources();
    void createPacman();
    void createScoreCounter();
    void createMaze();

    int score = 0;
    Entity scoreCounter;

    Entity pacman;
	std::vector<EntityID> pellets;
    
    std::shared_ptr<sf::Texture> pacmanTex = std::make_shared<sf::Texture>();
    sf::Font counterFont;

    ECS& ecs;   
    sf::RenderWindow& window;
};
