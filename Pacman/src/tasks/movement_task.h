#pragma once
#include <ecs/ecs.h>

using namespace EECS;

class MovementTask : public Task<MovementTask> {
public:
    MovementTask(ECS& engine);
    void update() override;
};
