#include "scene.h"
#include "entity.h"

Entity Scene::CreateEntity(const std::string& name) {
  Entity e = Entity(m_ecs.CreateEntity(), this);
  e.AddComponent<Tag>(name);
  e.AddComponent<Transform>();
  return e;
}

void Scene::DestroyEntity(Entity e) {
  m_ecs.DestroyEntity(e.m_id);
}
