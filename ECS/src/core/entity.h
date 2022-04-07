#pragma once
#include <limits>
#include "entityManager.h"
#include "entityID.h"
#include "componentContainerID.h"

namespace EECS {
class Entity {
public:
    Entity() : id(0), entities(nullptr), components(nullptr) {}
    Entity(EntityID id, EntityManager& entities, ComponentManager& components) :
        id(id), entities(&entities), components(&components) {}

    EntityID getID() const { return id; }
    operator EntityID() const { return getID(); }
    //bool operator==(const Entity&& other) { return this->getID() == other.getID(); }
    //bool operator==(const EntityID&& other) { return this->getID() == other; }

    bool reassign(EntityID id) {
        if (!entities || !entities->entityExists(id))
            return false;

        this->id = id;
        cachedComponent = { std::numeric_limits<size_t>::max(), nullptr };

        return true;
    }

    bool exists() const { return entities ? entities->entityExists(id) : false; }
    operator bool() const { return exists(); }

    bool deleteEntity() {
        if (entities == nullptr)
            return false;

        auto result = entities->deleteEntity(id);
        id = 0;
        return result;
    }

    Entity clone() const { return entities ? entities->cloneEntity(id) : Entity(); }

    template <class T>
    T* component() {
        if (!components)
            return nullptr;

        if (cachedComponent.first != ComponentContainerID::get<T>() ||
            !components->validComponentPointer((T*)cachedComponent.second, id)) {

            cachedComponent = { ComponentContainerID::get<T>(), components->getComponent<T>(id) };
        }

        return (T*)cachedComponent.second;
    }

    template <class T>
    ComponentHandle<T> componentHandle() {
        if (!components)
            return ComponentHandle<T>();

        if (cachedComponent.first != ComponentContainerID::get<T>() ||
            !components->validComponentPointer((T*)cachedComponent.second, id)) {

            cachedComponent = { ComponentContainerID::get<T>(), components->getComponent<T>(id) };
        }

        return ComponentHandle<T>{*components, (T*)cachedComponent.second};
    }

    template <class T, class... Args>
    T* addComponent(Args&&... args) {
        return components ? components->addComponent<T>(id, std::forward<Args>(args)...) : nullptr;
    }

    template <class T>
    bool deleteComponent() {
        return components ? components->deleteComponent<T>(id) : false;
    }

private:
    EntityID id;
    EntityManager* entities;
    ComponentManager* components;
    std::pair<size_t, void*> cachedComponent;
};
}
