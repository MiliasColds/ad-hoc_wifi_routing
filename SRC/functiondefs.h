#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "classdefs.h"

using namespace std;

void staticRoutes(RouteTable* rt, address next){
	char hosts[6][16] = {"192.168.5.66", "192.168.5.74", "192.168.5.76", "192.168.5.95", "192.168.5.118", "192.168.5.131"};
	for( int i = 0; i < 6; i++){
		address tmp = address(hosts[i]);
		rt->addEntry(tmp,next.addr);
	}
	
	return;
}

void forwardTo(RouteTable t,packet p, int port){
  
  struct sockaddr_in local_address, remote_address;
	struct hostent *server;
	
	char string_route_destination[16];
	strncpy(string_route_destination, p.dest.addr, 16);
	char string_route_next[16];


	char buffer[256];
	bzero(buffer,256);
	
	//take in commandline arguments for easy(ier) reading
	address static_route_destination = address( string_route_destination );
	address static_route_next = address( string_route_next );
	
	static_route_next = t.getToAddress(static_route_destination);
	
	// allocate socket
	int sout = socket(AF_INET, SOCK_DGRAM, 0);
	if (sout < 0){
		cout<<"socket create failed\n";
		//error("Socket Error\n");
	}
	
	// bind socket to UDP port
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(port);
	//remote_address.sin_addr.s_addr = inet_addr(string_route_destination);
	inet_aton(static_route_next.addr, &remote_address.sin_addr);
	
	socklen_t remote_length = sizeof(struct sockaddr_in);
	
	//char* charpacket = p.tocharstar();
	
	//int n = sendto(sout,(void*)&p,sizeof(packet),0,
	//				(struct sockaddr *)&remote_address, sizeof(struct sockaddr_in));
	cout << "packet:"<<p.data <<", "<<p.dest.addr<<"\n";
	packet *j = &p;
	int n = sendto(sout,(void*)j,sizeof(packet),0,
					(struct sockaddr *)&remote_address, remote_length);
	if (n < 0){
		//error("Sendto Error\n");
		cout<<"send error\n";
	}
	cout << "data sent\n";
	close(sout);
}