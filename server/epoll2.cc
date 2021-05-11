#include "lib/tcp.h"
#include <asm-generic/errno.h>
#include <bits/stdint-uintn.h>
#include <iostream>
#include <ostream>
#include <signal.h>
#include <string.h>
#include <sys/epoll.h>
using std::cout;
using std::endl;

const int INIT_SIZE = 256;
const int MAXLINE = 1024;
const char *response =
    "HTTP/1.1 200 OK\nServer: Microsoft-IIS/5.1\nX-Powered-By: ASP.NET\nDate: "
    "Fri, 03 Mar 2006 06:34:03 GMT\nContent-Type: text/html\nAccept-Ranges: "
    "bytes\nLast-Modified: Fri, 03 Mar 2006 06:33:18 GMT\nETag: "
    "\"5ca4f75b8c3ec61:9ee\"\nContent-Length: 1\n\n1";

int main() {
  int listen_fd = tcp::server("0.0.0.0", 9999);
  tcp::make_nonblocking(listen_fd);
  int efd = epoll_create(3);

  struct epoll_event event, event_set[INIT_SIZE];
  char buffer[MAXLINE];
  memset(buffer, 0, MAXLINE);

  // put listen_fd into the event
  event.data.fd = listen_fd;
  event.events = EPOLLIN;
  epoll_ctl(efd, EPOLL_CTL_ADD, listen_fd, &event);

  int count = 1;
  while (true) {
    count += 1;
    int ready_number = epoll_wait(efd, event_set, INIT_SIZE, 5000);

    if (ready_number < 0) {
      perror("error in epoll");
      exit(0);
    }
    
    for (int index = 0; index < ready_number; index += 1) {
      int connfd = event_set[index].data.fd;
      uint32_t __event = event_set[index].events;

      if(__event & EPOLLIN) {
	if(connfd == listen_fd) {
	  int sockfd = tcp::accept(connfd);
	  int count = 0;
	  while(sockfd > 0) {
	    // STUB
	    count += 1;
	    event.data.fd = sockfd;
	    event.events  = EPOLLIN;
	    epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &event);
	    sockfd = tcp::accept(connfd);
	  }
	  
	  if(sockfd == -1 && errno != EWOULDBLOCK) {
	    perror("error in accept");
	    exit(0);
	  }
	} else {
	  int result = tcp::recv(connfd, buffer, MAXLINE, 0);
	  if(result > 0) {
	    event.events = EPOLLOUT;
	    epoll_ctl(efd, EPOLL_CTL_MOD, connfd, &event);
	  } else if(result == 0) {
	    // STUB
	    cout << "[in read] result == 0" << endl;
	    tcp::shutdown(connfd, 2);
	    epoll_ctl(efd, EPOLL_CTL_DEL, connfd, NULL);
	  } else if(result < 0) {
	    tcp::shutdown(connfd, 2);
	    epoll_ctl(efd, EPOLL_CTL_DEL, connfd, NULL);
	    perror("error in recv");
	  }
	}
	
      } else if(__event & EPOLLOUT) {
	int result = tcp::send(connfd, response, strlen(response), 0);
	if(result > 0) {
	  event.events = EPOLLIN;
	  epoll_ctl(efd, EPOLL_CTL_MOD, connfd, &event);
	} else if(result <= 0) {
	  // STUB
	  cout << "[in write] result == " << result << endl;
	  tcp::shutdown(connfd, 2);
	  epoll_ctl(efd, EPOLL_CTL_DEL, connfd, NULL);
	  perror("error in send");
	}
      }
    }
  }

  tcp::shutdown(listen_fd, 2);
  return 0;
}
