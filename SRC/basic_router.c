#include <iostream>
#include "functiondefs.h"
#include "classdefs.h"

/**
* Simple utility function to send to a data to the network
*/
void recieve_packet() {
	struct sockaddr_rc local_address = { 0 };
	struct sockaddr_rc remote_address = { 0 };
	int s, bytes_read;
	
	// allocate socket
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		perror("Error opening socket");
	
	local_address.rc_family = AF_INET;
	local_address.bdaddr = 
	
}

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
	serv_addr.sin_port = htons(UNICAST_PORTNO);

	if ((numbytes = sendto(s, packet, sizeof(*packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) == -1) {
		perror("sendto");
	}

	close(s);
}

/**
* Forwards a packet to a destination 
*/
void forward_packet(PACKET *packet){

	PACKET new_packet;
	new_packet.type = packet->type;
	new_packet.size = packet->size;
	strcpy(new_packet.dest, packet->dest);
	new_packet.current_num_ints = packet->current_num_ints++;
	strcpy(new_packet.data, packet->data);

	for (i = 0; i < current_num_ints; i++){
		strcpy(new_packet.intermediate_nodes[i], packet->intermediate_nodes[i]);
	}

	send_to_network(packet->dest, (char *)&new_packet, sizeof(new_packet));
	
}

int main(int argc, char* argv[]){
	if (argc < 3){
			std::cerr << "Usage: basic_router <route_dest> <route_next>\n";
      return 1;
    }
	}
	//take in commandline arguments for easy(ier) reading
	address static_route_destination = address(argv[1]);
	address static_route_next = address(argv[2]);
	
	//initialize the static route table
	RouteTable table = RouteTable();
	table.addEntry(static_route_destination , static_route_next);
	
	//initialize an incoming packet
	PACKET packet;
	
	//setup our wrapper functions
	boost::asio::io_service io_service;

	udp::socket s(io_service, udp::endpoint(udp::v4(), 0));

	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), argv[1], argv[2]);
	udp::resolver::iterator iterator = resolver.resolve(query);

	
	//keep searching for packets to recieve and retransmit
	while(true){
		packet = recieve_packet();
		forward_packet(&packet);
	}
	return 1;
}
