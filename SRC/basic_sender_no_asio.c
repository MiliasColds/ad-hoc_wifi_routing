#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "classdefs.h"


int main(int argc, char* argv[]){
	if (argc < 3){
		std::cerr << "Usage: basic_sender <route_dest> <route_next> <port>\n";
		return 1;
  }
  
	struct sockaddr_in local_address, remote_address;
	struct hostent *server;
	char string_route_destination[16];
	strncpy(string_route_destination, argv[1], 16);
	char string_route_next[16];
	strncpy(string_route_next, argv[2], 16);

	int port = atoi(argv[3]);
	char buffer[256];
	bzero(buffer,256);
	
	//take in commandline arguments for easy(ier) reading
	address static_route_destination = address( string_route_destination );
	address static_route_next = address( string_route_next );
	
	//initialize the static route table
	RouteTable table = RouteTable();
	table.addEntry(static_route_destination, static_route_next);
	server = gethostbyname(string_route_next);
	//make the packet
	char data[30] = "the good the bad and the ugly";
	size_t request_length = strlen(data);
	
	packet p = packet(
		DAT,
		static_route_destination,
		30,
		data,
		0);
	
	// allocate socket
	int sout = socket(AF_INET, SOCK_STREAM, 0);
	if (sout < 0) error("Socket Error\n");
	
	// bind socket to UDP port
	remote_address.in_family = AF_INET;
	remote_address.in_port = htons(portno);
	bcopy((char *)server->h_addr, 
         (char *)&remote_address.sin_addr.s_addr,
         server->h_length);
	 if (connect(sout,(struct sockaddr_in *) &remote_address,sizeof(remote_address)) < 0) 
		error("ERROR connecting");
	unsigned int remote_length = sizeof(struct sockaddr_in);
	
	int n = write(sout,packet,sizeof(packet));//,0,
					//(struct sockaddr *)&remote_address, &remote_length);
	if (n < 0) error("Sendto Error\n");
	
	close(sout);
}
