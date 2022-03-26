#include "PlayState.h"
#include <random>
#include <ctime>

//tasks
#include "../tasks/AttachedCameraController.h"

//events
#include "../events/system/ApplicationClosed.h"
#include "../events/system/MouseButtonPressed.h"
#include "../events/CollisionEvent.h"
#include "../events/system/KeyPressed.h"

//components
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/MovementComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/GraphicsComponent.h"
#include "../components/GUITextComponent.h"

PlayState::PlayState(ECS& engine, sf::RenderWindow& window) : engine(engine), Receives(engine.events), window(window) {
	init();
}

void PlayState::init() {
	loadResources();
	createFlappy();
	createScoreCounter();
	createCamera();
	setupInitialPipes();
}

bool PlayState::receive(ApplicationClosedEvent&) {
	engine.logger.info("Application close request received in Controller, stopping engine...");
	cleanup();
	engine.stop();

    return true;
}

bool PlayState::receive(MouseButtonPressed& buttonPress) {
    const auto LPM = 0;
	if(buttonPress.button.button == LPM) {
        flapFlappyWings();
	}

    return true;
}

bool PlayState::receive(KeyPressed& keystroke) {
    if(keystroke.key.code == sf::Keyboard::R) {
		cleanup();
		init();
	}

    return true;
}

bool PlayState::receive(CollisionEvent& collision) {
    auto colliding = collision.firstBody == flappy ? collision.secondBody : collision.firstBody;
    if(colliding != collision.secondBody) {
        collision.minimumTranslationVector = -collision.minimumTranslationVector;
    }

    //colliding with score trigger
    if(std::find(begin(holes), end(holes), colliding) != end(holes)) { 
        //check if it isn't hole that we're already in contact still 
        if(currentlyCollidingHole != colliding) { 
            currentlyCollidingHole = colliding;

            //update player score
            score++; 
            if(auto text = engine.components.getComponent<GUITextComponent>(scoreCounter)) {
                text->text.setString(std::to_string(score)); 
            }

            //create new pipe
            createPipeSegment(lastPipePosition + pipeSpacing);
            lastPipePosition += pipeSpacing;

            //remove oldest pipe, if it can't be seen now.
            auto leftScreenBoundary = window.getView().getCenter().x - window.getView().getSize().x / 2;
            auto oldestPipePosition = engine.components.getComponent<PositionComponent>(pipes[0])->position.x;
            if(oldestPipePosition < leftScreenBoundary) {
                engine.entities.deleteEntity(pipes[0]);    
                engine.entities.deleteEntity(pipes[1]);
                pipes.erase(begin(pipes));
                pipes.erase(begin(pipes));
            }
        }
    }     
    //colliding with obstacle
    else { 
        auto flappyPos = engine.components.getComponent<PositionComponent>(flappy);
        auto flappyMovement = engine.components.getComponent<MovementComponent>(flappy);

        if(flappyPos) {
            flappyPos->position += collision.minimumTranslationVector;
        }
        if(flappyPos && flappyMovement) {
            flappyMovement->oldPosition.x = flappyPos->position.x; //stop forwad motion
        }
    }

    return true;
}

void PlayState::createFlappy() {
	flappy = engine.entities.addEntity().getID();

    //create components
	auto flappyPosition = engine.components.addComponent<PositionComponent>(flappy);
	auto flappySize = engine.components.addComponent<SizeComponent>(flappy);
	auto flappyMovement = engine.components.addComponent<MovementComponent>(flappy);
    auto flappyCollision = engine.components.addComponent<CollisionComponent>(flappy);
	auto flappyAppearance = engine.components.addComponent<GraphicsComponent>(flappy);

    //setup position and size
	flappyPosition->position.x = engine.config.get<float>("gameplay.flappy.position.x");
	flappyPosition->position.y = engine.config.get<float>("gameplay.flappy.position.y");
	flappySize->width = engine.config.get<float>("gameplay.flappy.size.width");
	flappySize->height = engine.config.get<float>("gameplay.flappy.size.height");

    //setup physics
	flappyMovement->oldPosition = flappyPosition->position;
	flappyMovement->addPersistentForce({0, engine.config.get<float>("gameplay.flappy.forces.gravity")});
	flappyMovement->addTemporalForce({engine.config.get<float>("gameplay.flappy.forces.forwardConst"), 0});

    flappyCollision->emitEvent = true;
	flappyAppearance->texture = flappyTex;
}

void PlayState::createPipeSegment(float positionX) {
    //get parameters from configuration
    auto segmentWidth = engine.config.get<float>("gameplay.pipeSegmentWidth");
    auto holeHeight = engine.config.get<float>("gameplay.hole.height");
    auto holeUpperMargin = engine.config.get<float>("gameplay.hole.upperMargin");
    auto holeLowerMargin = engine.config.get<float>("gameplay.hole.lowerMargin");
    auto invisibleSkyHeight = engine.config.get<float>("gameplay.invisibleSkyHeight");
    auto floorHeight = engine.config.get<float>("gameplay.floorHeight");

    //calculate screen boundaries
	sf::View view = window.getView();
	auto screenUpperBoundary = view.getCenter().y - view.getSize().y / 2;
	auto screenLowerBoundary = view.getCenter().y + view.getSize().y / 2;

    //calculate range of possible hole positions
	auto holeMinPosition = screenUpperBoundary + holeUpperMargin;
	auto holeMaxPosition = screenLowerBoundary - floorHeight - holeHeight - holeLowerMargin;

    //roll random hole position
	auto holePositionsDistribution = std::uniform_real_distribution<float>{holeMinPosition, holeMaxPosition};
    static auto dice = std::bind(holePositionsDistribution, std::default_random_engine((unsigned int)time(0)));
	auto holeYPosition = dice();

    createHole({positionX, holeYPosition}, segmentWidth, holeHeight);

    //create upper pipe
    float upperPipeTop = screenUpperBoundary - invisibleSkyHeight;
    float upperPipeHeight = holeYPosition - (screenUpperBoundary - invisibleSkyHeight);
    createPipe({positionX, upperPipeTop}, segmentWidth, upperPipeHeight);

    //create lower pipe
    float lowerPipeTop = holeYPosition + holeHeight;
    float lowerPipeHeight = (screenLowerBoundary - floorHeight) - (holeYPosition + holeHeight);
    createPipe({positionX, lowerPipeTop}, segmentWidth, lowerPipeHeight);
}

void PlayState::createScoreCounter() {
    scoreCounter = engine.entities.addEntity().getID();
    auto counterAppearance = engine.components.addComponent<GUITextComponent>(scoreCounter);
    auto counterPosition = engine.components.addComponent<PositionComponent>(scoreCounter);

    counterFont.loadFromFile(engine.config.get("gameplay.files.font"));
    counterAppearance->text.setFont(counterFont);
    counterAppearance->text.setOutlineColor(sf::Color::White);
    counterAppearance->text.setFillColor(sf::Color::White);
    counterAppearance->text.setCharacterSize(engine.config.get<int>("gameplay.score.fontSize"));
    counterAppearance->text.setString("0");

    counterPosition->position.x = window.getDefaultView().getCenter().x;
}

void PlayState::createCamera() {
    auto cameraFollowXAxis = engine.config.get("camera.follow.x", true);
    auto cameraFollowYAxis = engine.config.get("camera.follow.y", false);
    auto cameraOffset = sf::Vector2f{engine.config.get<float>("camera.offset.x"),
                                    engine.config.get<float>("camera.offset.y")};
	engine.tasks.addTask<AttachedCameraController>(window, flappy, cameraOffset, cameraFollowXAxis, cameraFollowYAxis);
}

void PlayState::setupInitialPipes() {
    pipeSpacing = engine.config.get<float>("gameplay.spaceBetweenPipes");
    auto initEmptySpace = engine.config.get<float>("gameplay.initialEmptySpace");
    auto screenBoundary = window.getView().getSize().x;
	for(auto pos = initEmptySpace; pos <= initEmptySpace + screenBoundary; pos += pipeSpacing) {
		createPipeSegment(pos);
        lastPipePosition = pos;
    }                                
}

void PlayState::loadResources() {
	pipeTex->loadFromFile(engine.config.get("gameplay.files.pipeTexture"));
	flappyTex->loadFromFile(engine.config.get("gameplay.files.flappyTexture"));
}

void PlayState::createHole(sf::Vector2f position, float width, float height) {
	auto hole = engine.entities.addEntity().getID();
	holes.push_back(hole);

	auto holePosComponent = engine.components.addComponent<PositionComponent>(hole);
	holePosComponent->position = {position.x, position.y};

	auto holeSize = engine.components.addComponent<SizeComponent>(hole);
	holeSize->width = width;
	holeSize->height = height;

    engine.components.addComponent<CollisionComponent>(hole);
}

void PlayState::createPipe(sf::Vector2f position, float width, float height) {
	auto pipe = engine.entities.addEntity().getID();
    pipes.push_back(pipe);

    auto pipePosition = engine.components.addComponent<PositionComponent>(pipe);
    pipePosition->position = position;

    auto pipeSize = engine.components.addComponent<SizeComponent>(pipe);
    pipeSize->width = width;
    pipeSize->height = height;

    auto pipeCollision = engine.components.addComponent<CollisionComponent>(pipe);
    pipeCollision->isStatic = true;

    auto pipeAppearance = engine.components.addComponent<GraphicsComponent>(pipe);
    pipeAppearance->texture = pipeTex;
}

void PlayState::flapFlappyWings() {
    auto flappyMovement = engine.components.getComponent<MovementComponent>(flappy);
    auto flappyPosition = engine.components.getComponent<PositionComponent>(flappy);
    flappyMovement->oldPosition.y = flappyPosition->position.y; //flappy universe logic here; velocity = 0
    flappyMovement->addTemporalForce({0, engine.config.get<float>("gameplay.flappy.forces.lift")});
}

void PlayState::cleanup() {
	engine.entities.deleteEntity(flappy);
	flappy = 0;
	engine.entities.deleteEntity(scoreCounter);
	scoreCounter = 0;
	score = 0;
	
	for (auto hole : holes) {
		engine.entities.deleteEntity(hole);
	}
	holes.clear();
	
	for (auto pipe : pipes) {
		engine.entities.deleteEntity(pipe);
	}
	pipes.clear();

	engine.tasks.deleteTask<AttachedCameraController>();
}