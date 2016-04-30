#include <stdio.h>
#include <stdlib.h>

#include "convert.h"

void change_Order(char* buf){
	int i , j ,k=0;
	char temp[32];
	for(i=0;i<32;i++){
		temp[i] = buf[i];
	}
	for(i=3;i>=0;i--){
		for(j=0;j<8;j++){
			buf[k] = temp[i*8+j];
			k++;
		}
	}

}

/*unsigned int toHex(char* buff){

	int i,pow=1;
	unsigned int re=0;
	for(i=31;i>=0;i--){
		//printf("i = %d , add %d , cur re = %d\n",i,buff[i]*pow,re);
		re = re + (buff[i]-'0')*pow;
		pow = pow * 2 ;
	}
	return re;
}
*/
int read_32_Bits(char * buf,FILE* file){
	unsigned int raw;
	int i;

	if( !fread(&raw,sizeof(unsigned int),1,file) ) return 0;

	//printf("%d\n",raw);
	for(i=31;i>=0;i--){
		buf[i]='0' + raw%2;
		//printf("%d ",raw);
		raw = raw / 2 ;
		//printf("%d ",raw);
	}

	buf[32]='\0';

	//printf("%s  before\n",buff);
	change_Order(buf);
	//unsigned int hex = toHex(buff);

	return 1;
	
}


