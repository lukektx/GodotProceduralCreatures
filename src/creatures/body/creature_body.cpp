#include "creature_body.h"

using namespace godot;

void CreatureBody::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_length"), &CreatureBody::get_length);
  ClassDB::bind_method(D_METHOD("set_length", "p_length"),
                       &CreatureBody::set_length);
  ADD_PROPERTY(PropertyInfo(Variant::INT, "length"), "set_length",
               "get_length");
  ClassDB::bind_method(D_METHOD("move_towards", "head_pos", "speed"),
                       &CreatureBody::move_towards);
  ClassDB::bind_method(D_METHOD("initialize_const_radius", "radius", "length"),
                       static_cast<void (CreatureBody::*)(float, int)>(
                           &CreatureBody::initialize_segments));
  ClassDB::bind_method(D_METHOD("initialize_lengths", "lengths"),
                       static_cast<void (CreatureBody::*)(Array)>(
                           &CreatureBody::initialize_segments));
}

CreatureBody::CreatureBody() {
  collision_polygon = memnew(CollisionPolygon2D);
  add_child(collision_polygon);
  outline = memnew(Line2D);
  add_child(outline);
  initialize_segments(DEFAULT_SEGMENT_RADIUS, length);
}
CreatureBody::~CreatureBody() { cleanup_segments(); }

void CreatureBody::move_towards(Vector2 head_pos, float speed) {
  CreatureBodySegment *current_segment = head;
  CreatureBodySegment *prev_segment = nullptr;
  CreatureBodySegment *prev_prev_segment = nullptr;

  Vector2 current_pos;
  Vector2 prev_pos;

  while (current_segment) {
    current_pos = current_segment->get_pos();
    if (current_segment == head) {
      current_segment->set_direction((head_pos - current_pos).normalized());
    } else {
      // Apply angle constraint here

      Vector2 prev_dir = prev_segment->get_direction();
      Vector2 current_dir = current_segment->get_direction();
      prev_segment->set_direction(angle_constraint(prev_dir, current_dir));

      if (prev_prev_segment) {
        Vector2 pos_change =
            prev_segment->get_direction() * prev_prev_segment->get_radius();
        prev_segment->set_pos(prev_prev_segment->get_pos() - pos_change);
        prev_pos = prev_segment->get_pos();
      } else if (prev_segment == head) {
        prev_segment->set_pos(prev_pos + prev_segment->get_direction() * speed);
        prev_pos = prev_segment->get_pos();
        current_segment->set_direction((prev_pos - current_pos).normalized());
      }
      current_segment->set_direction((prev_pos - current_pos).normalized());

      if (current_segment == tail && current_segment != head) {
        Vector2 pos_change =
            current_segment->get_direction() * prev_segment->get_radius();
        current_segment->set_pos(prev_pos - pos_change);
      }
    }

    prev_prev_segment = prev_segment;
    prev_segment = current_segment;
    prev_pos = prev_segment->get_pos();
    current_segment = current_segment->get_next();
  }

  queue_redraw();
  update_shape();
}

// Changes dir1 if not within angle constraint
Vector2 CreatureBody::angle_constraint(Vector2 dir1, Vector2 dir2) {
  double angle = dir2.angle_to(dir1);
  if (abs(angle) > MAX_ANGLE) {
    double rotation = angle > 0 ? MAX_ANGLE : -MAX_ANGLE;
    return dir2.rotated(rotation);
  }
  return dir1;
}

void CreatureBody::add_segment(float radius) {
  CreatureBodySegment *new_segment = new CreatureBodySegment(radius);
  length++;
  if (tail) {
    tail->set_next(new_segment);
    new_segment->set_prev(tail);
    tail = new_segment;
  } else {
    head = new_segment;
    tail = new_segment;
  }
}

void CreatureBody::initialize_segments(float radius, int length) {
  cleanup_segments();
  for (int i = 0; i < length; i++) {
    add_segment(radius);
    CreatureBodySegment *prev = tail->get_prev();
    if (prev) {
      UtilityFunctions::print(
          "initialized to ",
          prev->get_pos() + Vector2(1, 0) * prev->get_radius());
      tail->set_pos(prev->get_pos() + Vector2(1, 0) * prev->get_radius());
    }
  }
}

void CreatureBody::initialize_segments(Array radii) {
  cleanup_segments();
  for (int i = 0; i < radii.size(); i++) {
    add_segment(radii[i]);
    CreatureBodySegment *prev = tail->get_prev();
    if (prev) {
      tail->set_pos(prev->get_pos() + Vector2(1, 0) * prev->get_radius());
    }
  }
}

void CreatureBody::cleanup_segments() {
  CreatureBodySegment *current_segment = head;

  while (current_segment) {
    CreatureBodySegment *next_segment = current_segment->get_next();
    delete (current_segment);
    current_segment = next_segment;
  }
}

void CreatureBody::update_shape() {
  PackedVector2Array all_points;
  // 1 point on each side, one front and 1 back then copy of front
  all_points.resize(2 * length + 3);
  static int last_index = all_points.size() - 1;
  CreatureBodySegment *current_segment = head;
  CreatureBodySegment *prev_segment = nullptr;
  int segment_index = 0;

  // add head points
  if (head) {
    Vector2 head_point = head->get_head_point();
    all_points[segment_index] = head_point;
    all_points[last_index - segment_index] = head_point;
    segment_index++;
  }
  // add all side body points
  while (current_segment) {
    PackedVector2Array sides = current_segment->get_side_points();
    all_points[segment_index] = sides[0];
    all_points[last_index - segment_index] = sides[1];
    prev_segment = current_segment;
    current_segment = current_segment->get_next();
    segment_index++;
  }
  // add tail point
  all_points[segment_index] = prev_segment->get_tail_point();

  collision_polygon->set_polygon(all_points);
  Curve2D *outline_curve = memnew(Curve2D);
  for (int i = 0; i < all_points.size(); i++) {
    outline_curve->add_point(all_points[i]);
  }
  outline->set_points(outline_curve->tessellate());
}

CreatureBody::iterator CreatureBody::begin() { return head; }
CreatureBody::const_iterator CreatureBody::begin() const { return head; }
CreatureBody::iterator CreatureBody::end() { return tail; }
CreatureBody::const_iterator CreatureBody::end() const { return tail; }

int CreatureBody::get_length() { return length; }
void CreatureBody::set_length(int p_length) { length = p_length; }

void CreatureBody::_ready() {
  initialize_segments(DEFAULT_SEGMENT_RADIUS, length);
}

void CreatureBody::_draw() {
  CreatureBodySegment *current_segment = head;
  while (current_segment) {
    Vector2 pos = current_segment->get_pos();
    // draw_circle(pos, current_segment->get_radius(), Color(0, 0, 1, 1), false,
    //             5);
    // // draw_line(pos, pos + current_segment->get_direction() * 10,
    // Color(1, 0, 0, 1), 5);
    current_segment = current_segment->get_next();
  }
}