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


int recv(int sock, packet *p, sockaddr_in *fromaddr){
	socklen_t socket_size = sizeof(sockaddr_in);
	cout << "waiting for message\n";
	int n = recvfrom(sock,(void*)p,sizeof(packet),MSG_PEEK,
				(struct sockaddr *)fromaddr,&socket_size);
	//n = recv(sin,buffer,512*sizeof(char),MSG_PEEK);
	if( n > 0 ){
		//recv(sin,buffer,512*sizeof(char),0);
		n = recvfrom(sock,(void*)p,sizeof(packet),0,
				(struct sockaddr *)fromaddr,&socket_size);
		
		cout<<"no error recieving\n";
		
		
	}else{
		cout<<"error recieving\n";
	}
	return n;
}


int allocateListenSocket(int port, struct sockaddr_in* local_address){
	// allocate socket
	int sin = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sin < 0){
		cout << "socket error\n";
	}
	
	//setup socket
	memset(local_address, 0, sizeof(sockaddr_in));
	local_address->sin_family = AF_INET;
	local_address->sin_port = htons(port);
	local_address->sin_addr.s_addr= htonl(INADDR_ANY);
	cout << "binding on port " << port << "\n";
	// bind socket to UDP port
	if ( bind( sin,(struct sockaddr*)local_address,sizeof(sockaddr_in) ) <0 ) {
		cout<<"error binding\n";

	}
	return sin;
  }
//int allocate socket_send()

int sendPacket(int port, packet* p, sockaddr_in* dest, address next){
	
	//allocate socket
	int sout = socket(AF_INET, SOCK_DGRAM, 0);
	if (sout < 0){
		cout<<"socket create failed\n";
		//error("Socket Error\n");
	}
	
	// setup socket
	
	dest->sin_family = AF_INET;
	dest->sin_port = htons(port);
	//remote_address.sin_addr.s_addr = inet_addr(string_route_destination);
	inet_aton(next.addr, &(dest->sin_addr));
	
	socklen_t remote_length = sizeof(struct sockaddr_in);
	
	cout << "packet:"<< p->data << ", " << p->dest.addr << "\n";

	//actually send the packet
	int n = sendto(sout,(void*)p,sizeof(packet),0,
					(struct sockaddr *)dest, sizeof(struct sockaddr_in));
	if (n < 0){												//error handling
		cout<<"send error\n";
	}
	cout << "data sent\n";						//notification
	
	//close socket
	close(sout);

	return n;
}

//blocking receive packet(int socket, char*, sockaddr*)

