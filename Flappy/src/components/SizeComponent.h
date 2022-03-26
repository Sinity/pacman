#pragma once
#include <ecs/ecs.h>
#include <SFML/System.hpp>

struct SizeComponent : public Component<SizeComponent> {
	explicit SizeComponent(float width = 0, float height = 0) : width(width), height(height) { }

    //dimmensions of the object in world's coordinate space(the same as positions, for example)
	float width = 0.f;
	float height = 0.f;
};

