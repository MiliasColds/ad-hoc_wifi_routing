#include <vector>

#include <cstdio>
#include <cmath>

#include <iostream>
#include <string>

using namespace std;

class address {
public :
  short A,B,C,D;
  char addr[16];
  
	address(){ }
  address(char inaddr[16]){
		std::cerr << "hallo address constructor\n";

		strcpy(addr, inaddr);
		char *tok = strtok(addr,".");
		short x = 0;
		while(tok !=  NULL){
			short n = atoi(tok);
			switch(x) {
				case 0:
				A = n;
				break;
				case 1:
				B = n;
				break;
				case 2:
				C = n;
				break;
				case 3:
				D = n;
				break;
			}
			std::cerr << "hallo address loop\n";
			tok = strtok(NULL,".");
			x++;
		}
			std::cerr << "hallo\n";

  }
  
  short getOctet(short x){
	switch(x) {
	  case 0:
		return A;
	  case 1:
		return B;
	  case 2:
		return C;
	  case 3:
		return D;
	  }
  }
  
  bool equals(address *comp){
		if(comp->A==A &&
			comp->B == B &&
			comp->C == C &&
			comp->D == D){
			
			return true;
		}
		return false;
		
	}
};


class Table_Entry{
	public:
	address dest,next;
  
  Table_Entry(address dest_addr, address next_addr){
		dest = dest_addr;
		next = next_addr;
  }

};
 
class RouteTable {
	public:
	vector<Table_Entry> table;
	
	RouteTable(){
		table = vector<Table_Entry>();
	}
	
	void addEntry(address dest, address next){
		Table_Entry t = Table_Entry(dest,next);
		table.push_back(t);
	}
	
	address getToAddress(address dest){
		for (std::vector<Table_Entry>::iterator i = table.begin(); i != table.end(); ++i) { //Iterate through 'items'
			if((*i).dest.equals(&dest)){
				return (*i).next;
			}
		}
	}
	
	void removeEntry_bydest(address dest) {
		for (std::vector<Table_Entry>::iterator i = table.begin(); i != table.end(); ++i) { //Iterate through 'items'
			if((*i).dest.equals(&dest)){
				table.erase(i);
			}
		}
	}
	
	void removeEntry_bynext(address next) {
		for (std::vector<Table_Entry>::iterator i = table.begin(); i != table.end(); ++i) { //Iterate through 'items'
			if((*i).next.equals(&next)){
				table.erase(i);
			}
		}
	}
};

typedef enum {ACK, DAT} packettype;

class packet {
public:
	packettype type;
	address dest;
	int size;
	char data[30];
	int current_num_ints;
	
	packet(packettype pt, address d, int s, char indat[30], int n){
		type = pt;
		dest = d;
		size = s;
		strncpy(data, indat, 30);
		current_num_ints = n;
	}
	packet(string buffer){
		char temp[30];
		//strncpy(temp, buffer, 
		
	}
	char* tocharstar(){
		char* chartype;
		chartype = (char*)&type;
		
		char chardest[16];
		strncpy(chardest, dest.addr, 16);
		
		char* charsize;
		charsize = (char*)&size;
		
		char chardata[30];
		strncpy(chardata, data, 30);
		
		char* charn;
		charn = (char*)&current_num_ints;
		
		char* out = (char*)malloc(49*sizeof(char));
		strcpy(out, chartype);
		strcat(out, chardest);
		strcat(out, charsize);
		strcat(out, chardata);
		strcat(out, charn);
		
		return out;
	}
	
};

typedef struct{
	packettype type;			//type of packet ACK or DATA
	char dest[16];			//destination
	int size;				//size of data field
	char data[30];		//maximum size of data payload
	int current_num_ints;		//number of intermediate nodes
						//traversed by packet
	char intermediate_nodes[8][16]; //array to hold ip
// addresses of intermediate nodes
} PACKET;
