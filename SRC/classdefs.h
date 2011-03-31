#include <vector.h>
#include <string.h>
#include <stdio.h>
#include <iostream.h>
#include <math.h>

class address {
public :
  short A,B,C,D;
  char[15] addr;
  
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
  
}


class Table_Entry{
  address From,To;
  