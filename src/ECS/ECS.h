#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <array>
#include <unordered_map>
#include <typeindex>
#include <set>

const unsigned int MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

class Entity
{
private:
    int _id;

public:
    Entity(int id) : _id(id) {};
    int GetId() const;
    bool operator==(const Entity& other) const
    {
        return _id == other._id;
    }

    bool operator<(const Entity& other) const
    {
        return _id < other._id;
    }
};

struct BaseComponent
{
protected:
    static int _nextId;
};

template <typename T>
class Component : public BaseComponent
{
    static int GetId()
    {
        static auto id = _nextId;
        _nextId += 1;
        return id;
    }
};

class System
{
private:
    Signature _componentSignature;
    std::vector<Entity> _entities;

public:
    System() = default;
    ~System() = default;

    const Signature &GetSignature() const;
    void AddEntity(Entity e);
    void RemoveEntity(Entity e);
    std::vector<Entity> GetEntities() const;

    template <typename TComponent>
    void RequireComponent();
};

class BasePool {
    public:
        virtual ~BasePool() {}
};

template <typename T>
class Pool: public BasePool
{
private:
    std::vector<T> _data;

public:
    Pool(int size = 100) { Resize(size); }
    virtual ~Pool() = default;
    bool IsEmpty() const { return _data.empty(); }
    size_t GetSize() const { return _data.size(); }
    void Resize(size_t size) { _data.resize(size); }
    void Clear() { _data.clear(); }
    void Add(T object) { _data.push_back(object); }
    const T& Get(size_t index) const { return _data[index]; }
    const T& operator[](size_t index) const { return _data[index]; }
};

class Registry
{
private:
    // Total number of current entities.
    int _numEntities = 0;
    std::set<Entity> _entitiesToBeAdded;
    std::set<Entity> _entitiesToBeKilled;

    // Keeps pointers to pools of components for each possible component.
    // The pools are as big as the maximum amount of entities. 
    std::vector<std::unique_ptr<BasePool>> _componentPools;

    // Keeps track of which components are 'enabled' for each entity.
    std::vector<Signature> _entityComponentSignature;

    // Keeps track of all of the systems.
    // They can be accessed by the type, which is convenient.
    std::unordered_map<std::type_index, std::unique_ptr<System>> _systems;

public:
    Registry() = default;
    Entity CreateEntity();
    template<typename TComponent, typename ...TArgs> void AddComponent(Entity e, TArgs&& ...args); 
    template<typename T> void RemoveComponent(Entity e);
    template<typename T> bool HasComponent(Entity e) const;
    template<typename T> T& GetComponent(Entity e) const;
    void AddEntityToSystem(Entity e);
    // Here is where we actually insert/delete entities that
    // are waiting to be added/removed from the registry.
    // This is done like this to avoid having entities that
    // get added or removed in the middle of other systems
    // being updated.
    void Update();
    void KillEntity();

    // TODO:
    //
    // CreateEntity()
    // KillEntity()
    //
    // AddComponent(Entity e)
    // RemoveComponent(Entity e)
    // HasComponent(Entity e)
    //
    // AddSystem()
    // RemoveSystem()
    // HasSystem()
    // GetSystem()
};

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    _componentSignature.set();
}

template <typename TComponent, typename... TArgs>
void Registry::AddComponent(Entity e, TArgs &&...args)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = e.GetId();

    if (componentId > _componentPools.size()) {
        _componentPools.resize(componentId + 1, nullptr);
    }

    if (!_componentPools[componentId]) {
        _componentPools[componentId] = std::make_unique<Pool<TComponent>>();
    }

    auto componentPool = _componentPools[componentId];
    
    if (entityId > componentPool->GetSize()) {
        componentPool->Resize(_numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);

    _entityComponentSignature[entityId].set(componentId);
    _entityComponentSignature[entityId].(componentId);
}

template <typename TComponent>
void Registry::RemoveComponent(Entity e)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = e.GetId();

    _entityComponentSignature[entityId].set(componentId, false);
}

template <typename TComponent>
bool Registry::HasComponent(Entity e) const
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = e.GetId();

    return _entityComponentSignature[entityId].test(componentId);
}

#endif