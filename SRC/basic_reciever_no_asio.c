#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstdio>
#include <stdlib.h>
//#include <unistd>
#include <string.h>


#include "classdefs.h"


int main(int argc, char* argv[]){
	if (argc < 2){
		std::cerr << "Usage: basic_reciever <port>\n";
		return 1;
  }
  
  int socket_size, n;
	struct sockaddr_in local_address, remote_address;
	struct hostent *server;
	packet p;
	int port = atoi(argv[1]);
	char buffer[256];
	bzero(buffer,256);
	
	//initialize the static route table
	RouteTable table = RouteTable();
	
	// allocate socket
	int sin = socket(PF_INET, SOCK_DGRAM, 0);
	if (sin < 0){
		//error("Socket Error\n");
		cout << "socket error\n";
	}
	
	// bind socket to UDP port
	memset(&local_address, 0, sizeof(local_address));
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(port);
	local_address.sin_addr.s_addr=INADDR_ANY;
	
	socket_size = sizeof(local_address);
	if (bind(sin,(struct sockaddr *)&local_address,socket_size)<0) {
		//error("binding");
		cout<<"error binding\n";
	}
	
	unsigned int packet_length = sizeof(struct sockaddr_in);
	cout << "waiting for message\n";
	while(1){
		//recieve packet
		n = recvfrom(sin,buffer,256,0,(struct sockaddr *)&remote_address,&packet_length);
		if (n < 0){
			//error("recvfrom");
			cout<<"error recieving\n";
		}
		
		//rebuild packet
		//packet p = packet(buffer);
		
		//print data
		printf("data recieved:%s -to: %s", p.data, p.dest.addr); 
		
	}
	
	close(sin);
	return 0;
}
