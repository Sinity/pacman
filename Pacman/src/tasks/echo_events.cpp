#include "echo_events.h"
#include "../events/system_events.h"

EchoEvents::EchoEvents(ECS& ecs) : Task<EchoEvents>(ecs), Receives(ecs.events), logger("EchoEvents") {
	if(ecs.config.get("tasks.echo_events.log") == "true") {
		auto cOut = std::make_shared<ConsoleOutput>();
		cOut->setMinPriority(LogType::Information);
		logger.addOutput(std::move(cOut));
	}
}

void EchoEvents::update() {
}

bool EchoEvents::receive(UnknownSFMLEvent& unknownSFMLEvent) {
	logger.info("Unknown sfml event{type=", (unsigned int)unknownSFMLEvent.event.type, "}");

	return true;
}

bool EchoEvents::receive(ApplicationClosed& appClosed) {
	(void)appClosed;
	logger.info("Application closed event received");

	return true;
}

bool EchoEvents::receive(KeyPressed& keyPressed) {
	logger.info("Pressed key '", (unsigned int)keyPressed.key.code, "'{alt=", keyPressed.key.alt,
	                 ",control=", keyPressed.key.control, ",shift=", keyPressed.key.shift, ",system=", keyPressed.key.system, "}");

	return true;
}

bool EchoEvents::receive(KeyReleased& keyReleased) {
	logger.info("Released key '", (unsigned int)keyReleased.key.code, "'{alt=", keyReleased.key.alt,
	                 ",control=", keyReleased.key.control, ",shift=", keyReleased.key.shift, ",system=", keyReleased.key.system, "}");

	return true;
}

bool EchoEvents::receive(TextEntered& textEntered) {
	logger.info("Entered text{unicode='", (unsigned int)textEntered.text.unicode,
	                 "',asAscii='", (char)textEntered.text.unicode, "'}");

	return true;
}

bool EchoEvents::receive(MouseButtonPressed& mouseButtonPressed) {
	logger.info("Pressed mouse button{code='", (unsigned int)mouseButtonPressed.button.button,
	                 "',position=<", mouseButtonPressed.button.x, ", ", mouseButtonPressed.button.x, ">}");

	return true;
}

bool EchoEvents::receive(MouseButtonReleased& mouseButtonReleased) {
	logger.info("Released mouse button{code='", (unsigned int)mouseButtonReleased.button.button,
	                 "',position=<", mouseButtonReleased.button.x, ", ", mouseButtonReleased.button.x, ">}");

	return true;
}

bool EchoEvents::receive(MouseMoved& mouseMoved) {
	logger.info("Mouse location{", mouseMoved.where.x, ", ", mouseMoved.where.y, "}"
	                 "\nMouse delta{", mouseMoved.delta.x, ", ", mouseMoved.delta.y, "}",
	                 "\nWorld delta{", mouseMoved.worldDelta.x, ", ", mouseMoved.worldDelta.y, "}");

	return true;
}

bool EchoEvents::receive(MouseWheelMoved& mouseWheelMoved) {
	logger.info("Mouse Wheel Moved{ticks: ", mouseWheelMoved.ticks,
	                 ", mousePos{", mouseWheelMoved.x, ", ", mouseWheelMoved.y, "}}");

	return true;
}
