#include <iostream>
#include <boost/asio.hpp>
#include "classdefs.h"

using boost::asio::ip::udp;

int main(int argc, char* argv[]){
	if (argc < 3){
		std::cerr << "Usage: basic_sender <route_dest> <route_next>\n";
		return 1;
  }
  
	char string_route_destination[16];
	strncpy(string_route_destination, argv[1], 16);
	char string_route_next[16];
	strncpy(string_route_next, argv[2], 16);
	
		//take in commandline arguments for easy(ier) reading
	address static_route_destination = address( string_route_destination );
	address static_route_next = address( string_route_next );
	
	//initialize the static route table
	RouteTable table = RouteTable();
	table.addEntry(static_route_destination, static_route_next);
	
	//make the packet
	char data[30] = "the good the bad and the ugly";
	size_t request_length = strlen(data);
	
	packet p = packet(
		DAT,
		static_route_destination,
		30,
		data,
		0);

	//setup our network wrapper
	boost::asio::io_service io_service;

	//let's get it on!
	udp::socket s(io_service, udp::endpoint(udp::v4(), 0));
	
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), argv[1], argv[2]);
	udp::resolver::iterator iterator = resolver.resolve(query);
	
	char* pstr = p.makestring();
	
	s.send_to(boost::asio::buffer((void*)pstr, request_length), *iterator);

}
