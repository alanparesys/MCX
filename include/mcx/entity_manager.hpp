#pragma once

#include "mcx/export.hpp"
#include <cstdint>
#include <functional>
#include <unordered_map>

namespace mcx {

using EntityId = uint64_t;
using ComponentType = uint32_t;

class MCX_API Component {
public:
    virtual ~Component() = default;
    virtual ComponentType GetType() const = 0;
};

class MCX_API EntityManager {
public:
    EntityId CreateEntity();
    void DestroyEntity(EntityId id);
    
    void AddComponent(EntityId entity, Component* component);
    void RemoveComponent(EntityId entity, ComponentType type);
    
    template<typename T>
    T* GetComponent(EntityId entity) {
        return static_cast<T*>(GetComponentRaw(entity, T::Type));
    }
    
    [[nodiscard]] size_t GetEntityCount() const;

private:
    Component* GetComponentRaw(EntityId entity, ComponentType type);
    
    std::unordered_map<EntityId, std::unordered_map<ComponentType, Component*>> entities_;
    EntityId nextId_{1};
};

} // namespace mcx
