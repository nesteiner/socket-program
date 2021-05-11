#include "lib/tcp.h"
#include <algorithm>
#include <iostream>
#include <poll.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>

using std::cout;
using std::endl;
using std::string;

#define INIT_SIZE 128
#define MAXLINE 1024

void poll_handler(int listen_fd);
void epoll_handler(int listen_fd);

int main() {
  int listen_fd = tcp::server("127.0.0.1", 9999);
  epoll_handler(listen_fd);

  tcp::shutdown(listen_fd, 2);
  return 0;
}

void epoll_handler(int listen_fd) {
  char buffer[MAXLINE];
  memset(buffer, 0, MAXLINE);

  struct epoll_event event, events_set[INIT_SIZE];
  int efd = epoll_create(3);
  // load the listen fd to event
  event.data.fd = listen_fd;
  event.events = EPOLLIN;
  epoll_ctl(efd, EPOLL_CTL_ADD, listen_fd, &event);

  for (int count = 1; count <= 10; count += 1) {
    int ready_number = epoll_wait(efd, events_set, INIT_SIZE, -1);
    if (ready_number <= 0) {
      perror("error in epoll wait");
      exit(0);
    }

    for (int index = 0; index < ready_number; index += 1) {
      int sockfd = events_set[index].data.fd;
      uint32_t __event = events_set[index].events;
      // PROBLEM 排错怎么办
      // ATTENTION 排错直接找 event
      if (sockfd == listen_fd) {
        int connfd = tcp::accept(listen_fd);
        event.data.fd = connfd;
        event.events = EPOLLIN;
        epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &event);
      } else {
        if (__event & EPOLLIN) {
          int result = tcp::recv(sockfd, buffer, MAXLINE, 0);
          while (result > 0) {
            cout << "[server] recv: " << string(buffer, result) << endl;
            memset(buffer, 0, MAXLINE);
            sprintf(buffer, "fuck you");
            tcp::send(sockfd, buffer, strlen(buffer), 0);

            result = tcp::recv(sockfd, buffer, MAXLINE, 0);
          }

          tcp::shutdown(sockfd, 2);
	  // ATTENTION this is important !!!
	  epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
        }
      }
    }
  }
}

void poll_handler(int listen_fd) {
  char buffer[MAXLINE];
  memset(buffer, 0, MAXLINE);

  struct pollfd events[INIT_SIZE];
  events[0].fd = listen_fd;
  events[0].events = POLLIN;
  // init the events with fd -1

  for (int index = 1; index < INIT_SIZE; index += 1) {
    events[index].fd = -1;
  }

  while (true) {
    int ready_number = 0;
    int connfd;

    ready_number = poll(events, INIT_SIZE, -1);

    if (ready_number < 0) {
      perror("erro in poll ");
      exit(0);
    }
    // ignore the case of ready_number == -1
    if (events[0].revents & POLLIN) {
      connfd = tcp::accept(listen_fd);
      // load it into events
      for (int start = 1; start < INIT_SIZE; start += 1) {
        if (events[start].fd < 0) {
          events[start].fd = connfd;
          events[start].events = POLLIN;
          break;
        }
      }
    }

    for (int index = 1; index < INIT_SIZE; index += 1) {
      int sockfd = events[index].fd;
      int result;
      if (sockfd >= 0 && events[index].revents & POLLIN) {
        result = tcp::recv(sockfd, buffer, MAXLINE, 0);
        if (result > 0) {
          cout << "[server] read from client: " << string(buffer, result)
               << endl;
          memset(buffer, 0, sizeof(buffer));
          sprintf(buffer, "fuck you");
          tcp::send(sockfd, buffer, strlen(buffer), 0);
        } else if (result == 0) {
          cout << "[server] shutdown " << sockfd << endl;
          tcp::shutdown(sockfd, 2);
          events[index].fd = -1;
        } else {
          // read error
          cout << "read error" << endl;
          exit(0);
        }
      }
    }
  }

  tcp::shutdown(listen_fd, 2);
}
