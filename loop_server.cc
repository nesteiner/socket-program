#include "lib/tcp.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

const int buffer_size = 1024;
int main() {
  int server = tcp::server("127.0.0.1", 9999);
  char buffer[buffer_size];
  memset(buffer, 0, sizeof(buffer));

  while (true) {
    int connfd = tcp::accept(server);
    int result = tcp::recv(connfd, buffer, buffer_size, 0);
    while (strncmp(buffer, "quit", 4) != 0) {
      cout << "[server] recv from client: " << string(buffer, result) << endl;

      // send back
      memset(buffer, 0, sizeof(buffer));
      sprintf(buffer, "fuck you");
      result = tcp::send(connfd, buffer, strlen(buffer), 0);
      result = tcp::recv(connfd, buffer, buffer_size, 0);
    }

    tcp::shutdown(connfd, 2);
  }

  tcp::shutdown(server, 2);
  return 0;
}
