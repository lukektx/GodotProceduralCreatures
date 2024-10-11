#include "socket.h"

using namespace godot;

void Socket::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_parent"), &Socket::get_parent);
  ClassDB::bind_method(D_METHOD("set_parent", "p_parent"), &Socket::set_parent);
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "parent"), "set_parent",
               "get_parent");
}

Socket::Socket() {}
Socket::Socket(Object *p_parent, SocketSize p_size, SocketType p_type) {
  parent = p_parent;
  size = p_size;
  type = p_type;
}
Socket::~Socket() {}

Object *Socket::get_parent() { return parent; }
void Socket::set_parent(Object *p_parent) { parent = p_parent; }
Socket *Socket::get_connected_socket() { return connected_socket; }
void Socket::set_connected_socket(Socket *p_connected_socket) {
  connected_socket = p_connected_socket;
}

bool Socket::is_connected() { return connected_socket != nullptr; }

bool Socket::try_connect(Socket *other) {
  if (other->size == this->size && !this->is_connected() &&
      !other->is_connected()) {
  }
}