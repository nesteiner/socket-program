#include "lib/tcp.h"
const int buffer_size = 1024;

int main() {
  int client = tcp::client();
  tcp::connect(client, "127.0.0.1", 9999);

  char buffer[buffer_size];
  memset(buffer, 0, sizeof(buffer));
  // 1. read
  // 2. send
  int result = 0;

  sprintf(buffer, "hello world");
  result = tcp::send(client, buffer, strlen(buffer), 0);
  std::cout << "[client] send message to server" << std::endl;
  result = tcp::recv(client, buffer, sizeof(buffer), 0);
  std::cout << "[client] recv from server:" << std::string(buffer, result) << std::endl;
  
  tcp::shutdown(client, 2);
  return 0;
}
