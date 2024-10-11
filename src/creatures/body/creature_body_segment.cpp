#include "creature_body_segment.h"

using namespace godot;

CreatureBodySegment::CreatureBodySegment() {}
CreatureBodySegment::CreatureBodySegment(float p_radius) { radius = p_radius; }
CreatureBodySegment::CreatureBodySegment(float p_radius, float p_max_angle) {
  radius = p_radius;
  max_angle = p_max_angle;
}
CreatureBodySegment::~CreatureBodySegment() {}

PackedVector2Array CreatureBodySegment::get_side_points() {
  double angle1 = angle + (Math_PI / 2);
  double angle2 = angle - (Math_PI / 2);
  PackedVector2Array out;
  out.append(pos + Vector2(cos(angle1), sin(angle1)) * radius);
  out.append(pos + Vector2(cos(angle2), sin(angle2)) * radius);
  return out;
}

Vector2 CreatureBodySegment::get_head_point() {
  return pos + Vector2::from_angle(angle) * radius;
}
Vector2 CreatureBodySegment::get_tail_point() {
  return pos - Vector2::from_angle(angle) * radius;
}

float CreatureBodySegment::get_radius() { return radius; }
void CreatureBodySegment::set_radius(float p_radius) { radius = p_radius; }

void CreatureBodySegment::set_pos(Vector2 p_pos) { pos = p_pos; }
Vector2 CreatureBodySegment::get_pos() { return pos; }
void CreatureBodySegment::set_angle(float p_angle) { angle = p_angle; }
float CreatureBodySegment::get_angle() { return angle; }
void CreatureBodySegment::set_max_angle(float p_max_angle) {
  max_angle = p_max_angle;
}
float CreatureBodySegment::get_max_angle() { return max_angle; }