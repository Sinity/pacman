#include <ecs/ecs.h>
#include "tasks/Controller.h"

int main() {
	auto engine = ECS{};

	auto cOut = std::make_shared<ConsoleOutput>();
	cOut->setMinPriority(LogType::Information);
	engine.logger.addOutput(std::move(cOut));

	if(!engine.config.load("config.cfg")) {
		engine.logger.fatal("main: Failed to load main config file. Stopping execution.");
		return EXIT_FAILURE;
	}

	engine.tasks.addTask<Controller>();

	engine.run();

	engine.logger.info("Application executed, returning to system");
	return 0;
}

