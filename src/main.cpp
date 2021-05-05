#include "scene.h"
#include "entity.h"
#include <iostream>

/// Prints out facts about the given entity
/// @param e entity we want to print out
void PrintEntity(Entity e) {
  Transform* t = e.GetComponent<Transform>();
  std::cout << "Entity " << e.GetComponent<Tag>()->tag << ":\n"
    << "pos.x: " << t->position.x << " "
    << "pos.y: " << t->position.y << " "
    << "pos.z: " << t->position.z << "\n"
    << "rot.x: " << t->rotation.x << " "
    << "rot.y: " << t->rotation.y << " "
    << "rot.z: " << t->rotation.z << "\n"
    << "scale.x: " << t->scale.x << " "
    << "scale.y: " << t->scale.y << " "
    << "scale.z: " << t->scale.z << "\n";
}

/// A fake component for testing
struct Dummy { };

int main(void) {
	// Create our scene
	Scene scene;
	// Make a couple entities
	Entity e1 = scene.CreateEntity("e1");
	Entity e2 = scene.CreateEntity();
	PrintEntity(e1);
	PrintEntity(e2);

  // modify the component of one
  std::cout << "Modifying e1...\n";
	e1.GetComponent<Transform>()->rotation = {2.6, 5.3, 0.f};
	e2.AddComponent<Dummy>();
	e2.AddComponent<Dummy>();
	PrintEntity(e1);

  std::cout << "Listing entities with transform and tag\n";
	// get all entities with a transform (should be both of them)
	for (auto& id : scene.GetEcs().GetEntitiesWith<Transform, Tag>()) {
    std::cout << id << "\n";
  }

  std::cout << "Listing entities with dummy component\n";
	// get all entities with a transform (should be both of them)
	for (auto& id : scene.GetEcs().GetEntitiesWith<Dummy>()) {
    std::cout << id << "\n";
  }

	// add a dummy component
	// get all with a transform and the dummy

  // delete an entity
  std::cout << "Deleting entity e1\n";
	scene.DestroyEntity(e1);

	assert(e1.GetComponent<Transform>() == nullptr);
	assert(e1.GetComponent<Tag>() == nullptr);

}
