#include "mcx/entity_manager.hpp"

namespace mcx {

EntityId EntityManager::CreateEntity() {
    return nextId_++;
}

void EntityManager::DestroyEntity(EntityId id) {
    entities_.erase(id);
}

void EntityManager::AddComponent(EntityId entity, Component* component) {
    entities_[entity][component->GetType()] = component;
}

void EntityManager::RemoveComponent(EntityId entity, ComponentType type) {
    auto it = entities_.find(entity);
    if (it != entities_.end()) {
        it->second.erase(type);
    }
}

Component* EntityManager::GetComponentRaw(EntityId entity, ComponentType type) {
    auto it = entities_.find(entity);
    if (it != entities_.end()) {
        auto comp = it->second.find(type);
        if (comp != it->second.end()) {
            return comp->second;
        }
    }
    return nullptr;
}

size_t EntityManager::GetEntityCount() const {
    return entities_.size();
}

} // namespace mcx
