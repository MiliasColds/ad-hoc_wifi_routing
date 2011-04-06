#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstdio>
#include <stdlib.h>
#include <unistd>
#include <string>


#include "classdefs.h"


int main(int argc, char* argv[]){
	if (argc < 2){
		std::cerr << "Usage: basic_reciever <port>\n";
		return 1;
  }
  
  int socket_size, n;
	struct sockaddr_in local_address, remote_address;
	struct hostent *server;
	
	int port = atoi(argv[2]);
	char buffer[256];
	bzero(buffer,256);
	
	//initialize the static route table
	RouteTable table = RouteTable();
	
	// allocate socket
	int sin = socket(AF_INET, SOCK_STREAM, 0);
	if (sin < 0){
		//error("Socket Error\n");
	}
	
	// bind socket to UDP port
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(port);
	remote_address.sin_addr.s_addr=INADDR_ANY;
	
	socket_size = sizeof(remote_address);
	if (bind(sin,(struct sockaddr *)&server,socket_size)<0) {
		//error("binding");
	}
	
	unsigned int packet_length = sizeof(struct sockaddr_in);
	
	while(1){
		//recieve packet
		n = recvfrom(sin,buffer,sizeof(packet),0,(struct sockaddr *)&remote_address,&packet_length);
		if (n < 0){
			//error("recvfrom");
		}
		
		//rebuild packet
		packet p = packet(buffer);
		
		//print data
		printf("data recieved:%s -from: %s", p.data, p.dest.addr); 

	}
	
	close(sin);
	return 0;
}
