#pragma once
#include <ecs/ecs.h>

using namespace EECS;

class VerletIntegrator : public Task<VerletIntegrator> {
public:
    VerletIntegrator(ECS& engine);
    void update() override;
};
