#include <stdio.h>
#include <stdlib.h>

#include "operation.h"
#include "execution.h"

void NOT_funt(int* input){
	int i;
	for(i=0;i<32;i++){
		input[i]= (input[i]+1)%2;
	}
}
int adder_funt(int a,int b , int* c){
	int sum = a + b + *c;
	if(sum > 1 ) *c = 1;
	else *c = 0;
	return sum%2;
	
}
void pc_ADD(int* result,int* input1,int* input2, bool* error_buffer){
	int i;
	int c=0;

	int temp1 = input1[0];
	int temp2 = input2[0];

	for(i=31;i>=0;i--){
		result[i] = adder_funt(input1[i],input2[i],&c);
	}
	// overflow
	if(temp1==temp2 ){
		if(temp1!=result[0] )error_buffer[0] = error_buffer[2] = true;
	} 

}

void ADDU_funt(int* result,int* input1,int* input2){

	bool bhandler[6];
	pc_ADD(result,input1,input2,bhandler);

}

void SUB_funt(int* result,int* input1,int* input2, bool* error_buffer){
	int one[32]={0};
	int temp[32];
	bool bhandler[6];	

	one[31]=1;
	NOT_funt(input2);
	pc_ADD(temp,input2,one,bhandler);
	int sign1=input1[0],sign2=temp[0];

	pc_ADD(result,input1,temp,bhandler);
	if(sign1==sign2){
		if( result[0]!=sign1)
		error_buffer[0] = error_buffer[2] = 1;
	}

}
void AND_funt(int* result,int* input1,int* input2){
	int i;
	for(i=0;i<32;i++){
		result[i] = input1[i] & input2[i];
	}
}
void OR_funt(int* result,int* input1,int* input2){
	int i;
	for(i=0;i<32;i++){
		result[i] = input1[i] | input2[i];
	}
}
void XOR_funt(int* result,int* input1,int* input2){
	int i;
	for(i=0;i<32;i++){
		result[i] = input1[i] ^ input2[i];
	}
}
void NOR_funt(int* result,int* input1,int* input2){
	OR_funt(result,input1,input2);
	NOT_funt(result);
}
void NAND_funt(int* result,int* input1,int* input2){
	AND_funt(result,input1,input2);
	NOT_funt(result);
}
void SLT_funt(int* result,int* input1,int* input2){
	int i;
	int a = Set_Int(input1,32);
	int b = Set_Int(input2,32);

	for(i=0;i<32;i++){
		result[i] = 0;
	}
	if(a<b) result[31]=1;
}
void SLL_funt(int* result,int* input2,int* sh){
	int shamt = change_to_UInt(sh,5);
	int i;
	
	for(i=0;i<32;i++){
		if(i+shamt<32) result[i] = input2[i+shamt];
		else result[i] = 0;
	}

}
void SRL_funt(int* result,int* input2,int* sh){
	int shamt = change_to_UInt(sh,5);
	int i;

	for(i=0;i<32;i++){
		if(i-shamt<0) result[i] = 0;
		else result[i]=input2[i-shamt];
	}
}
void SRA_funt(int* result,int* input2,int* sh){
        int shamt = change_to_UInt(sh,5);
        int i;
	int temp = input2[0];

        for(i=0;i<32;i++){
                if(i-shamt<0) result[i] = temp;
                else result[i]=input2[i-shamt];
        }


}
void LUI_funt(int* result, int* input2){
	int i;
	for(i=0;i<16;i++){
		result[i]=input2[16+i];
	}
	for(i=16;i<32;i++) result[i]=0;
}

void JR_funt(int* PC,int* input1){
	int i;
	for(i=0;i<32;i++){
		PC[i] = input1[i];
	}
}


