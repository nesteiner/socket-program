#include "lib/tcp.h"
#include <string>
const int buffer_size = 1024;

int main() {
  int server = tcp::server("127.0.0.1", 9999);
  int connfd = tcp::accept(server);

  char buffer[buffer_size];
  memset(buffer, 0, sizeof(buffer));
  int result = 0;
  
  result = tcp::recv(connfd, buffer, sizeof(buffer), 0);
  std::cout << "[server] recv from client: " << std::string(buffer, result) << std::endl;
  sprintf(buffer, "fuck you");
  tcp::send(connfd, buffer, strlen(buffer), 0);
  std::cout << "[server] send done" << std::endl;
  
  tcp::shutdown(connfd, 2);
  tcp::shutdown(server, 2);
  return 0;
}

