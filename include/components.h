#pragma once
#include <string>

/// Dummy type
struct Component {};

/// Tag to keep track of entities
struct Tag {
  /// The name of this component
  std::string tag;

  /// Delete the default constructor because the tag field must set
  Tag() = default;
  /// Copy constructor
  Tag(const Tag&) = default;
  /// Construct a new tag
  /// @param t the contents of the tag
  Tag(const std::string& t):tag(t) {}
};

/// A three element vector, not really a component but to make transform easier
struct vec3 {
  /// X, y, and z values of the vector
  float x, y, z;
};

/// Transform to apply to an object
struct Transform {
  ///  position of the entity
  vec3 position{0.f, 0.f, 0.f};
  ///  rotation of the entity
  vec3 rotation{0.f, 0.f, 0.f};
  ///  scale of the entity
  vec3 scale{1.f, 1.f, 1.f};

  /// Constructors!
  Transform() = default;
  /// Copy constructor
  Transform(const Transform&) = default;
  /// Create a transform at a position
  /// @param pos the position this transform will be created with
  Transform(const vec3& pos): position(pos) {};
};
