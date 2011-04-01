#include <vector.h>
#include <string.h>
#include <stdio.h>
#include <iostream.h>
#include <math.h>

using namespace std;
class address {
public :
  short A,B,C,D;
  char[16] addr;
  
  address(char **addr){
	strcpy(this.addr,addr*);
	char *tok = strtok(this.addr,".");
	short x = 0;
	while(strlen(tok)>0){
	short n = itof(tok);
	switch(x) {
	  case 0:
		this.A = n;
		break;
	  case 1:
		this.B = n;
		break;
	  case 2:
		this.C = n;
		break;
	  case 3:
		this.D = n;
		break;
	  }
	x++;
	}
  }
  
  short getOctet(short x){
	switch(x) {
	  case 0:
		return this.A;
	  case 1:
		return this.B;
	  case 2:
		return this.C;
	  case 3:
		return this.D;
	  }
  }
  
  bool equals(address *comp){
	if(comp->A==this.A&&
		comp->B = this.B &&
		comp->C = this.C &&
		comp->D = this.D){
		
		return true;
	}
	return false;
  
}


class Table_Entry{
  address dest,next;
  
  Table_Entry(address dest_addr, address next_addr){
	this.dest = dest_addr;
	this.next = next_addr;
  }

}
 
class RouteTable {
	vector<Table_Entry> table;
	
	RouteTable(){
		this.table = vector<Table_Entry>();
	}
	
	void addEntry(address dest, address next){
		Table_Entry t = Table_Entry(dest,next);
		this.table.push_back(t);
	}
	
	address getToAddress(address dest){
		for (std::vector<Table_Entry>::iterator i = this.table.begin(); i != this.table.end(); ++i) { //Iterate through 'items'
			if((*i).dest.equals(dest)){
				return (*i).next;
			}
		}
	}
	
	void removeEntry_bydest(address dest) {
		for (std::vector<Table_Entry>::iterator i = this.table.begin(); i != this.table.end(); ++i) { //Iterate through 'items'
			if((*i).dest.equals(dest)){
				this.table.erase(i);
			}
		}
	}
	
	void removeEntry_bynext(address next) {
		for (std::vector<Table_Entry>::iterator i = this.table.begin(); i != this.table.end(); ++i) { //Iterate through 'items'
			if((*i).next.equals(next)){
				this.table.erase(i);
			}
		}
	}
}


	
	
 
  