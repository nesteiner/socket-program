#include "../lib/tcp.h"
#include <asm-generic/socket.h>
#include <memory>
#include <sys/socket.h>

int tcp::server(const char *ip, unsigned int port) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(ip);
  address.sin_port = htons(port);

  // reuse address
  int on = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  
  ::bind(sockfd, (struct sockaddr *)&address, sizeof(address));
  ::listen(sockfd, 5);

  return sockfd;
}

int tcp::client(int domain) {
  int sockfd = socket(domain, SOCK_STREAM, 0);
  return sockfd;
}

int tcp::accept(int serverfd) {
  int connfd = ::accept(serverfd, NULL, NULL);
  return connfd;
}

int tcp::connect(int sockfd, const char *ip, unsigned int port) {
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(ip);
  address.sin_port = htons(port);

  int result = ::connect(sockfd, (struct sockaddr *)&address, sizeof(address));

  return result;
}


void tcp::set_timeout(int connfd, struct timeval tv) {
  setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
}

