#pragma once
#include "componentContainerID.h"
#include "entityID.h"
#include "globalDefs.h"
#include <unordered_map>

namespace EECS {

// todo: add checking for cyclical dependencies in ComponentManager's, add assert that there are none in it's
// constructor. Then add Unit tests for whole Dependency System. Also, double-check if it doesn't add too much overhead.
// Also, remember to finish that stuff written on that Vim page.

template <typename What, typename... On>
class Depends {
    Depends() {
        auto dependentID = ComponentContainerID::get<What>();

        if (componentDependencies().size() <= dependentID) {
            componentDependencies().resize(dependentID + 1);
        }

        fillWithIDs(componentDependencies()[dependentID]);
    }

    template <typename H, typename... T>
    void fillWithIDs(std::vector<size_t>& container) {
        container.emplace_back(ComponentContainerID::get<H>());
        fillWithIDs<T...>(container);
    }

    void fillWithIDs(std::vector<size_t>&) {}

    // ensure that Component doesn't depend on itself
    template <typename S, typename H, typename... T>
    constexpr bool dependsOnSelf() {
        static_assert(!std::is_same<S, H>(), "Component can't depend on itself!");
        dependsOnSelf<S, T...>();
        return false;
    }

    template <typename S>
    constexpr bool dependsOnSelf() {
        return false;
    }

    static_assert(!dependsOnSelf<What, On...>());
};

// Used for registering component type in the system.
// Allows for reflection stuff, like defining Entity archetypes from data.
template <typename T>
class ComponentRegistrator {
   public:
    ComponentRegistrator() {
        auto id = ComponentContainerID::get<T>();

        if (singleComponentContainerArchetypes().size() <= id) {
            singleComponentContainerArchetypes().resize(id + 1);
        }
        singleComponentContainerArchetypes()[id] = std::make_unique<ComponentContainer<T>>();
    }
};

/** \brief base Component type
*
* Each component have entityID member, which defines to what Entity given component belongs to.
*
* You can define any method, but it's meant as a structure of data, not a class.
*
* When you define component, you need to supply it's type in template argument of the Component(base class).
*
* Example of component definition:
*
* struct ComponentTypename : Component<ComponentTypename> {
*   int x = 1;
*   int y = 42;
*   char* buff = nullptr;
*
*   ~ComponentTypename() {
*       delete buff;
*   }
* };
*
*/
template <typename Derived>
struct Component {
    EntityID entityID;

   private:
    Component() { (void)componentRegistrator; }

    static ComponentRegistrator<Derived> componentRegistrator;

    friend Derived;
};

template <typename Derived>
ComponentRegistrator<Derived> Component<Derived>::componentRegistrator;
}
