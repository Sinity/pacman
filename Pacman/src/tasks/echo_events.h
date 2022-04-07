#pragma once
#include <ecs/ecs.h>

using namespace EECS;

struct UnknownSFMLEvent;
struct ApplicationClosed;
struct KeyPressed;
struct KeyReleased;
struct TextEntered;
struct MouseButtonPressed;
struct MouseButtonReleased;
struct MouseMoved;
struct MouseWheelMoved;
class EchoEvents : public Task<EchoEvents>, Receives<EchoEvents, UnknownSFMLEvent, ApplicationClosed, KeyPressed, KeyReleased, TextEntered, MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseWheelMoved> {
public:
	EchoEvents(ECS& ecs);

	void update() override;

	bool receive(UnknownSFMLEvent& unknownSFMLEvent);
	bool receive(ApplicationClosed& appClosed);
	bool receive(KeyPressed& keyPressed);
	bool receive(KeyReleased& keyReleased);
	bool receive(TextEntered& textEntered);
	bool receive(MouseButtonPressed& mouseButtonPressed);
	bool receive(MouseButtonReleased& mouseButtonReleased);
	bool receive(MouseMoved& mouseMoved);
	bool receive(MouseWheelMoved& mouseWheelMoved);

private:
	Logger logger;
};
