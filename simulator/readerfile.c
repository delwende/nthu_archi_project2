#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "operation.h"
#include "simulator.h"
#include "convert.h"
#include "readerfile.h"

//FILE* _assembly;

FILE* snapshot;
FILE* error;
/**
    decoder for unsigned binaries
**/
unsigned int unsigned_Decoder(int* a , int len){
	int i,digit = 1;
	int amt = 0;

	for(i=len-1;i>=0;i--){
		amt = amt + a[i] * digit;
		//printf("add %d , current amt = %d\n",a[i]*digit , amt);
		digit = digit * 2;
	}
	return amt;
}

/**
    decoder for signed binaries
**/
int signed_Decoder(int*a , int len){
        int i,digit = 1;
        int amt = 0;

	if(a[0]==0){ // pos
        	for(i=len-1;i>0;i--){
                	amt = amt + a[i] * digit;
                	//printf("add %d , current amt = %d\n",a[i]*digit , amt);
                	digit = digit * 2;
        	}
		return amt;
	}
	else{ // neg
		for(i=len-1;i>0;i--){
			amt = amt + (  (a[i]+1)%2 ) * digit;
			digit = digit * 2;
		}
		amt = -(amt+1);
		return amt;
	}

}

void RType(int f){

	switch(f){
		case 0x20://add
			fprintf(snapshot,"ADD");
			break;
		case 0x21://addu
			fprintf(snapshot,"ADDU");
                        break;
		case 0x22://sub
			fprintf(snapshot,"SUB");
                        break;
                case 0x24://and
			fprintf(snapshot,"AND");
                        break;
                case 0x25://or
			fprintf(snapshot,"OR");
                        break;
                case 0x26://xor
			fprintf(snapshot,"XOR");
                        break;
                case 0x27://nor
			fprintf(snapshot,"NOR");
                        break;
                case 0x28://nand
			fprintf(snapshot,"NAND");
                       break;
                case 0x2A://slt
			fprintf(snapshot,"SLT");
                        break;
                case 0x00://sll
			fprintf(snapshot,"SLL");
                        break;
                case 0x02://srl
			fprintf(snapshot,"SRL");
                        break;
                case 0x03://sra
			fprintf(snapshot,"SRA");
                        break;
                case 0x08://jr
			fprintf(snapshot,"JR");
                        break;
		default:
			printf("exception: %x\n",f);
	}

}


/**
    translate binaries(char array) to integer for all forms of register segments
**/
/** read32Bits need fix
void translateInputToInt(int* o, int* sS, int* sU, int* tS, int* tU, int* dS, int* dU, int* shS, int* shU, int* f, int* imS , int* imU){
	int i;

	char binRaw[33];
	char arg[100];

	int bin[32];
	int op[6] , opNum;
	int rs[5] , rsNumU , rsNumS;
	int rt[5] , rtNumU , rtNumS;
	int rd[5] , rdNumU , rdNumS;
	int shamt[5] , shamtNumU , shamtNumS;
	int func[6] , funcNum;
	int immediate[16] , immediateU , immediateS;


		if(read32Bits(binRaw)==0 ) printf("read file fail!\n");

		// trans char to int and verify each col
		for(i=0;i<32;i++){
			bin[i] = binRaw[i] - '0';
			if(i<6) op[i]=bin[i];
			else if(i<11) rs[i-6] = bin[i];
			else if(i<16) rt[i-11] = bin[i];
			else if(i<21) rd[i-16] = bin[i];
			else if(i<26) shamt[i-21] = bin[i];
			else func[i-26] = bin[i];
		}

		*o   = opNum = unsignedDecoder(op , 6);
		*sS  = rsNumS = signedDecoder(rs , 5);
		*sU  = rsNumU = unsignedDecoder(rs,5);
		*tS  = rtNumS = signedDecoder(rt , 5);
		*tU  = rtNumU = unsignedDecoder(rt,5);
		*dS  = rdNumS = signedDecoder(rd , 5);
		*dU  = rdNumU = unsignedDecoder(rd,5);
		*shS = shamtNumS = signedDecoder(shamt , 5);
		*shU = shamtNumU = unsignedDecoder(shamt,5);

		*f = funcNum = unsignedDecoder(func , 6);
		for(i=0;i<16;i++){
			if(i<5){
				immediate[i] = rd[i];
			}
			else if(i<10){
				immediate[i] = shamt[i-5];
			}
			else{
				immediate[i] = func[i-10];
			}
		}

		*imS = immediateS = signedDecoder(immediate , 16);
		*imU = immediateU = unsignedDecoder(immediate , 16);

		return;
}
**/


void derive_Function(instruc* in){

	int opNum = unsigned_Decoder(in->opcode,6);
	int funcNum = unsigned_Decoder(in->funct,6);
	//printf("funcNum = %d\n",funcNum);

	int i,flag=0;
	
	//for(i=0;i<32;i++) printf("%d ",in->inst[i]);
	//printf("\n");

	for(i=0;i<32;i++){
		if(i>=6 && i<=10) continue;
		if(in->inst[i]!=0){
			flag = 1;
			break;
		}
	}

	if(flag==0){
		fprintf(snapshot,"NOP");
		//printf("NOP\n");
		return ;
	}

        switch(opNum){
			case 0  :
				RType(funcNum);
				break;
			// IType

			case 8  :
				fprintf(snapshot,"ADDI");//addi();
				//printf("addi   %s\n",printArg(arg,rtNumU,rsNumU,immediateS,3) );
				break;
			case 9  : //addiu();
				fprintf(snapshot,"ADDIU");
				break;
			case 35 : //lw();
				fprintf(snapshot,"LW");
				break;
			case 33 : //lh();
				fprintf(snapshot,"LH");
				break;
			case 37 : //lhu();
				fprintf(snapshot,"LHU");
				break;
			case 32 : //lb();
				fprintf(snapshot,"LB");
                                break;
			case 36 : //lbu();
				fprintf(snapshot,"LBU");
                                break;
			case 43 : //sw();
				fprintf(snapshot,"SW");
                                break;
			case 41 : //sh();
				fprintf(snapshot,"SH");
                                break;
			case 40 : //sb();
				fprintf(snapshot,"SB");
                                break;
			case 15 : //lui();
				fprintf(snapshot,"LUI");
				break;
			case 12 : //andi();
				fprintf(snapshot,"ANDI");
				break;
			case 13 : //ori();
				fprintf(snapshot,"ORI");
                                break;
			case 14 : //nori();
				fprintf(snapshot,"NORI");
                                break;
			case 10 : //slti();
				fprintf(snapshot,"SLTI");
                                break;
			case 4  : //beq();
				fprintf(snapshot,"BEQ");
				break;
			case 5  : //bne();
				fprintf(snapshot,"BNE");
                                break;
			case 7  : //bgtz();
				fprintf(snapshot,"BGTZ");
                                break;
			// JType
			case 2  : //j();
				fprintf(snapshot,"J");
				break;
			case 3  : //jal();
				fprintf(snapshot,"JAL");
				break;

			// SType

			case 63 : //halt();
				fprintf(snapshot,"HALT");
				break;
			default : printf("exception instruction : opNum = %d \n",opNum);
		}
	//printf("flg\n");

}

void get_Fir_And_Sec(int* fir , unsigned int *sec , FILE* f){
	char buff[33];
	int intBuff[33];
	int i;

	read_32_Bits(buff,f);
	for(i=0;i<32;i++){
		fir[i]=buff[i]-'0';
	}

	read_32_Bits(buff,f);
        for(i=0;i<32;i++){
                intBuff[i]=buff[i]-'0';
        }
        
        *sec = unsigned_Decoder(intBuff,32);

}

void open_Files(void){

	
	snapshot = fopen("snapshot.rpt","w");
	error = fopen("error_dump.rpt","w");
}

void close_Files(void){
	
	fclose(snapshot);
	fclose(error);

}

void read_IImage(int buff[][32] , int *PC){
	FILE* iimage = fopen("iimage.bin","rb");

	int i,j;
	
	char raw[33];
	get_Fir_And_Sec(PC ,&instCount , iimage);

	unsigned int PcNum = unsigned_Decoder(PC,32);
	int pivot = PcNum/4;

	for(i=0;i<instCount;i++){
		read_32_Bits(raw,iimage);
		for(j=0;j<32;j++){
			buff[i+pivot][j] = raw[j] - '0';
      
		}

	}

fclose(iimage);
	//printf("Read Iimage Done ! \n");
 	//return 0; 
}
void read_DImage(int buff[][32],int *sp){
FILE* dimage = fopen("dimage.bin","rb");
	int i,j;
	unsigned int  wordCount;
	char raw[33];
  
	get_Fir_And_Sec(sp , &wordCount , dimage);

	for(i=0;i<wordCount;i++){
		read_32_Bits(raw,dimage);
		for(j=0;j<32;j++){
			buff[i][j] = raw[j] - '0';
			//printf("%d ",buff[i][j]);
		}
		//printf("\n");
	}
  fclose(dimage);
	//printf("Read Dimage Done ! \n");
}

void register_output(  int cycle){
	
	fprintf( snapshot , "cycle %d\n",cycle);
	int i;
	unsigned int temp;

	for(i=0;i<32;i++){
		temp = unsigned_Decoder(reg[i],32);
		fprintf(snapshot, "$%02d: 0x%08X\n",i,temp);
		//printf("$%02d: 0x%08x\n",i,temp);
	}
	temp = unsigned_Decoder(PC,32);
	fprintf(snapshot, "PC: 0x%08X\n",temp);
	//printf("PC: 0x%08x\n",temp);
	//printf("  write to snapshot complete ! \n");

}
void error_output(bool * error_buffer,int cycle){
	if(error_buffer[1])fprintf(error,"In cycle %d: Write $0 Error\n",cycle);
	else if(error_buffer[3])fprintf(error,"In cycle %d: Address Overflow\n",cycle);
	else if(error_buffer[4])fprintf(error,"In cycle %d: Misalignment Error\n",cycle);
	else if(error_buffer[2])fprintf(error,"In cycle %d: Number Overflow\n",cycle);

}



void write_Stage(int* cur,instruc* ID,instruc* EX,instruc* DM,instruc* WB,int is_Stall,int forward1,int forward2, int is_Flush){

	// IF
	unsigned int temp;
	temp = unsigned_Decoder(cur,32);
	if(is_Stall){
		fprintf(snapshot, "IF: 0x%08X to_be_stalled\n",temp);
	}
	else if(is_Flush){
		fprintf(snapshot, "IF: 0x%08X to_be_flushed\n",temp);
	}
	else{
		fprintf(snapshot, "IF: 0x%08X\n",temp);
	}

	// ID
	fprintf(snapshot, "ID: ");
	derive_Function(ID);
	if(is_Stall) fprintf(snapshot, " to_be_stalled");
	else if(forward1==3) fprintf(snapshot, " fwd_EX-DM_rs_$%d fwd_EX-DM_rt_$%d",unsigned_Decoder(ID->rs,5) ,unsigned_Decoder(ID->rs,5));
	else if(forward1==1) fprintf(snapshot, " fwd_EX-DM_rs_$%d",unsigned_Decoder(ID->rs,5) );
	else if(forward1==2) fprintf(snapshot, " fwd_EX-DM_rt_$%d",unsigned_Decoder(ID->rt,5) );
	//fprintf(_snapshot," inst=0x%08X ",unsignedDecoder(ID->inst,32));
	fprintf(snapshot, "\n");

	// EX
	fprintf(snapshot, "EX: ");
        derive_Function(EX);
	if(forward2==3) fprintf(snapshot, " fwd_EX-DM_rs_$%d fwd_EX-DM_rt_$%d",unsigned_Decoder(EX->rs,5) ,unsigned_Decoder(EX->rs,5));
	else if(forward2==1) fprintf(snapshot, " fwd_EX-DM_rs_$%d",unsigned_Decoder(EX->rs,5) );
	else if(forward2==2) fprintf(snapshot, " fwd_EX-DM_rt_$%d",unsigned_Decoder(EX->rt,5) );
	//fprintf(_snapshot," inst=0x%08X ",unsignedDecoder(EX->inst,32));
	fprintf(snapshot, "\n");	

	// DM
	fprintf(snapshot, "DM: ");
        derive_Function(DM);
	//fprintf(_snapshot," inst=0x%08X ",unsignedDecoder(DM->inst,32));
	fprintf(snapshot, "\n");

	// WB
	fprintf(snapshot, "WB: ");
        derive_Function(WB);
	//fprintf(_snapshot," inst=0x%08X ",unsignedDecoder(WB->inst,32));
	fprintf(snapshot, "\n");


	fprintf(snapshot, "\n\n");

}

