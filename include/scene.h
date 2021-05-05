#pragma once
#include "ecs.h"
class Entity;
/// Stores the ECS for the current context
class Scene {
  private:
    /// So that an entity can use the ecs of it's scene
    friend Entity;
    /// This scene's private ecs
    ECS m_ecs;

  public:
    /// Constructor
    Scene() {};
    /// Destructor
    ~Scene(){};

    /// Creates a new entity in this scene
    /// @param name of the entity we are creating
    /// @return the new entity
    Entity CreateEntity(const std::string& name = "Empty");

    /// Destroys an entity
    /// @param entity we are destroying
    void DestroyEntity(Entity entity);

    /// Get a refernce to the ECS of this scene
    /// @return this scene's ECS
    ECS& GetEcs() { return m_ecs; }
};
