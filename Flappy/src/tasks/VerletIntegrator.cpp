#include "VerletIntegrator.h"
#include "../components/MovementComponent.h"
#include "../components/PositionComponent.h"

using namespace EECS;

VerletIntegrator::VerletIntegrator(ECS& engine) : Task<VerletIntegrator>(engine) { }

void VerletIntegrator::update() {
    auto elapsedTime = (float)frequency.count() / 1000;

	auto physical = ecs.components.intersection<MovementComponent, PositionComponent>();
	for (auto& obj : physical) {
		for(auto& force : obj.get<MovementComponent>().persistentForces)
			obj.get<MovementComponent>().resultantForce += force;

		auto currentPosition = obj.get<PositionComponent>().position;
        auto velocity = currentPosition - obj.get<MovementComponent>().oldPosition;
		auto acceleration = obj.get<MovementComponent>().resultantForce / obj.get<MovementComponent>().mass;
		obj.get<PositionComponent>().position = obj.get<PositionComponent>().position + velocity + acceleration * (elapsedTime*elapsedTime);

		obj.get<MovementComponent>().oldPosition = currentPosition;
		obj.get<MovementComponent>().resultantForce = {0, 0};
	}
}

