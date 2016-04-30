#include <stdio.h>
#include <stdlib.h>

#ifndef declaration_H
#define declaration_H


int cur[32]={0};
bool error_buffer[5] = {false};
	int TWO[5] = {0,0,0,1,0};
	int FOUR[32]={0};

int l2ExImme[32]={0};
	
	int is_Stall=0,forward1=0,forward2=0,is_Flush=0,stall_Rec=0;

	int nextPC[32];

	// branch and jump
	int branch_AluResult[32],jumpAddress[32];
	instruc *empty;
instruc *IF;
instruc *ID;
instruc *EX;
instruc *DM;
instruc *WB;


//                 0         5   6       10  11      15  16      20  21      25  26        31

int RA_ADDR[]   = {1,1,1,1,1};
int opMask[]	= {1,1,1,1,1,1  ,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0,0};
int rsMask[]	= {0,0,0,0,0,0  ,1,1,1,1,1  ,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0,0};
int rtMask[]	= {0,0,0,0,0,0  ,0,0,0,0,0  ,1,1,1,1,1  ,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0,0};
int rdMask[]	= {0,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0  ,1,1,1,1,1  ,0,0,0,0,0  ,0,0,0,0,0,0};
int shamtMask[]	= {0,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0  ,1,1,1,1,1  ,0,0,0,0,0,0};
int funcMask[]	= {0,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0  ,1,1,1,1,1,1};
int immeMask[]	= {0,0,0,0,0,0  ,0,0,0,0,0  ,0,0,0,0,0  ,1,1,1,1,1  ,1,1,1,1,1  ,1,1,1,1,1,1};
int LMask[]	= {0,0,0,0,0,0  ,1,1,1,1,1  ,1,1,1,1,1  ,1,1,1,1,1  ,1,1,1,1,1  ,1,1,1,1,1,1};

void slice(int* result , int* input , int f , int t){

	int i;
	int leng = t - f+ 1 ;
	for(i=leng-1;i>=0;i--){
		result[i] = input[f+i];
	}
}

unsigned int change_to_UInt(int* input , int leng){
	int i , p=1,result=0,pow=1;

	for(i=leng-1;i>=0;i--){
		result +=input[i]*pow;
		pow *= 2;
	}

	return result;
}
void mask(int* result , int* input , int* m){
	int i;
	for(i=0;i<32;i++){
		result[i] = input[i] & m[i];
	}
}
unsigned int PCtoUInt(int* pc){
	int i , pow=1,result=0;

	for(i=32-1;i>=0;i--){
		result +=pc[i]*pow;
		 pow *= 2;
	}

	return result;
}



//rs_s = toSInt(masked,6,10);
int Set_Int(int* input , int leng){
        int i , pow=1,result=0;
	if(input[0]==0){ // pos
        	for(i=leng-1;i>0;i--){
                	result +=  input[i]*pow;
                	 pow *= 2;
        	}
        	return result;
	}
	else{ // neg
		for(i=leng-1;i>0;i--){
			result = result + (input[(i)]+1)%2 * pow;
			pow *= 2;
		}
		result ++;
		result=-result;
		return result;
	}
}
void fect_32_Bits(int* target, int* source){
	int i;
	for(i=0;i<32;i++){
		target[i] = source[i];
	}
}
#endif

