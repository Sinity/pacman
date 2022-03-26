#pragma once
#include <ecs/ecs.h>

using namespace EECS;

struct UnknownSFMLEvent;
struct ApplicationClosedEvent;
struct KeyPressed;
struct KeyReleased;
struct TextEntered;
struct MouseButtonPressed;
struct MouseButtonReleased;
struct MouseMoved;
struct MouseWheelMoved;
class InputEcho : public Task<InputEcho>, Receives<InputEcho, UnknownSFMLEvent, ApplicationClosedEvent, KeyPressed, KeyReleased, TextEntered, MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseWheelMoved> {
public:
	InputEcho(ECS& engine);

	void update() override;

	bool receive(UnknownSFMLEvent& unknownSFMLEvent);
	bool receive(ApplicationClosedEvent& appClosed);
	bool receive(KeyPressed& keyPressed);
	bool receive(KeyReleased& keyReleased);
	bool receive(TextEntered& textEntered);
	bool receive(MouseButtonPressed& mouseButtonPressed);
	bool receive(MouseButtonReleased& mouseButtonReleased);
	bool receive(MouseMoved& mouseMoved);
	bool receive(MouseWheelMoved& mouseWheelMoved);

private:
	Logger debugLogger;
};
