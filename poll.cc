#include "lib/tcp.h"
#include <iostream>
#include <string>
#include <sys/select.h>
using std::cout;
using std::endl;
using std::string;

const int buffer_size = 1024;
void select_handler(int connfd);

int main() {
  int client = tcp::client();
  tcp::connect(client, "127.0.0.1", 9999);
  select_handler(client);
  tcp::shutdown(client, 2);
  return 0;
}


void select_handler(int connfd) {
  char buffer[buffer_size];
  memset(buffer, 0, sizeof(buffer));

  fd_set readmask, allread;
  FD_ZERO(&allread);
  FD_SET(0, &allread);
  FD_SET(connfd, &allread);

  int result, nBytes;
  
  while (true) {
    readmask = allread;
    result = select(connfd + 1, &readmask, NULL, NULL, NULL);

    // ignore this case: result < 0

    if (FD_ISSET(0, &readmask)) {
      nBytes = read(0, buffer, buffer_size);
      cout << "[stdin] " << string(buffer, nBytes) << endl;

      // send them to the server
      tcp::send(connfd, buffer, nBytes, 0);
      if(strncmp(buffer, "quit", 4) == 0) {
	exit(0);
      }
    }

    if (FD_ISSET(connfd, &readmask)) {
      nBytes = tcp::recv(connfd, buffer, buffer_size, 0);
      cout << "[client] recv from server: " << string(buffer, nBytes) << endl;
    }
  }

}
