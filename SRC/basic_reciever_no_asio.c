#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstdio>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>

#include "functiondefs.h"

int main(int argc, char* argv[]){
	if (argc < 4){
		std::cerr << "Usage: basic_reciever <my address> <forwarding address> <port>\n";
		return 1;
	}
	
	address my_address = address(argv[1]);
	address next_address = address(argv[2]);
	
	int n;
	socklen_t socket_size;
	struct sockaddr_in local_address, remote_address;
	struct hostent *server;
	socket_size = sizeof(sockaddr_in);
	packet p;
	int port = atoi(argv[3]);
	char buffer[512];
	bzero(buffer,512);
	
	//initialize the static route table
	RouteTable table = RouteTable();
	staticRoutes(&table,next_address);
	
	
	// allocate socket

	
	// bind socket to UDP port
	int sin =  allocateListenSocket(port, &local_address);
	
	while(1){
		//recieve packet
		cout << "waiting for message\n";
			
			n = recv(sin,&p,&remote_address);
			//forward the packet if not for us
			if(n>0){
				cout<<"no error recieving\n";
				if(p.dest.equals(&my_address)){
					//THIS ONE FOR ME
					printf("data recieved:%s -to: %s\n", p.data, p.dest.addr);
					
					//get the next address from the table
					address next = table.getToAddress(p.dest);
			
					//make the packet
					packet q = packet(						//construct packet
						ACK,
						p.dest,
						p.source,
						p.size,
						p.data,
						0);
					
					//send an ACK
					sendPacket(port, &q, &remote_address, next);
				}else{
					//NOT FOR ME
					printf("Not my data, for: %s\n", p.dest.addr); 
					forwardTo(table, p, port);
				}
			}

	}

	close(sin);
	return 0;
}
