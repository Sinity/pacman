#include "InputEcho.h"
#include "../events/system/ApplicationClosed.h"
#include "../events/system/KeyPressed.h"
#include "../events/system/KeyReleased.h"
#include "../events/system/MouseButtonPressed.h"
#include "../events/system/MouseButtonReleased.h"
#include "../events/system/MouseMoved.h"
#include "../events/system/MouseWheelMoved.h"
#include "../events/system/TextEntered.h"
#include "../events/system/UnknownSFMLEvent.h"

using namespace EECS;

InputEcho::InputEcho(ECS& engine) : Task<InputEcho>(engine), debugLogger("DEBUG") {
	if(engine.config.get("tasks.debugTask.log") == "true") {
		auto cOut = std::make_shared<ConsoleOutput>();
		cOut->setMinPriority(LogType::Information);
		debugLogger.addOutput(std::move(cOut));
	}
}

void InputEcho::update() {
}

bool InputEcho::receive(UnknownSFMLEvent& unknownSFMLEvent) {
	debugLogger.info("Unknown sfml event{type=", (unsigned int)unknownSFMLEvent.event.type, "}");

	return true;
}

bool InputEcho::receive(ApplicationClosedEvent& appClosed) {
	(void)appClosed;
	debugLogger.info("Application closed event received");

	return true;
}

bool InputEcho::receive(KeyPressed& keyPressed) {
	debugLogger.info("Pressed key '", (unsigned int)keyPressed.key.code, "'{alt=", keyPressed.key.alt,
	                 ",control=", keyPressed.key.control, ",shift=", keyPressed.key.shift, ",system=", keyPressed.key.system, "}");

	return true;
}

bool InputEcho::receive(KeyReleased& keyReleased) {
	debugLogger.info("Released key '", (unsigned int)keyReleased.key.code, "'{alt=", keyReleased.key.alt,
	                 ",control=", keyReleased.key.control, ",shift=", keyReleased.key.shift, ",system=", keyReleased.key.system, "}");

	return true;
}

bool InputEcho::receive(TextEntered& textEntered) {
	debugLogger.info("Entered text{unicode='", (unsigned int)textEntered.text.unicode,
	                 "',asAscii='", (char)textEntered.text.unicode, "'}");

	return true;
}

bool InputEcho::receive(MouseButtonPressed& mouseButtonPressed) {
	debugLogger.info("Pressed mouse button{code='", (unsigned int)mouseButtonPressed.button.button,
	                 "',position=<", mouseButtonPressed.button.x, ", ", mouseButtonPressed.button.x, ">}");

	return true;
}

bool InputEcho::receive(MouseButtonReleased& mouseButtonReleased) {
	debugLogger.info("Released mouse button{code='", (unsigned int)mouseButtonReleased.button.button,
	                 "',position=<", mouseButtonReleased.button.x, ", ", mouseButtonReleased.button.x, ">}");

	return true;
}

bool InputEcho::receive(MouseMoved& mouseMoved) {
	debugLogger.info("Mouse location{", mouseMoved.where.x, ", ", mouseMoved.where.y, "}"
	                 "\nMouse delta{", mouseMoved.delta.x, ", ", mouseMoved.delta.y, "}",
	                 "\nWorld delta{", mouseMoved.worldDelta.x, ", ", mouseMoved.worldDelta.y, "}");

	return true;
}

bool InputEcho::receive(MouseWheelMoved& mouseWheelMoved) {
	debugLogger.info("Mouse Wheel Moved{ticks: ", mouseWheelMoved.ticks,
	                 ", mousePos{", mouseWheelMoved.x, ", ", mouseWheelMoved.y, "}}");

	return true;
}
