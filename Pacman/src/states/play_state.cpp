#include "play_state.h"
#include <random>
#include <ctime>
#include "../events/system_events.h"
#include "../events/collision_event.h"
#include "../components/position_component.h"
#include "../components/movement_component.h"
#include "../components/size_component.h"
#include "../components/collision_component.h"
#include "../components/graphics_component.h"
#include "../components/gui_text_component.h"

PlayState::PlayState(ECS& ecs, sf::RenderWindow& window) : ecs(ecs), Receives(ecs.events), window(window) {
	init();
}

void PlayState::init() {
	loadResources();
	createPacman();
	createScoreCounter();
    createMaze();
}

void PlayState::cleanup() {
    pacman.deleteEntity();
    scoreCounter.deleteEntity();
    score = 0;
}

bool PlayState::receive(ApplicationClosed&) {
	ecs.logger.info("Application close request received in Controller, stopping ecs...");
	cleanup();
	ecs.stop();

    return true;
}

bool PlayState::receive(MouseButtonPressed& buttonPress) {
    return true;
}

bool PlayState::receive(KeyPressed& keystroke) {
    if (keystroke.key.code == sf::Keyboard::R) {
        cleanup();
        init();
    } else if (keystroke.key.code == sf::Keyboard::W)
        pacman.component<MovementComponent>()->direction = Direction::UP;
    else if (keystroke.key.code == sf::Keyboard::S)
        pacman.component<MovementComponent>()->direction = Direction::DOWN;
    else if (keystroke.key.code == sf::Keyboard::A)
        pacman.component<MovementComponent>()->direction = Direction::LEFT;
    else if (keystroke.key.code == sf::Keyboard::D)
        pacman.component<MovementComponent>()->direction = Direction::RIGHT;

    return true;
}

bool PlayState::receive(CollisionEvent& collision) {
    // this assumes one of colliding bodies is pacman, for now
    // also unfinished

    auto collidingID = collision.firstBody != pacman.getID() ? collision.firstBody : collision.secondBody;
    if (collidingID != collision.secondBody)
        collision.minimumTranslationVector = -collision.minimumTranslationVector;

    if (std::find(begin(pellets), end(pellets), collidingID) != end(pellets)) {
        ecs.entities.deleteEntity(collidingID);
        score++;
        if (auto text = ecs.components.getComponent<GUITextComponent>(scoreCounter))
            text->text.setString(std::to_string(score));
    } else
        pacman.component<PositionComponent>()->position += collision.minimumTranslationVector;

    return true;
}

void PlayState::loadResources() {
    pacmanTex->loadFromFile(ecs.config.get("gameplay.files.pacmanTexture"));
}

void PlayState::createPacman() {
	pacman = ecs.entities.addEntity();

    // get parameters from game configuration
    auto pacmanSpeed = ecs.config.get<float>("gameplay.pacman.speed");
    auto pacmanDirStr = ecs.config.get("gameplay.pacman.direction", "LEFT");
    Direction pacmanDir;
    if (pacmanDirStr == "UP")
        pacmanDir = Direction::UP;
    else if (pacmanDirStr == "DOWN")
        pacmanDir = Direction::DOWN;
    else if (pacmanDirStr == "LEFT")
        pacmanDir = Direction::LEFT;
    else if (pacmanDirStr == "RIGHT")
        pacmanDir = Direction::RIGHT;
    else
        ecs.logger.error("Error in the config file: pacman init direction must be one of UP/DOWN/LEFT/RIGHT.");
    auto pacmanWidth = ecs.config.get<float>("gameplay.pacman.size.width");
    auto pacmanHeight = ecs.config.get<float>("gameplay.pacman.size.height");
    auto pacmanPositionX = ecs.config.get<float>("gameplay.pacman.position.x");
    auto pacmanPositionY = ecs.config.get<float>("gameplay.pacman.position.y");

    //create components
	pacman.addComponent<PositionComponent>(pacmanPositionX, pacmanPositionY);
	pacman.addComponent<SizeComponent>(pacmanWidth, pacmanHeight);
	pacman.addComponent<MovementComponent>(pacmanDir, pacmanSpeed);
    auto pacmanCollision = pacman.addComponent<CollisionComponent>();
	auto pacmanAppearance = pacman.addComponent<GraphicsComponent>();

    pacmanCollision->emitEvent = true;
	pacmanAppearance->texture = pacmanTex;
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

void PlayState::createMaze() {
    // 22x19 (hXw)
    // # - wall
    // <empty> - nothing
    // . - food pellet
    // o - power pellet
    // - door
    const char* mazeLayout = {
        "###################"
        "#........#........#"
        "#o##.###.#.###.##o#"
        "#.##.###.#.###.##.#"
        "#.................#"
        "#.##.#.#####.#.##.#"
        "#....#...#...#....#"
        "####.### # ###.####"
        "   #.#       #.#   "
        "####.# ##-## #.####"
        "    .  #   #  .    "
        "####.# ##### #.####"
        "   #.#       #.#   "
        "####.# ##### #.####"
        "#........#........#"
        "#.##.###.#.###.##.#"
        "#o.#..... .....#.o#"
        "##.#.#.#####.#.#.##"
        "#....#...#...#....#"
        "#.######.#.######.#"
        "#.................#"
        "###################"
    };

    // TODO
}
