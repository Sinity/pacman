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
	void createFlappy();
	void createPipeSegment(float positionX);
    void createScoreCounter();
	void createCamera();
    void setupInitialPipes();
    void loadResources();
    void createHole(sf::Vector2f position, float width, float height);
    void createPipe(sf::Vector2f position, float width, float height);
    void flapFlappyWings();
	void init();
	void cleanup();

    EntityID flappy;
    EntityID scoreCounter;
	std::vector<EntityID> holes;
    std::vector<EntityID> pipes; //even indices -> upper segments, odd indices -> lower segments

    int score = 0;
    EntityID currentlyCollidingHole;
    float lastPipePosition = 0.f;
    float pipeSpacing = 0.f;

    std::shared_ptr<sf::Texture> flappyTex = std::make_shared<sf::Texture>();
    std::shared_ptr<sf::Texture> pipeTex = std::make_shared<sf::Texture>();
    sf::Font counterFont;

    ECS& engine;   
    sf::RenderWindow& window;
};
