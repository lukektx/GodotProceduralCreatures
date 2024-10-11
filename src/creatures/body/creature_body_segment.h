#ifndef CREATURE_BODY_SEGMENT_H
#define CREATURE_BODY_SEGMENT_H

#include <godot_cpp/variant/utility_functions.hpp>

#define DEFAULT_SEGMENT_RADIUS 20

namespace godot {

class CreatureBodySegment {
 private:
  CreatureBodySegment *next = nullptr;
  CreatureBodySegment *prev = nullptr;
  Vector2 pos = Vector2();
  Vector2 direction = Vector2();
  float radius = DEFAULT_SEGMENT_RADIUS;

 public:
  CreatureBodySegment();
  CreatureBodySegment(float p_radius);
  ~CreatureBodySegment();

  PackedVector2Array get_side_points();
  Vector2 get_head_point();
  Vector2 get_tail_point();

  void set_next(CreatureBodySegment *p_next);
  CreatureBodySegment *get_next();
  void set_prev(CreatureBodySegment *p_prev);
  CreatureBodySegment *get_prev();
  void set_radius(float p_radius);
  float get_radius();
  void set_pos(Vector2 p_pos);
  Vector2 get_pos();
  void set_direction(Vector2 p_direction);
  Vector2 get_direction();
};
}  // namespace godot

#endif