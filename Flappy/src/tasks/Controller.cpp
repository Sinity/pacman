#include "Controller.h"
#include "../states/PlayState.h"

//tasks
#include "../tasks/SFMLInputProxy.h"
#include "../tasks/Renderer.h"
#include "../tasks/VerletIntegrator.h"
#include "../tasks/CollisionDetector.h"

using namespace EECS;

Controller::Controller(ECS& ecs) : Task(ecs) {
	//boot all core tasks
	ecs.tasks.addTask<Renderer>(window);
	ecs.tasks.addTask<SFMLInputProxy>(window);
	ecs.tasks.addTask<VerletIntegrator>();
	ecs.tasks.addTask<CollisionDetector>(window);

	//start the gameplay immediately
	states.push(std::make_unique<PlayState>(ecs, window));
}

void Controller::update() {
    states.update();
}
