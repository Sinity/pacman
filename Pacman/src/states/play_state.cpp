#include "play_state.h"
#include <random>
#include <ctime>
#include <math.h>
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
	pacman = createPacman("gameplay.pacman");
    pacman2 = createPacman("gameplay.secondPacman");
	scoreCounterP1 = createScoreCounter(0.25);
    scoreCounterP2 = createScoreCounter(0.75);
    createMaze();
}

void PlayState::cleanup() {
    pacman.deleteEntity();
    pacman2.deleteEntity();
    scoreP1 = 0;
    scoreCounterP1.deleteEntity();
    scoreP2 = 0;
    scoreCounterP2.deleteEntity();

    for (auto wall : walls)
        ecs.entities.deleteEntity(wall);
    for (auto pellet : pellets)
        ecs.entities.deleteEntity(pellet);
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
    else if (keystroke.key.code == sf::Keyboard::Up)
        pacman2.component<MovementComponent>()->direction = Direction::UP;
    else if (keystroke.key.code == sf::Keyboard::Down)
        pacman2.component<MovementComponent>()->direction = Direction::DOWN;
    else if (keystroke.key.code == sf::Keyboard::Left)
        pacman2.component<MovementComponent>()->direction = Direction::LEFT;
    else if (keystroke.key.code == sf::Keyboard::Right)
        pacman2.component<MovementComponent>()->direction = Direction::RIGHT;

    return true;
}

bool PlayState::receive(CollisionEvent& collision) {
    if (collision.firstBody == pacman.getID() || collision.secondBody == pacman.getID()) {
        auto collidingID = collision.firstBody != pacman.getID() ? collision.firstBody : collision.secondBody;

        if (std::find(begin(pellets), end(pellets), collidingID) != end(pellets)) {
            ecs.entities.deleteEntity(collidingID);
            scoreP1++;
            if (auto text = ecs.components.getComponent<GUITextComponent>(scoreCounterP1))
                text->text.setString(std::to_string(scoreP1));
        } else
            pacman.component<MovementComponent>()->direction = Direction::NONE;

        return false;
    }

    if (collision.firstBody == pacman2.getID() || collision.secondBody == pacman2.getID()) {
        auto collidingID = collision.firstBody != pacman2.getID() ? collision.firstBody : collision.secondBody;

        if (std::find(begin(pellets), end(pellets), collidingID) != end(pellets)) {
            ecs.entities.deleteEntity(collidingID);
            scoreP2++;
            if (auto text = ecs.components.getComponent<GUITextComponent>(scoreCounterP2))
                text->text.setString(std::to_string(scoreP2));
        } else
            pacman2.component<MovementComponent>()->direction = Direction::NONE;

        return false;
    }

    return true;
}

Entity PlayState::createPacman(const std::string& configRoot) {
	auto pacman = ecs.entities.addEntity();

    auto pacmanSpeed = ecs.config.get<float>(configRoot + ".speed");
    auto pacmanDirStr = ecs.config.get(configRoot + ".direction", "LEFT");
    Direction pacmanDir;
    if (pacmanDirStr == "UP")
        pacmanDir = Direction::UP;
    else if (pacmanDirStr == "DOWN")
        pacmanDir = Direction::DOWN;
    else if (pacmanDirStr == "LEFT")
        pacmanDir = Direction::LEFT;
    else if (pacmanDirStr == "RIGHT")
        pacmanDir = Direction::RIGHT;
    else if (pacmanDirStr == "NONE")
        pacmanDir = Direction::NONE;
    else
        ecs.logger.error("Error in the config file: pacman init direction must be one of UP/DOWN/LEFT/RIGHT.");
    auto pacmanWidth = ecs.config.get<float>(configRoot + ".size.width");
    auto pacmanHeight = ecs.config.get<float>(configRoot + ".size.height");
    auto pacmanPositionX = ecs.config.get<float>(configRoot + ".position.x");
    auto pacmanPositionY = ecs.config.get<float>(configRoot + ".position.y");

    //create components
	pacman.addComponent<PositionComponent>(pacmanPositionX, pacmanPositionY);
	pacman.addComponent<SizeComponent>(pacmanWidth, pacmanHeight);
	pacman.addComponent<MovementComponent>(pacmanDir, pacmanSpeed);
    auto pacmanCollision = pacman.addComponent<CollisionComponent>();
    pacmanCollision->emitEvent = true;
    pacmanCollision->pushFromCollision = true;

    auto pacmanAppearance = pacman.addComponent<GraphicsComponent>();
    pacmanAppearance->texture = std::make_shared<sf::Texture>();
    pacmanAppearance->texture->loadFromFile(ecs.config.get(configRoot + ".texture"));
    
    return pacman;
}

Entity PlayState::createScoreCounter(float pos) {
    counterFont.loadFromFile(ecs.config.get("gameplay.files.font"));

    auto scoreCounter = ecs.entities.addEntity();
    scoreCounter.addComponent<GUITextComponent>();
    scoreCounter.component<GUITextComponent>()->text.setFont(counterFont);
    scoreCounter.component<GUITextComponent>()->text.setOutlineColor(sf::Color::White);
    scoreCounter.component<GUITextComponent>()->text.setFillColor(sf::Color::White);
    scoreCounter.component<GUITextComponent>()->text.setCharacterSize(ecs.config.get<int>("gameplay.score.fontSize"));
    scoreCounter.component<GUITextComponent>()->text.setString("0");

    scoreCounter.addComponent<PositionComponent>()->position.x = window.getDefaultView().getSize().x * pos;

    return scoreCounter;
}

void PlayState::createMaze() {
    // # - wall
    // <empty> - nothing
    // . - food pellet
    const char* mazeLayout = {
        "###################"
        "#........#........#"
        "#.##.###.#.###.##.#"
        "#.##.###.#.###.##.#"
        "#.................#"
        "#.##.#.#####.#.##.#"
        "#....#...#...#....#"
        "####.### # ###.####"
        "   #.#       #.#   "
        "####.# ## ## #.####"
        "#   .  #   #  .   #"
        "####.# ##### #.####"
        "   #.#       #.#   "
        "####.# ##### #.####"
        "#........#........#"
        "#.##.###.#.###.##.#"
        "#..#..... .....#..#"
        "##.#.#.#####.#.#.##"
        "#....#...#...#....#"
        "#.######.#.######.#"
        "#.................#"
        "###################"
    };
    int mazeWidth = 19;
    int mazeHeight = 22;

    for (int i = 0; i < mazeWidth; i++)
        for (int j = 0; j < mazeHeight; j++) {
            char tileKind = *(mazeLayout + mazeWidth*j + i);
            if (tileKind == '#')
                createWallSegment(i, j);
            else if (tileKind == '.')
                createFoodPellet(i, j);
        }
}

void PlayState::createWallSegment(int posX, int posY) {
    auto wall = ecs.entities.addEntity();
    walls.push_back(wall);

    static auto tileSize = ecs.config.get<float>("gameplay.map.tileSize");
    static auto tileOriginX = ecs.config.get<float>("gameplay.map.origin.x");
    static auto tileOriginY = ecs.config.get<float>("gameplay.map.origin.y");

    wall.addComponent<PositionComponent>(tileOriginX + posX*tileSize, tileOriginY + posY*tileSize);
    wall.addComponent<SizeComponent>(tileSize, tileSize);

    auto wallCollision = wall.addComponent<CollisionComponent>();
    wallCollision->isStatic = true;

    auto wallAppearance = wall.addComponent<GraphicsComponent>();
    wallAppearance->color = sf::Color::Blue;
}

void PlayState::createFoodPellet(int posX, int posY) {
    auto pellet = ecs.entities.addEntity();
    pellets.push_back(pellet);

    static auto tileSize = ecs.config.get<float>("gameplay.map.tileSize");
    static auto tileOriginX = ecs.config.get<float>("gameplay.map.origin.x");
    static auto tileOriginY = ecs.config.get<float>("gameplay.map.origin.y");

    pellet.addComponent<PositionComponent>(tileOriginX + posX * tileSize + 0.33*tileSize, tileOriginY + posY * tileSize + 0.33*tileSize);
    pellet.addComponent<SizeComponent>(tileSize / 4, tileSize / 4);

    auto pelletCollision = pellet.addComponent<CollisionComponent>();
    pelletCollision->isStatic = true;

    auto pelletAppearance = pellet.addComponent<GraphicsComponent>();
    pelletAppearance->color = sf::Color::Yellow;
}