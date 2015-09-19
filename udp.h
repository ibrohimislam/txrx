#ifndef _UDP_H_
#define _UDP_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iomanip>
#include <iostream>
#include <string>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>

using namespace std;

class udp {
public:
  udp();
  udp(int port);
  ~udp();

  void setTarget(char *targetHost, int targetPort);
  char rxchar();
  void txchar(char c);
  int getLocalPort();
private:
  struct sockaddr_in *localAddress;
  struct sockaddr_in *remoteAddress;

  int *sock;
};

udp::udp(int port) {
  sock = new int(0);
  localAddress = new sockaddr_in();
  remoteAddress = new sockaddr_in();

  memset(localAddress, 0, sizeof(*localAddress));
  (*localAddress).sin_family=AF_INET;
  (*localAddress).sin_addr.s_addr=htonl(INADDR_ANY);
  (*localAddress).sin_port=htons(port);

  if ((*sock=socket(AF_INET, SOCK_DGRAM, 0))<0) {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }

  if (bind(*sock, (struct sockaddr *)localAddress, sizeof(*localAddress)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
}

udp::~udp(){
  close(*sock);

  delete sock;
  delete localAddress;
  delete remoteAddress;
}

void udp::setTarget(char *targetHost, int targetPort) {
  inet_pton(AF_INET,targetHost,&((*remoteAddress).sin_addr.s_addr));
  (*remoteAddress).sin_port=htons(targetPort);
}

char udp::rxchar() {
  struct sockaddr_in packetRemoteAddress;
  socklen_t addrlen = sizeof(packetRemoteAddress);

  char *buf = new char();
  int recvlen = recvfrom(*sock, buf, 1, 0, (struct sockaddr *) &packetRemoteAddress, &addrlen);
  return buf[0];
}

void udp::txchar(char c) {
  sendto(*sock, &c, 1, 0, (struct sockaddr*) remoteAddress, sizeof(*remoteAddress));
}

int udp::getLocalPort(){
  struct sockaddr_in cLocalAddress;
  socklen_t addrlen = sizeof(cLocalAddress);

  if(getsockname(*sock, (struct sockaddr *)&cLocalAddress, &addrlen) == 0 &&
     cLocalAddress.sin_family == AF_INET &&
     addrlen == sizeof(cLocalAddress)) {
    
    return ntohs(cLocalAddress.sin_port);
  }

  return 0;
}

#endif
