#include <ecs/ecs.h>
#include "tasks/controller.h"

void setupLogger(ECS& ecs);
bool loadConfiguration(ECS& ecs);

int main() {
	auto ecs = ECS{};

	setupLogger(ecs);

	if (!loadConfiguration(ecs))
		return EXIT_FAILURE;

	ecs.tasks.addTask<Controller>();
	ecs.run();

	ecs.logger.info("Application executed, returning to system");
	return 0;
}

void setupLogger(ECS& ecs) {
	auto consoleLoggerOut = std::make_shared<ConsoleOutput>();
	consoleLoggerOut->setMinPriority(LogType::Information);
	ecs.logger.addOutput(std::move(consoleLoggerOut));
	ecs.logger.info("Main logger initialized");
}

bool loadConfiguration(ECS& ecs) {
	if (!ecs.config.load("config.cfg")) {
		ecs.logger.fatal("main: Failed to load main config file. Stopping execution.");
		return false;
	}

	return true;
}