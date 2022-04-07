#include "movement_task.h"
#include "../components/movement_component.h"
#include "../components/position_component.h"

using namespace EECS;

MovementTask::MovementTask(ECS& engine) : Task<MovementTask>(engine) { }

void MovementTask::update() {
    auto elapsedTime = (float)frequency.count() / 1000;

	auto physical = ecs.components.intersection<MovementComponent, PositionComponent>();
	for (auto&& obj : physical) {
        auto& pos = obj.get<PositionComponent>();
        auto& movement = obj.get<MovementComponent>();
        float displacement = movement.speed * elapsedTime;

        switch (movement.direction) {
        case Direction::UP:
            pos.position.y = pos.position.y - displacement;
            break;
        case Direction::DOWN:
            pos.position.y = pos.position.y + displacement;
            break;
        case Direction::LEFT:
            pos.position.x = pos.position.x - displacement;
            break;
        case Direction::RIGHT:
            pos.position.x = pos.position.x + displacement;
            break;
        }
	}
}

