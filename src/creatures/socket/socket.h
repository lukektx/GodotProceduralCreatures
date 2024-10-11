#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class Socket : public Object {
  GDCLASS(Socket, Object)

  enum SocketSize { SMALL, MEDIUM, LARGE };
  enum SocketType { BODY, HEAD };

 private:
  Object *parent;
  Socket *connected_socket;
  SocketSize size;
  SocketType type;

 protected:
  static void _bind_methods();

 public:
  Socket();
  Socket(Object *parent, SocketSize size, SocketType type);
  ~Socket();

  bool is_connected();
  bool try_connect(Socket *other);

  Object *get_parent();
  void set_parent(Object *p_parent);
  Socket *get_connected_socket();
  void set_connected_socket(Socket *p_connected_socket);
  Object *get_parent();
  void set_parent(Object *p_parent);
};

}  // namespace godot

#endif