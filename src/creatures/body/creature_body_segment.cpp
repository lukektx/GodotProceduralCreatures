#include "creature_body_segment.h"

using namespace godot;

CreatureBodySegment::CreatureBodySegment() {}
CreatureBodySegment::CreatureBodySegment(float p_radius) { radius = p_radius; }
CreatureBodySegment::~CreatureBodySegment() {}

PackedVector2Array CreatureBodySegment::get_side_points() {
  double angle1 = direction.angle() + (Math_PI / 2);
  double angle2 = direction.angle() - (Math_PI / 2);
  PackedVector2Array out;
  out.append(pos + Vector2(cos(angle1), sin(angle1)) * radius);
  out.append(pos + Vector2(cos(angle2), sin(angle2)) * radius);
  return out;
}

Vector2 CreatureBodySegment::get_head_point() {
  return pos + direction * radius;
}
Vector2 CreatureBodySegment::get_tail_point() {
  return pos - direction * radius;
}

void CreatureBodySegment::set_next(CreatureBodySegment *p_next) {
  next = p_next;
}
CreatureBodySegment *CreatureBodySegment::get_prev() { return prev; }
void CreatureBodySegment::set_prev(CreatureBodySegment *p_prev) {
  prev = p_prev;
}
CreatureBodySegment *CreatureBodySegment::get_next() { return next; }
float CreatureBodySegment::get_radius() { return radius; }
void CreatureBodySegment::set_radius(float p_radius) { radius = p_radius; }

void CreatureBodySegment::set_pos(Vector2 p_pos) { pos = p_pos; }
Vector2 CreatureBodySegment::get_pos() { return pos; }
void CreatureBodySegment::set_direction(Vector2 p_direction) {
  direction = p_direction;
}
Vector2 CreatureBodySegment::get_direction() { return direction; }