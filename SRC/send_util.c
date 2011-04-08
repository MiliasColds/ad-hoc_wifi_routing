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


/**
* Simple utility function to send to a data to the network
*/
void send_to_network(char *dest, PACKET *packet, int len) {
	int s;
	struct sockaddr_in serv_addr;
	int numbytes = 0;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		perror("Error opening socket");
	
	bzero((char*) &serv_addr, sizeof(serv_addr));
	
	//set the connection parameters (who to connect to)
	serv_addr.sin_family = AF_INET;
	inet_aton(dest, &serv_addr.sin_addr);
	serv_addr.sin_port = htons(8);
	
	if ((numbytes = sendto(s, packet, sizeof(*packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) == -1) {
		perror("sendto");
	}
	
	close(s);
}

/**
* Copies a packet
*/
PACKET copy_packet(PACKET* packet){
	
	PACKET new_packet;
	new_packet.type = packet->type;
	new_packet.size = packet->size;
	strcpy(new_packet.dest, packet->dest);
	new_packet.current_num_ints = packet->current_num_ints++;
	strcpy(new_packet.data, packet->data);

	for (int i = 0; i < current_num_ints; i++){
		strcpy(new_packet.intermediate_nodes[i], packet->intermediate_nodes[i]);
	}
	
	return new_packet;
}

/**
* Makes a packet with the given information
*/
PACKET make_packet(packettype type, int size, char dest[16], char data[30], int current_num_ints){
	
	PACKET new_packet;
	new_packet.type = type;
	new_packet.size = size;
	strcpy(new_packet.dest, dest);
	new_packet.current_num_ints = current_num_ints++;
	strcpy(new_packet.data, data);
	
	for (int i = 0; i < current_num_ints; i++){
		strcpy(new_packet.intermediate_nodes[i], intermediate_nodes[i]);
	}
	
	return new_packet;
}

/**
* Forwards a packet to a destination 
*/

void forward_packet(PACKET *packet){

	PACKET new_packet = make_packet(packet->type,
																	packet->size,
																	packet->dest,
																	packet->data,
																	packet->current_num_ints,
																	packet->intermediate_nodes);
	
	send_to_network(packet->dest,
		 &new_packet, sizeof(new_packet));
	
}

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
	server = gethostbyname(string_route_next);
	//make the packet
	char data[30] = "the good the bad and the ugly";
	size_t request_length = strlen(data);
	
	PACKET p = make_packet(
		DAT,
		30,
		static_route_destination.addr,
		data,
		5,
		);

	forward_packet(&p);
		
	return 0;
}
