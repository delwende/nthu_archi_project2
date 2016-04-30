#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simulator.h"
#include "convert.h"
#include "readerfile.h"
#include "execution.h"


int main(void){

	  memset( reg, 0, sizeof(reg) );
	   memset( i_memory, 0, sizeof(i_memory) );
	    memset( d_memory, 0, sizeof(d_memory) );
	

	

	open_Files();	

	read_IImage(i_memory,PC);
  	read_DImage(d_memory,sp);

	execute(); 


	close_Files();

	
	
	return 0;
}

