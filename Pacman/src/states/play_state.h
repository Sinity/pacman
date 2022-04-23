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

    Entity createPacman(const std::string& configRoot);
    Entity createScoreCounter(float pos);
    void createMaze();
    void createWallSegment(int posX, int posY);
    void createFoodPellet(int posX, int posY);

    Entity pacman;
    std::shared_ptr<sf::Texture> pacmanTex = std::make_shared<sf::Texture>();
    int scoreP1 = 0;
    Entity scoreCounterP1;

    Entity pacman2;
    std::shared_ptr<sf::Texture> pacman2Tex = std::make_shared<sf::Texture>();
    int scoreP2 = 0;
    Entity scoreCounterP2;
    
    sf::Font counterFont;

	std::vector<EntityID> pellets;
    std::vector<EntityID> walls;
    
    ECS& ecs;   
    sf::RenderWindow& window;
};
