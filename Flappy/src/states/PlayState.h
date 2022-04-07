#pragma once
#include "State.h"
#include <ecs/ecs.h>
#include <SFML/Graphics.hpp>

using namespace EECS;
struct ApplicationClosedEvent;
struct MouseButtonPressed;
struct KeyPressed;
struct CollisionEvent;

class PlayState : public State, Receives<PlayState, ApplicationClosedEvent, MouseButtonPressed, KeyPressed, CollisionEvent> {
public:
	PlayState(ECS& engine, sf::RenderWindow& window);
	bool receive(ApplicationClosedEvent& closeRequest);
    bool receive(MouseButtonPressed& buttonPress);
    bool receive(KeyPressed& buttonPress);
    bool receive(CollisionEvent& collision);

private:
    void init();
    void cleanup();
    void flapFlappyWings();

    void loadResources();
    void createFlappy();
    void createScoreCounter();
    void createCamera();
    void setupInitialPipes();
	void createPipeSegment(float positionX);
    void createHole(sf::Vector2f position, float width, float height);
    void createPipe(sf::Vector2f position, float width, float height);

    Entity flappy;
    Entity scoreCounter;
	std::vector<EntityID> holes;
    std::vector<Entity> pipes; //even indices -> upper segments, odd indices -> lower segments

    int score = 0;
    EntityID currentlyCollidingHoleID;
    float lastPipePosition = 0.f;
    float pipeSpacing = 0.f;

    std::shared_ptr<sf::Texture> flappyTex = std::make_shared<sf::Texture>();
    std::shared_ptr<sf::Texture> pipeTex = std::make_shared<sf::Texture>();
    sf::Font counterFont;

    ECS& ecs;   
    sf::RenderWindow& window;
};
