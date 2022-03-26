#pragma once
#include <ecs/ecs.h>

struct OrientationComponent : public Component<OrientationComponent> {
    //rotation of object with orgin in it's center, in degrees
	float rotation = 0.f;
};
