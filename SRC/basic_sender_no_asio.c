#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "functiondefs.h"


int main(int argc, char* argv[]){
	if (argc < 5){
		std::cerr << "Usage: basic_sender <route_dest> <route_next> <my address> <port>\n";
		std::cerr << "or: basic_sender <route_dest> <route_next> <my address> <port> -f\n";

		return 1;
	}
	
	struct sockaddr_in local_address, remote_address;
	
	char string_route_destination[16];
	strncpy(string_route_destination, argv[1], 16);
	char string_route_next[16];
	strncpy(string_route_next, argv[2], 16);
	
	address my_address = address(argv[3]);
	
	int port = atoi(argv[4]);
	char buffer[256];
	bzero(buffer,256);
	
	//take in commandline arguments for easy(ier) reading
	address static_route_destination = address( string_route_destination );
	address static_route_next = address( string_route_next );
	
	//initialize the static route table
	RouteTable table = RouteTable();
	table.addEntry(static_route_destination, static_route_next);
	
	//beginning of -f option code (MAIN WIFI LAB CODE)
  if(argc > 5){
		if(strcmp(argv[5],"-f")==0){
			//read file instead of read text
			cin >> buffer;
			
			//open file for reading
			FILE *fp = fopen(argv[4],"r");
			
			int count = 0;
			int ch = getc(fp);
			
			//file loop - send packet, clear buffer, wait for recieve
			while(ch!=EOF){
				if(count == 30){
					
					//make the packet
					packet p = packet(						//construct packet
						DAT,
						static_route_destination,
						strlen(buffer),
						buffer,
						0);
						
					cout << "packet:"<<p.data <<", "<<p.dest.addr<<"\n";
					packet *j = &p;
					
					//get the next address from the table
					address next = table.getToAddress(p.dest);
					//remote_address.sin_port = htons(port);
					//actually send the packet
					sendPacket( port, j, &remote_address, next);
					
					//grab a socket to listen on
					int sin = allocateListenSocket(port, &local_address);
					
					while(1){
						//recieve packet
						cout << "waiting for message\n";
							
						int n = recv(sin,&p,&remote_address);
						//forward the packet if not for us
						if(n>0){
							if(p.dest.equals(&my_address)){
								//THIS ONE FOR ME
								printf("data recieved:%s -to: %s\n", p.data, p.dest.addr);
								if(p.type == ACK){
									//THIS IS WHAT WE'VE BEEN WAITING FOR OUR ENTIRE LIVES
									break;
								}
							}else{
								//NOT FOR ME
								printf("Not my data, for: %s\n", p.dest.addr); 
								forwardTo(table, p, port);
							}
						}
							
					}
					//finally, got ack, continue
					
					
					count = 0;
				}
				buffer[count] = ch;
				count++;
				ch = getc(fp);
			}
			
			return 0;
		}
		
	}
	//end of -f option code
			
	//main loop: send text entered to destination specified in arguments
	
	while(1){
		cin >> buffer;								//take in text
		if( (buffer[0] == 'q') && (buffer[1] == '\0') ){
			break;											//quit on 'q' press
		}
		size_t request_length = strlen(buffer);
		
		//make the packet
		packet p = packet(						//construct packet
			DAT,
			static_route_destination,
			request_length,
			buffer,
			0);
			
		packet *j = &p;
		
		//get the next address from the table
		address next = table.getToAddress(p.dest);
		
		//send that thang
		sendPacket( port, &p, &remote_address, next);
	}
	
	return 0;
}
