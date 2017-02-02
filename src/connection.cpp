#include "connection.hpp"

connection* inner_create(u_long host, u_short port) {
  SOCKADDR_IN address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = host;
  address.sin_port = port;
  return new connection(address);
}

connection create(u_short port) {
  port = htons(port);
  connection* created = inner_create(INADDR_ANY, port);
  return *created;
}

connection create(char* hostname, u_short port) {
  u_long host = inet_addr(hostname);
  port = htons(port);
  connection* created = inner_create(host, port);
  return *created;
}

connection::connection(SOCKADDR_IN address) {
  this->address = address;
}

int connection::get_port() {
  return port;
}

SOCKET connection::get_socket() {
  return socket;
}

void connection::set_socket(SOCKET socket) {
  this->socket = socket;
}

SOCKADDR_IN connection::get_address() {
  return address;
}
