#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "classdefs.h"


int main(int argc, char* argv[]){
	if (argc < 4){
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
	//server = gethostbyname(string_route_next);
	//make the packet
	
	
	
	
	// allocate socket
	int sout = socket(AF_INET, SOCK_DGRAM, 0);
	if (sout < 0){
		cout<<"socket create failed\n";
		//error("Socket Error\n");
	}
	
	// setup socket to UDP port
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(port);
	//remote_address.sin_addr.s_addr = inet_addr(string_route_destination);
	inet_aton(static_route_next.addr, &remote_address.sin_addr);
	
	socklen_t remote_length = sizeof(struct sockaddr_in);
	
	//char* charpacket = p.tocharstar();
	
	//int n = sendto(sout,(void*)&p,sizeof(packet),0,
	//				(struct sockaddr *)&remote_address, sizeof(struct sockaddr_in));
	
	
	while(1){
		cin >> buffer;
		if( (buffer[0] == 'q') && (buffer[1] == '\0') ){
			break;
		}
		size_t request_length = strlen(buffer);
		
		packet p = packet(
			DAT,
			static_route_destination,
			request_length,
			buffer,
			0);
			
		cout << "packet:"<<p.data <<", "<<p.dest.addr<<"\n";
		packet *j = &p;
		int n = sendto(sout,(void*)j,sizeof(packet),0,
						(struct sockaddr *)&remote_address, remote_length);
		if (n < 0){
			//error("Sendto Error\n");
			cout<<"send error\n";
		}
		cout << "data sent\n";
	}
	
	
	close(sout);
	return 0;
}
