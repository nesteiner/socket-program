#pragma once
#include <bits/types/struct_timeval.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/un.h>
#include <string.h>
#include <iostream>
#include <exception>
#include <string>
#include <errno.h>
#include <memory>
#include <queue>
namespace tcp {
  /*
    1. deploy
    2. connect
    3. transport
    4. close

    ATTENTION consider network first
  */

  // deploy
  int server(const char * ip, unsigned int port);
  int client(int domain=AF_INET);

  // connect
  int accept(int serverfd);
  int connect(int sockfd, const char * ip, unsigned int port);

  // transport
  const auto send = ::send;
  const auto recv = ::recv;

  // close
  const auto shutdown = ::shutdown;

  // set timeout
  void set_timeout(int connfd, struct timeval tv);
};


