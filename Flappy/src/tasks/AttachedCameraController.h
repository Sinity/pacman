#pragma once
#include <ecs/ecs.h>
#include <SFML/Graphics.hpp>
#include "../components/PositionComponent.h"

using namespace EECS;

class AttachedCameraController : public Task<AttachedCameraController> {
public:
	AttachedCameraController(ECS& engine, sf::RenderWindow& window, Entity& attachmentPoint, sf::Vector2f offset,
	                         bool followX, bool followY) :
			Task(engine), engine(engine),
			window(window), attachmentPoint(attachmentPoint),
            offset(offset), followX(followX), followY(followY) { }

    void setAttachmentPoint(Entity attachmentPoint) {
        attachmentPoint = attachmentPoint;
    }

	void update() override {
		auto view = window.getView();
		auto desiredPos = attachmentPoint.component<PositionComponent>()->position + offset;
		auto finalPos = sf::Vector2f{followX ? desiredPos.x : view.getCenter().x,
                                     followY ? desiredPos.y : view.getCenter().y};
		view.setCenter(finalPos);
		window.setView(view);
	}

private:
	ECS& engine;

    //this class is responsible for managing view, which is avviable through this object
	sf::RenderWindow& window;

    //camera will follow this entity's position
	Entity attachmentPoint;

    //offset from entity position. For example, if camera follows object on X-axis,
    //then X coordinate of this offest will set on which 'column' object will be on camera
	sf::Vector2f offset = {0.f, 0.f};

    //camera will be 'fixed' on object's position on desired axis
    //for example, if it follows X but not Y then object will be always in the same 'column' of window,
    //but will move freely up-down
	bool followX = false;
    bool followY = false;
};
