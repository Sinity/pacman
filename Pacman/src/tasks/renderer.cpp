#include "renderer.h"
#include "../components/position_component.h"
#include "../components/size_component.h"
#include "../components/movement_component.h"
#include "../components/graphics_component.h"
#include "../components/gui_text_component.h"

void Renderer::update() {
    window.clear(fillColor);
    renderSprites();
    renderText();
    window.display();
}

void Renderer::renderSprites() {
    auto ents = ecs.components.intersection<GraphicsComponent, SizeComponent, PositionComponent>();

    //calculate planes range
    auto maxPlane = std::numeric_limits<int>::min();
    auto minPlane = std::numeric_limits<int>::max();
    for(auto&& gfx : ents) {
        maxPlane = (maxPlane < gfx.get<GraphicsComponent>().plane ? gfx.get<GraphicsComponent>().plane : maxPlane);
        minPlane = (minPlane > gfx.get<GraphicsComponent>().plane ? gfx.get<GraphicsComponent>().plane : minPlane);
    }

    for(auto currentPlane = maxPlane; currentPlane >= minPlane; currentPlane--) {
        for (auto&& gfx : ents) {
            if (gfx.get<GraphicsComponent>().plane != currentPlane)
                continue;

            //prepare rotation transform
            auto rotationTransform = sf::Transform{};        
            if(auto movementComp = ecs.components.getComponent<MovementComponent>(gfx.get<GraphicsComponent>().entityID)) {
                float rotation;
                if (movementComp->direction == Direction::UP)
                    rotation = -90;
                else if (movementComp->direction == Direction::DOWN)
                    rotation = 90;
                else if (movementComp->direction == Direction::LEFT)
                    rotation = 180;
                else if (movementComp->direction == Direction::RIGHT)
                    rotation = 0;

                rotationTransform.rotate(rotation,
                    gfx.get<PositionComponent>().position.x + gfx.get<SizeComponent>().width / 2,
                    gfx.get<PositionComponent>().position.y + gfx.get<SizeComponent>().height / 2);
            }
            rotationTransform.translate(gfx.get<PositionComponent>().position);

            //prepare sprite vertices
            sf::Vertex vertices[4];

            //positions are in local(object) coordinate space, it's translated to world space later
            vertices[0].position = {0, 0};
            vertices[1].position = {gfx.get<SizeComponent>().width, 0};
            vertices[2].position = {gfx.get<SizeComponent>().width, gfx.get<SizeComponent>().height};
            vertices[3].position = {0, gfx.get<SizeComponent>().height};

            //always use whole texture. I will add support for mapping only part of it, when I will need it
            vertices[0].texCoords = {0, 0};
            vertices[1].texCoords = {(float)gfx.get<GraphicsComponent>().texture->getSize().x, 0};
            vertices[2].texCoords = {(float)gfx.get<GraphicsComponent>().texture->getSize().x,
                                     (float)gfx.get<GraphicsComponent>().texture->getSize().y};
            vertices[3].texCoords = {0, (float)gfx.get<GraphicsComponent>().texture->getSize().y};

            //draw it
            auto state = sf::RenderStates{};
            state.texture = gfx.get<GraphicsComponent>().texture.get();
            state.transform = rotationTransform;
            window.draw(vertices, 4, sf::Quads, state);
        }
    }
}

void Renderer::renderText() {
    auto currView = window.getView();
    window.setView(window.getDefaultView());

    auto texts = ecs.components.intersection<GUITextComponent, PositionComponent>();

    for(auto i = 0u; i < texts.size(); i++) {
        texts[i].get<GUITextComponent>().text.setPosition(texts[i].get<PositionComponent>().position);
        window.draw(texts[i].get<GUITextComponent>().text);
    }

    window.setView(currView);
}

Renderer::Renderer(ECS& engine, sf::RenderWindow& window) :
    Task(engine),
    window(window) {
        auto resX = engine.config.get("tasks.renderer.resolution.x", 1600u);
        auto resY = engine.config.get("tasks.renderer.resolution.y", 900u);
        auto winTitle = engine.config.get("tasks.renderer.windowTitle");
        auto fullscreen = engine.config.get("tasks.renderer.fullscreen", std::string("false")) == "true";
        if(fullscreen) {
            window.create(sf::VideoMode::getFullscreenModes()[0], winTitle, sf::Style::Fullscreen);
        }
        else {
            window.create(sf::VideoMode(resX, resY, 32), winTitle);
        }

        fillColor.r = engine.config.get("tasks.renderer.fillColor.red", 0u);
        fillColor.g = engine.config.get("tasks.renderer.fillColor.green", 0u);
        fillColor.b = engine.config.get("tasks.renderer.fillColor.blue", 0u);

        auto left = engine.config.get("tasks.renderer.initialView.left", 0.0f);
        auto top = engine.config.get("tasks.renderer.initialView.top", 0.0f);
        auto width = engine.config.get("tasks.renderer.initialView.width", (float)resX);
        auto height = engine.config.get("tasks.renderer.initialView.height", (float)resY);
        window.setView(sf::View({left, top, width, height}));
    }
