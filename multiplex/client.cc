#include "lib/tcp.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

const int buffer_size = 1024;

int main() {
  int connfd = tcp::client();
  tcp::connect(connfd, "127.0.0.1", 9999);
  cout << "connected" << endl;
  char buffer[buffer_size];
  int result;
  memset(buffer, 0, sizeof(buffer));
  sprintf(buffer, "hello");
  result = tcp::send(connfd, buffer, strlen(buffer), 0);

  memset(buffer, 0, sizeof(buffer));
  result = tcp::recv(connfd, buffer, buffer_size, 0);
  cout << "[client] recv from server: " << string(buffer, result) << endl;
  tcp::shutdown(connfd, 2);
  return 0;
}
