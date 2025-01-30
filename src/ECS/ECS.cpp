#include "ECS.h"
#include "../Logger/Logger.h"

int BaseComponent::_nextId = 0;

int Entity::GetId() const
{
    return _id;
}

const Signature &System::GetSignature() const
{
    return _componentSignature;
}

void System::AddEntity(Entity e)
{
    _entities.push_back(e);
}

void System::RemoveEntity(Entity e)
{
    _entities.erase(
        std::remove_if(_entities.begin(), _entities.end(),
                       [&e](auto other)
                       { return other == e; }),
        _entities.end());
}

std::vector<Entity> System::GetEntities() const
{
    return _entities;
}

void Registry::Update()
{
    // TODO
    // Add entities waiting to be added
    // Remove entities waiting to be killed
}

Entity Registry::CreateEntity()
{
    int entityId = _numEntities++;
    if (entityId >= _entityComponentSignature.size())
    {
        _entityComponentSignature.resize(entityId + 1);
    }
    Entity entity(entityId);
    _entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with id = " + std::to_string(entityId));

    return entity;
}

void Registry::KillEntity()
{

    Logger::Log("Entity killed...");
    _numEntities--;
}