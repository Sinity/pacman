#pragma once
#include "componentContainer.h"
#include "singleEventQueue.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace EECS {
std::vector<std::unique_ptr<ComponentContainerBase>>& singleComponentContainerArchetypes();
std::vector<std::unique_ptr<SingleEventQueueBase>>& singleEventQueueArchetypes();
std::vector<std::vector<size_t>>& componentDependencies();
}
