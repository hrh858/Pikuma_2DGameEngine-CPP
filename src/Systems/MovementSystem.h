#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

class MovementSystem: public System {
    public:
    MovementSystem() {
        // TODO...
        // RequireComponent<TransformComponent>();
        // RequireComponent<...>();
    }

    void Update() {
        // TODO ...
        // Iterate all of the entities that the system is intersted in
        for (auto entity : GetEntities()) {
            // TODO: Update their velocities and position, etc.
        }
    }
};

#endif