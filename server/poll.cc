#include "lib/tcp.h"
#include <iostream>
#include <ostream>
#include <poll.h>
#include <signal.h>
#include <string>
#include <string.h>
using std::cout;
using std::endl;
using std::string;

const int INIT_SIZE = 256;
const int MAXLINE = 1024;
const char *response =
    "HTTP/1.1 200 OK\nServer: Microsoft-IIS/5.1\nX-Powered-By: ASP.NET\nDate: "
    "Fri, 03 Mar 2006 06:34:03 GMT\nContent-Type: text/html\nAccept-Ranges: "
    "bytes\nLast-Modified: Fri, 03 Mar 2006 06:33:18 GMT\nETag: "
    "\"5ca4f75b8c3ec61:9ee\"\nContent-Length: 1\n\n1";

int main() {
  int listen_fd = tcp::server("0.0.0.0", 9999);
  // tcp::make_nonblocking(listen_fd);
  int index;
  struct pollfd events[INIT_SIZE];
  char buffer[MAXLINE];
  memset(buffer, 0, MAXLINE);

  // init the pollfds
  events[0].fd = listen_fd;
  events[0].events = POLLIN;
  for (int index = 1; index < INIT_SIZE; index += 1) {
    events[index].fd = -1;
  }

  int count = 1;
  while(true) {
    count += 1;
    int ready_number = poll(events, INIT_SIZE, -1);
    if (ready_number < 0) {
      perror("error in poll");
      exit(0);
    }

    // first check the listenfd

    // and you need to confirm errno
    if (events[0].revents & POLLIN) {

      int connfd = tcp::accept(listen_fd);
      if(connfd < 0) {
	continue;
      }
      
      for (index = 1; index < INIT_SIZE; index += 1) {
        if (events[index].fd < 0) {
          events[index].fd = connfd;
          events[index].events = POLLIN;
          break;
        }
      }

      if (index == INIT_SIZE) {
	std::cerr << "too many connection" << endl;
	exit(0);
      }
    }
    // and then check another event

    for (index = 1; index < INIT_SIZE; index += 1) {
      int connfd = events[index].fd;
      if (connfd > 0) {
        if (events[index].revents & POLLIN) {
          int result = tcp::recv(connfd, buffer, MAXLINE, 0);
          if (result > 0) {
            tcp::send(connfd, response, strlen(response), 0);
          } else if (result <= 0) {
            tcp::shutdown(connfd, 2);
            events[index].fd = -1;
          }
        }
      }
    }
  }

  tcp::shutdown(listen_fd, 2);
  return 0;
}

