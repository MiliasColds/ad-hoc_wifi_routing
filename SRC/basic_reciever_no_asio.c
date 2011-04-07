#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstdio>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>

#include "classdefs.h"

int main(int argc, char* argv[]){
	if (argc < 2){
		std::cerr << "Usage: basic_reciever <port>\n";
		return 1;
	}

	int n;
	socklen_t socket_size;
	struct sockaddr_in local_address, remote_address;
	struct hostent *server;
	socket_size = sizeof(sockaddr_in);
	packet p;
	int port = atoi(argv[1]);
	char buffer[512];
	bzero(buffer,512);

	//initialize the static route table
	RouteTable table = RouteTable();

	// allocate socket
	int sin = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sin < 0){
		cout << "socket error\n";
	}

	// bind socket to UDP port
	memset(&local_address, 0, socket_size);
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(port);
	local_address.sin_addr.s_addr= htonl(INADDR_ANY);

	if ( bind( sin,(struct sockaddr*)&local_address,sizeof(local_address) ) <0 ) {
		cout<<"error binding\n";
	}


	while(1){
		//recieve packet
		cout << "waiting for message\n";
		n = recvfrom(sin,(void*)&p,sizeof(packet),MSG_PEEK,
					(struct sockaddr *)&remote_address,&socket_size);
		//n = recv(sin,buffer,512*sizeof(char),MSG_PEEK);
		if( n > 0 ){
			//recv(sin,buffer,512*sizeof(char),0);
			n = recvfrom(sin,(void*)&p,sizeof(packet),0,
					(struct sockaddr *)&remote_address,&socket_size);
			//printf("data recieved:%s -to: \n", buffer);
			cout<<"no error recieving\n";
			printf("data recieved:%s -to: %s\n", p.data, p.dest.addr); 

		}else{
			cout<<"error recieving\n";
		}
		cout<<"waiting\n";
		//rebuild packet
		//packet p = packet(buffer);

		//print data
		//printf("data recieved:%s -to: %s\n", p.data, p.dest.addr); 
	}

	close(sin);
	return 0;
}
