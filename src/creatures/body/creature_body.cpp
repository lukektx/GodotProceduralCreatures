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
  std::vector<CreatureBodySegment *> new_segments();
  segments = new_segments;
  UtilityFunctions::print("Init segments", segments);
  initialize_segments(DEFAULT_SEGMENT_RADIUS, length);
}
CreatureBody::~CreatureBody() { cleanup_segments(); }

void CreatureBody::move_towards(Vector2 head_pos, float speed) {
  // CreatureBodySegment *current_segment = head;
  // CreatureBodySegment *prev_segment = nullptr;
  // CreatureBodySegment *prev_prev_segment = nullptr;

  // Vector2 current_pos;
  // Vector2 prev_pos;

  // while (current_segment) {
  //   current_pos = current_segment->get_pos();
  //   if (current_segment == head) {
  //     current_segment->set_direction((head_pos - current_pos).normalized());
  //   } else {
  //     // Apply angle constraint here

  //     Vector2 prev_dir = prev_segment->get_direction();
  //     Vector2 current_dir = current_segment->get_direction();
  //     prev_segment->set_direction(angle_constraint(prev_dir, current_dir));

  //     if (prev_prev_segment) {
  //       Vector2 pos_change =
  //           prev_segment->get_direction() * prev_prev_segment->get_radius();
  //       prev_segment->set_pos(prev_prev_segment->get_pos() - pos_change);
  //       prev_pos = prev_segment->get_pos();
  //     } else if (prev_segment == head) {
  //       prev_segment->set_pos(prev_pos + prev_segment->get_direction() *
  //       speed); prev_pos = prev_segment->get_pos();
  //       current_segment->set_direction((prev_pos -
  //       current_pos).normalized());
  //     }
  //     current_segment->set_direction((prev_pos - current_pos).normalized());

  //     if (current_segment == tail && current_segment != head) {
  //       Vector2 pos_change =
  //           current_segment->get_direction() * prev_segment->get_radius();
  //       current_segment->set_pos(prev_pos - pos_change);
  //     }
  //   }

  //   prev_prev_segment = prev_segment;
  //   prev_segment = current_segment;
  //   prev_pos = prev_segment->get_pos();
  //   current_segment = current_segment->get_next();
  // }
  CreatureBodySegment *prev_segment = nullptr;
  for (CreatureBodySegment *segment : *segments) {
    Vector2 current_pos = segment->get_pos();
    if (!prev_segment) {
      float max_angle = segment->get_max_angle();
      float angle = (head_pos - current_pos).angle();
      angle = CLAMP(angle, -max_angle, max_angle) * ANGLE_COEFFICIENT;

      // make sure angle is 2pi to -2pi
      segment->set_angle(angle);
      segment->set_pos(segment->get_pos() + Vector2::from_angle(angle) * speed);

    } else {
      Vector2 prev_pos = prev_segment->get_pos();
      float angle = (prev_pos - current_pos).angle();
      angle = angle_constraint(angle, prev_segment->get_angle(),
                               segment->get_max_angle());

      segment->set_angle(angle);
      segment->set_pos(prev_pos -
                       Vector2::from_angle(angle) * prev_segment->get_radius());
    }
  }

  queue_redraw();
  update_shape();
}

// returns new value for angle1 if not within angle constraint
float CreatureBody::angle_constraint(float angle1, float angle2,
                                     float max_angle) {
  double angle_diff = angle2 - angle1;
  if (abs(angle_diff) > max_angle) {
    double rotation = angle_diff > 0 ? max_angle : -max_angle;
    return angle2 + rotation;
  }
  return angle1;
}

void CreatureBody::initialize_segments(float radius, int length) {
  cleanup_segments();
  segments = new std::vector<CreatureBodySegment *>;
  for (int i = 0; i < length; i++) {
    segments->push_back(memnew(CreatureBodySegment(radius)));
  }
}

void CreatureBody::initialize_segments(Array radii) {
  cleanup_segments();
  segments = new std::vector<CreatureBodySegment *>;
  for (int i = 0; i < length; i++) {
    segments->push_back(memnew(CreatureBodySegment(radii[i])));
  }
}

void CreatureBody::cleanup_segments() {
  UtilityFunctions::print("Segments", segments);
  for (CreatureBodySegment *segment : *segments) {
    delete segment;
  }
  delete &segments;
}

void CreatureBody::update_shape() {
  PackedVector2Array all_points;
  // 1 point on each side, one front and 1 back then copy of front
  all_points.resize(2 * length + 3);
  static int last_index = all_points.size() - 1;
  for (int i = 0; i < segments->size(); i++) {
    if (i == 0) {
      Vector2 head_point = segments->at(i)->get_head_point();
      all_points[i] = head_point;
      all_points[last_index - i] = head_point;
    } else if (i == segments->size() - 1) {
      all_points[segments->size() / 2] = segments->at(i)->get_tail_point();
    }

    PackedVector2Array sides = segments->at(i)->get_side_points();
    all_points[i + 1] = sides[0];
    all_points[last_index - i - 1] = sides[1];
  }

  collision_polygon->set_polygon(all_points);
  Curve2D *outline_curve = memnew(Curve2D);
  for (int i = 0; i < all_points.size(); i++) {
    outline_curve->add_point(all_points[i]);
  }
  outline->set_points(outline_curve->tessellate());
  // memdelete(outline_curve);
}

int CreatureBody::get_length() { return length; }
void CreatureBody::set_length(int p_length) { length = p_length; }

void CreatureBody::_ready() {
  initialize_segments(DEFAULT_SEGMENT_RADIUS, length);
}

void CreatureBody::_draw() {
  for (CreatureBodySegment *segment : *segments) {
    Vector2 pos = segment->get_pos();
    draw_circle(pos, segment->get_radius(), Color(0, 0, 1, 1), false, 5);
    Vector2 dir = Vector2::from_angle(segment->get_angle()) * 30;
    draw_line(pos, pos + dir, Color(1, 0, 0, 1), 5);
  }
}