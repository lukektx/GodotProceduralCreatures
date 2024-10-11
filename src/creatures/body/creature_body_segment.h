#ifndef CREATURE_BODY_SEGMENT_H
#define CREATURE_BODY_SEGMENT_H

#include <godot_cpp/variant/utility_functions.hpp>

#define DEFAULT_SEGMENT_RADIUS 20
#define DEFAULT_MAX_ANGLE (Math_PI / 8)

namespace godot {

class CreatureBodySegment {
 private:
  Vector2 pos = Vector2();
  float angle = 0;
  float max_angle = DEFAULT_MAX_ANGLE;
  float radius = DEFAULT_SEGMENT_RADIUS;

 public:
  CreatureBodySegment();
  CreatureBodySegment(float p_radius);
  CreatureBodySegment(float p_radius, float p_max_angle);
  ~CreatureBodySegment();

  PackedVector2Array get_side_points();
  Vector2 get_head_point();
  Vector2 get_tail_point();

  void set_radius(float p_radius);
  float get_radius();
  void set_pos(Vector2 p_pos);
  Vector2 get_pos();
  void set_max_angle(float p_max_angle);
  float get_max_angle();
  void set_angle(float p_angle);
  float get_angle();
};
}  // namespace godot

#endif