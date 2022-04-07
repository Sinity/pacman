#include "controller.h"
#include "../states/play_state.h"
#include "../tasks/sfml_input_proxy.h"
#include "../tasks/echo_events.h"
#include "../tasks/renderer.h"
#include "../tasks/movement_task.h"
#include "../tasks/collision_detector.h"

using namespace EECS;

Controller::Controller(ECS& ecs) : Task(ecs) {
	//boot all core tasks
	ecs.tasks.addTask<SFMLInputProxy>(window);
    ecs.tasks.addTask<EchoEvents>();
	ecs.tasks.addTask<MovementTask>();
	ecs.tasks.addTask<CollisionDetector>(window);
    ecs.tasks.addTask<Renderer>(window);

	//start the gameplay immediately
	states.push(std::make_unique<PlayState>(ecs, window));
}

void Controller::update() {
    states.update();
}
