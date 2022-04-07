#include "PlayState.h"
#include <random>
#include <ctime>
#include "../tasks/AttachedCameraController.h"
#include "../events/system/ApplicationClosed.h"
#include "../events/system/MouseButtonPressed.h"
#include "../events/CollisionEvent.h"
#include "../events/system/KeyPressed.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/MovementComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/GraphicsComponent.h"
#include "../components/GUITextComponent.h"

PlayState::PlayState(ECS& ecs, sf::RenderWindow& window) : ecs(ecs), Receives(ecs.events), window(window) {
	init();
}

void PlayState::init() {
	loadResources();
	createFlappy();
	createScoreCounter();
	createCamera();
	setupInitialPipes();
}

void PlayState::cleanup() {
    flappy.deleteEntity();
    scoreCounter.deleteEntity();
    score = 0;

    for (auto&& hole : holes)
        ecs.entities.deleteEntity(hole);
    holes.clear();

    for (auto&& pipe : pipes)
        ecs.entities.deleteEntity(pipe);
    pipes.clear();

    ecs.tasks.deleteTask<AttachedCameraController>();
}

void PlayState::flapFlappyWings() {
    auto flappyMovement = flappy.component<MovementComponent>();
    auto flappyPosition = flappy.component<PositionComponent>();
    flappyMovement->oldPosition.y = flappyPosition->position.y; //flappy universe logic here; velocity = 0
    flappyMovement->addTemporalForce({ 0, ecs.config.get<float>("gameplay.flappy.forces.lift") });
}

bool PlayState::receive(ApplicationClosedEvent&) {
	ecs.logger.info("Application close request received in Controller, stopping ecs...");
	cleanup();
	ecs.stop();

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
    auto collidingID = collision.firstBody != flappy.getID() ? collision.firstBody : collision.secondBody;
    if (collidingID != collision.secondBody)
        collision.minimumTranslationVector = -collision.minimumTranslationVector;

    //colliding with score trigger
    if (std::find(begin(holes), end(holes), collidingID) != end(holes)) {
        //check if it isn't hole that we're already in contact still 
        if(currentlyCollidingHoleID != collidingID) {
            currentlyCollidingHoleID = collidingID;

            //update player score
            score++; 
            if (auto text = ecs.components.getComponent<GUITextComponent>(scoreCounter))
                text->text.setString(std::to_string(score)); 

            //create new pipe
            createPipeSegment(lastPipePosition + pipeSpacing);
            lastPipePosition += pipeSpacing;

            //remove oldest pipe, if it can't be seen now.
            auto leftScreenBoundary = window.getView().getCenter().x - window.getView().getSize().x / 2;
            auto oldestPipePosition = ecs.components.getComponent<PositionComponent>(pipes[0])->position.x;
            if (oldestPipePosition < leftScreenBoundary) {
                ecs.entities.deleteEntity(pipes[0]);    
                ecs.entities.deleteEntity(pipes[1]);
                pipes.erase(begin(pipes));
                pipes.erase(begin(pipes));
            }
        }
    }

    //colliding with obstacle
    else { 
        auto flappyPos = flappy.component<PositionComponent>();
        auto flappyMovement = flappy.component<MovementComponent>();

        if (flappyPos)
            flappyPos->position += collision.minimumTranslationVector;

        if (flappyPos && flappyMovement)
            flappyMovement->oldPosition.x = flappyPos->position.x; //stop forwad motion
    }

    return true;
}

void PlayState::loadResources() {
    pipeTex->loadFromFile(ecs.config.get("gameplay.files.pipeTexture"));
    flappyTex->loadFromFile(ecs.config.get("gameplay.files.flappyTexture"));
}

void PlayState::createFlappy() {
	flappy = ecs.entities.addEntity();

    //create components
	auto flappyPosition = flappy.addComponent<PositionComponent>();
	auto flappySize = flappy.addComponent<SizeComponent>();
	auto flappyMovement = flappy.addComponent<MovementComponent>();
    auto flappyCollision = flappy.addComponent<CollisionComponent>();
	auto flappyAppearance = flappy.addComponent<GraphicsComponent>();

    //setup position and size
	flappyPosition->position.x = ecs.config.get<float>("gameplay.flappy.position.x");
	flappyPosition->position.y = ecs.config.get<float>("gameplay.flappy.position.y");
	flappySize->width = ecs.config.get<float>("gameplay.flappy.size.width");
	flappySize->height = ecs.config.get<float>("gameplay.flappy.size.height");

    //setup physics
	flappyMovement->oldPosition = flappyPosition->position;
	flappyMovement->addPersistentForce({0, ecs.config.get<float>("gameplay.flappy.forces.gravity")});
	flappyMovement->addTemporalForce({ecs.config.get<float>("gameplay.flappy.forces.forwardConst"), 0});

    flappyCollision->emitEvent = true;
	flappyAppearance->texture = flappyTex;
}

void PlayState::createScoreCounter() {
    counterFont.loadFromFile(ecs.config.get("gameplay.files.font"));

    scoreCounter = ecs.entities.addEntity();
    scoreCounter.addComponent<GUITextComponent>();
    scoreCounter.component<GUITextComponent>()->text.setFont(counterFont);
    scoreCounter.component<GUITextComponent>()->text.setOutlineColor(sf::Color::White);
    scoreCounter.component<GUITextComponent>()->text.setFillColor(sf::Color::White);
    scoreCounter.component<GUITextComponent>()->text.setCharacterSize(ecs.config.get<int>("gameplay.score.fontSize"));
    scoreCounter.component<GUITextComponent>()->text.setString("0");

    scoreCounter.addComponent<PositionComponent>()->position.x = window.getDefaultView().getCenter().x;
}

void PlayState::createCamera() {
	auto cameraFollowXAxis = ecs.config.get("camera.follow.x", true);
	auto cameraFollowYAxis = ecs.config.get("camera.follow.y", false);
	auto cameraOffset = sf::Vector2f{ ecs.config.get<float>("camera.offset.x"),
									ecs.config.get<float>("camera.offset.y") };
	ecs.tasks.addTask<AttachedCameraController>(window, flappy, cameraOffset, cameraFollowXAxis, cameraFollowYAxis);
}

void PlayState::setupInitialPipes() {
	pipeSpacing = ecs.config.get<float>("gameplay.spaceBetweenPipes");
	auto initEmptySpace = ecs.config.get<float>("gameplay.initialEmptySpace");
	auto screenBoundary = window.getView().getSize().x;
	for (auto pos = initEmptySpace; pos <= initEmptySpace + screenBoundary; pos += pipeSpacing) {
		createPipeSegment(pos);
		lastPipePosition = pos;
	}
}

void PlayState::createPipeSegment(float positionX) {
    //get parameters from configuration
    auto segmentWidth = ecs.config.get<float>("gameplay.pipeSegmentWidth");
    auto holeHeight = ecs.config.get<float>("gameplay.hole.height");
    auto holeUpperMargin = ecs.config.get<float>("gameplay.hole.upperMargin");
    auto holeLowerMargin = ecs.config.get<float>("gameplay.hole.lowerMargin");
    auto invisibleSkyHeight = ecs.config.get<float>("gameplay.invisibleSkyHeight");
    auto floorHeight = ecs.config.get<float>("gameplay.floorHeight");

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


void PlayState::createHole(sf::Vector2f position, float width, float height) {
    auto hole = ecs.entities.addEntity();
	holes.push_back(hole);

	hole.addComponent<PositionComponent>()->position = {position.x, position.y};

	auto holeSize = hole.addComponent<SizeComponent>();
	holeSize->width = width;
	holeSize->height = height;

    hole.addComponent<CollisionComponent>();
}

void PlayState::createPipe(sf::Vector2f position, float width, float height) {
	auto pipe = ecs.entities.addEntity();
    pipes.push_back(pipe);

    pipe.addComponent<PositionComponent>()->position = position;

    auto pipeSize = pipe.addComponent<SizeComponent>();
    pipeSize->width = width;
    pipeSize->height = height;

    pipe.addComponent<CollisionComponent>()->isStatic = true;
    pipe.addComponent<GraphicsComponent>()->texture = pipeTex;
}
