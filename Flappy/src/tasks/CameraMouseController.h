#pragma once
#include <ecs/ecs.h>
#include <SFML/Graphics.hpp>

using namespace EECS;

struct MouseWheelMoved;
struct MouseButtonPressed;
struct MouseButtonReleased;
struct MouseMoved;
class CameraMouseController : public Task<CameraMouseController>, Receives<CameraMouseController, MouseWheelMoved, MouseButtonPressed, MouseButtonReleased, MouseMoved> {
public:
	CameraMouseController(ECS& engine, sf::RenderWindow& window);
	void update() override;
	bool receive(MouseWheelMoved& mouseWheelEvent);
	bool receive(MouseButtonPressed& mouseButtonPressEvent);
	bool receive(MouseButtonReleased& mouseButtonReleaseEvent);
	bool receive(MouseMoved& mouseMoved);

private:
	sf::RenderWindow& window;

	enum {
		Nothing,
		PanView
	} mouseMode{Nothing};

	float zoomFactor;
	float panViewXFactor;
	float panViewYFactor;
	unsigned int panViewMouseButtonCode;
};
