#ifndef CREATURE_BODY_H
#define BODYCREATURE_BODY_H_H

#define BAKE_INTERVAL 20
#define MAX_ANGLE Math_PI / 8
#define ALMOST_ZERO 0.00001

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/collision_polygon2d.hpp>
#include <godot_cpp/classes/curve2d.hpp>
#include <godot_cpp/classes/line2d.hpp>

#include "creature_body_segment.h"

namespace godot {
class CreatureBody : public CharacterBody2D {
  GDCLASS(CreatureBody, CharacterBody2D);

 private:
  CreatureBodySegment *head = nullptr;
  CreatureBodySegment *tail = nullptr;
  CollisionPolygon2D *collision_polygon;
  Line2D *outline;

  int length = 0;

 protected:
  static void _bind_methods();
  static Vector2 angle_constraint(Vector2 dir1, Vector2 dir2);
  void cleanup_segments();
  void update_shape();

 public:
  typedef CreatureBodySegment *iterator;
  typedef const CreatureBodySegment *const_iterator;

  CreatureBody();
  CreatureBody(float radius, int length);
  CreatureBody(Array lengths);
  ~CreatureBody();

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  void add_segment(float radius);
  void initialize_segments(float radius, int length);
  void initialize_segments(Array radii);

  void move_towards(Vector2 head_pos, float speed);

  int get_length();
  void set_length(int p_length);

  void _ready() override;
  void _draw() override;
};
}  // namespace godot

#endif