#include <stdio.h>
#include <stdlib.h>

#ifndef function_H
#define function_H

void cleararray(int* target, int leng){
	
	int i;
	for(i=0;i<leng;i++) target[i]=0;
	
}


void init(instruc *reg){
	cleararray(reg->inst,32);
	cleararray(reg->opcode,6);
	cleararray(reg->rs,5);
	cleararray(reg->rt,5);
	cleararray(reg->rd,5);
	cleararray(reg->shamt,5);
	cleararray(reg->imme,16);
	cleararray(reg->exImme,32);
	cleararray(reg->funct,6);
	cleararray(reg->L,26);
	
	cleararray(reg->read_Reg1,32);
	cleararray(reg->read_Reg2,32);
	cleararray(reg->read_Mem, 32);
	cleararray(reg->register_AluResult,32);
	reg->alu_Control = 0;

	reg->dst_Of_Instruction = 0;
	reg->is_Jumping = 0;
	reg->is_Branching = 0;
	reg->is_MemRead = 0;
	reg->is_MemToReg = 0;
	reg->ALUOP=0;
	reg->is_MemWrite = 0;
	reg->is_AluSrcImme = 0;
	reg->is_RegWrite = 0;
	reg->is_SystemCall = 0;

	cleararray(reg->ra,32);
	reg->is_Jal=0;
}


int function_derive_Columes(int *current , int* opcode, int* rs, int* rt, int* rd, int* shamt, int* funct, int* imme, int* L){
	int beenmask[32];

	//op
	mask(beenmask,current,opMask);
	slice(opcode,beenmask,0,5);

	//rs
	mask(beenmask,current,rsMask);
	slice(rs,beenmask,6,10);

	//rt
	mask(beenmask,current,rtMask);
	slice(rt,beenmask,11,15);

	//rd
	mask(beenmask,current,rdMask);
	slice(rd,beenmask,16,20);

	//shamt
	mask(beenmask,current,shamtMask);
	slice(shamt,beenmask,21,25);

	//func
	mask(beenmask,current,funcMask);
	slice(funct,beenmask,26,31);

	//imme
	mask(beenmask,current,immeMask);
	slice(imme,beenmask,16,31);

	// L
	mask(beenmask,current,LMask);
	slice(L,beenmask,6,31);
	return 1;
}

void  Memory_instruction( int* cur){
	int j;
	unsigned int PCNum;
		//get PC is currently on which instruction
		PCNum =PCtoUInt(PC);
		int pivot = PCNum / 4;		
		if(pivot>255||pivot<0){
			printf("PC is out of instruction memory boundaries!\n");
			return;
		}
            
            	 for(j=0;j<32;j++){
                        cur[j] = i_memory[pivot][j];
                }
		
            
           forward1=0; forward2=0; is_Flush=0;
}

void function_controller(int* opcode, int* funct, int* dst_Of_Instruction, int* is_Jumping, int* is_Branching, int* is_MemRead, int* is_MemToReg, int* ALUOP, int* is_MemWrite, int* is_AluSrcImme, int* is_RegWrite, int* is_SystemCall){
	int op_Num = change_to_UInt(opcode,6);
	int func_Num = change_to_UInt(funct,6);
        // controls
        
	

	
	

	 if(op_Num==0){ 				////////// RType
		*dst_Of_Instruction = 1;
		*is_Jumping = 0;
			if(func_Num==_JR) *is_Jumping = 1;
        	*is_Branching = 0;
        	*is_MemRead = 0;
        	*is_MemToReg = 0;
        	*ALUOP = 2;
        	*is_MemWrite = 0;
        	*is_AluSrcImme = 0;
	        *is_RegWrite = 1;
			if(func_Num==_JR) *is_RegWrite = 0; //jr don't write regs
		*is_SystemCall =0;
	}
	else if(op_Num == 0x02 || op_Num == 0x03){ ////////// JType
                *dst_Of_Instruction = 3;
		*is_Jumping = 1;
                *is_Branching = 0;
                *is_MemRead = 0; 
                *is_MemToReg = 0;
                //*ALUOP = 3;
                *is_MemWrite = 0; 
                //*isAluSrcImme = 0;
                *is_RegWrite = 0;
		*is_SystemCall=0;
	
	}
	else if(op_Num == 0x3F){ 		////////// SType
                *dst_Of_Instruction = 0;
		*is_Jumping = 0;
                *is_Branching = 0;
                *is_MemRead = 0;
                *is_MemToReg = 0;
                *ALUOP = 0;
                *is_MemWrite = 0;
                *is_AluSrcImme = 0;
                *is_RegWrite = 0;
		*is_SystemCall = 1;

	}
	else{ 					////////// IType

		*dst_Of_Instruction = 0;
		*is_Jumping = 0;
		*is_Branching = 0;
			if(op_Num==_BEQ || op_Num==_BNE || op_Num==_BGTZ) *is_Branching=1; // beq,bne,bgtz branch
		*is_MemRead = 0;
			if(op_Num==_LW) *is_MemRead = 1;
			else if(op_Num == _LH || op_Num == _LHU) *is_MemRead = 2;
			else if(op_Num == _LB || op_Num == _LBU) *is_MemRead = 3;
		*is_MemToReg = 0;
			if(op_Num==_LW || op_Num==_LH || op_Num==_LHU || op_Num==_LB || op_Num==_LBU) *is_MemToReg = 1;
		*ALUOP = 0;
			if(op_Num==_BEQ || op_Num==_BNE || op_Num==_BGTZ) *ALUOP = 1;
			else if(op_Num==_ADDI || op_Num==_ADDIU || op_Num==_ANDI || op_Num==_ORI || op_Num==_NORI || op_Num==_SLTI || op_Num==_LUI) *ALUOP=3;
		*is_MemWrite = 0;
			if(op_Num==_SW) *is_MemWrite = 1;
			else if(op_Num==_SH) *is_MemWrite = 2;
			else if(op_Num==_SB) *is_MemWrite = 3;
		*is_AluSrcImme = 1;
			if(op_Num==_BEQ || op_Num==_BNE || op_Num==_BGTZ) *is_AluSrcImme = 0;
		*is_RegWrite = 1;
			if(op_Num==_SW  || op_Num==_SH  || op_Num==_SB  || op_Num==_BEQ  || op_Num==_BNE  || op_Num==_BGTZ  ) *is_RegWrite = 0;
		*is_SystemCall = 0;
	
	}
}

void extend_Imme(int* imme, int* exImme, int* opcode){
	int i;
	int sign = imme[0];

	int op_Num = change_to_UInt(opcode,6);

	int needSign = 1;

	if(op_Num == _ANDI || op_Num == _ORI || op_Num == _NORI ){
		needSign = 0;
	}

	for(i=0;i<32;i++){
		 if(i<16 && needSign){
			exImme[i]=sign;
		}
		else if(i<16){ // no sign extension needed
			exImme[i]=0;
		}
		
		
		else{
			exImme[i]=imme[i-16];
		}
	}
}

void read_From_Register(int reg[][32], int* rs,int* rt, int* read1, int* read2, int forward){
	int rs_Num, rt_Num,i;
	
	rs_Num = change_to_UInt(rs,5);
	rt_Num = change_to_UInt(rt,5);
	
	for(i=0;i<32;i++){
		if(forward!=1&&forward!=3)read1[i] = reg[rs_Num][i];
		if(forward!=2&&forward!=3)read2[i] = reg[rt_Num][i];
	}


}



void write_To_Register(int reg[][32], int* dest, int* dat, bool* error_buffer){
	int i ;

	
	int des_Num = change_to_UInt(dest,5);
	if(des_Num == 0){ // write to $ZERO
		error_buffer[0] = error_buffer[1] = true;
		return;
	}

	for(i=0;i<32;i++){
		reg[des_Num][i] = dat[i];
	}
}

void read_From_Mem(int d_memory[][32],int* register_AluResult,int* read_Mem, int read_Style, int* opcode, bool* error_buffer){
	int op_Num = change_to_UInt(opcode,6);
	int i  , addr , temp[32] , remain;
	int errorOccur = 0;
        addr = change_to_UInt(register_AluResult,32)/4;
        remain = change_to_UInt(register_AluResult,32)%4 * 8;

	int len = 32;
	 if(read_Style == 3){
		len = 8;
	}
	else if(read_Style == 2){
		len = 16;
	}
	
	
	//detect memory mis alignment and overflow
	if(remain % len != 0){
		error_buffer[0] = error_buffer[4] = true;
		errorOccur = 1;
	}
	if(addr<0 || addr>255 || (addr==255 && (remain+len>32) ) ){
		error_buffer[0] = error_buffer[3] = true;
		errorOccur = 1;
	}
	// late return cuz need judge two errors both
	if(errorOccur){
		return;
	}

        for(i=0;i<32;i++){
                if(i+remain<32) temp[i] = d_memory[addr][i+remain];
		else temp[i] = d_memory[addr+1][i+remain-32];
        }

	for(i=0;i<len;i++){
		read_Mem[i+32-len] = temp[i];
	}
	// add remain signs
	if(op_Num==_LHU||op_Num==_LBU){
		for(i=32-len-1;i>=0;i--) 
			read_Mem[i] = 0;
	}
	else{
		for(i=32-len-1;i>=0;i--)
			read_Mem[i] = read_Mem[32-len];
	}

}

void write_To_Mem(int d_memory[][32],int* register_AluResult,int* read_Reg2 , int write_Style, bool* error_buffer){

	// wrtire style
	//	1	full
	//	2	half
	//	3	bit

	int i  , addr , remain , error_Occur=0; 
	addr = change_to_UInt(register_AluResult,32)/4;
	remain = change_to_UInt(register_AluResult,32)%4 * 8;

        int len=32;
        if(write_Style == 2) len = 16;
        else if(write_Style == 3) len = 8;
	
	// memory address overflow
	if(addr>255 || addr<0){
		error_buffer[0] = error_buffer[3] = true;
		error_Occur = 1;
	}
	else if(addr==255 && remain+len>32){
		error_buffer[0] = error_buffer[3] = true;
		error_Occur = 1;
	}

        //detect memory mis alignment and overflow
        if(remain % len != 0){
                error_buffer[0] = error_buffer[4] = true;
                error_Occur = 1;
        }
	
	// late return can detect two errors
	if(error_Occur) return ;


	for(i=0;i<len;i++){
		if(i+remain<32) d_memory[addr][i+remain] = read_Reg2[32-len+i];
		else  d_memory[addr+1][i+remain-32] = read_Reg2[32-len+i];
	}
}


void alu_Control_Identify(int* alu_Control,int ALUOP,int* funct, int* opArray, int* rs){
	int f = change_to_UInt(funct,6);
	int c; // aluControl for short
	// lw sw
	/*if(ALUOP==0){
		c = _OP_ADD;
	}
	//branches
	else if(ALUOP==1){
		c = _OP_SUB;
	}
	// R-Type
	*/switch(ALUOP){
		case 0: c = _OP_ADD;
						break;
		case 1: c = _OP_SUB;
				break;
		case 2:{
			switch(f){
				case _ADD : c = _OP_ADD;
							break;
				case _ADDU:c = _OP_ADDU;
						break;
				case _SUB : c = _OP_SUB;
						break;
				case _AND : c = _OP_AND;
						break;
				case _OR:c = _OP_OR;
						break;
				case _XOR : c = _OP_XOR;
						break;
				case _NOR : c = _OP_NOR;
						break;
				case _NAND : c = _OP_NAND;
						break;
				case _SLT : c = _OP_SLT;
						break;
				case _SLL : c = _OP_SLL;
						break;
				case _SRL : c = _OP_SRL;
						break;
				case _SRA : c = _OP_SRA;
						break;
			}
				break;
		}
		
	case  3 :{ int opcode = change_to_UInt(opArray,6);
		 switch(opcode){
		 	case  _ADDI : c = _OP_ADD;
		 			break;
		 	case _ADDIU : c = _OP_ADDU;
		 			break;
		 	case _LUI : c = _OP_LUI;
		 			break;
		 	case _ANDI : c = _OP_AND;
		 			break;
		 	case _ORI : c = _OP_OR;
		 			break;
		 	case _NORI : c = _OP_NOR;
		 			break;
		 	case _SLTI : c = _OP_SLT;
		 			break;
		 }
	    break;
	}
	//default : printf("exception ALUOP = %d",ALUOP);

	}
	/*else if(ALUOP==2){
		if(f==_ADD) c = _OP_ADD;
		else if(f==_ADDU) c = _OP_ADDU;
		else if(f==_SUB) c = _OP_SUB;
		else if(f==_AND) c = _OP_AND;
		else if(f==_OR) c = _OP_OR;
		else if(f==_XOR) c = _OP_XOR;
		else if(f==_NOR) c = _OP_NOR;
		else if(f==_NAND) c = _OP_NAND;
		else if(f==_SLT) c = _OP_SLT;
		else if(f==_SLL) c = _OP_SLL;
		else if(f==_SRL) c = _OP_SRL;
		else if(f==_SRA) c = _OP_SRA;
	
	}
	// imme operation
	else if(ALUOP==3){
		int op = change_to_UInt(opArray,6);
		if(op==_ADDI) c = _OP_ADD;
		else if(op==_ADDIU) c = _OP_ADDU;
		else if(op==_LUI)  c = _OP_LUI;
		else if(op==_ANDI) c = _OP_AND;
		else if(op==_ORI) c = _OP_OR;
		else if(op==_NORI) c = _OP_NOR;
		else if(op==_SLTI) c = _OP_SLT;

	}
	else printf("exception ALUOP = %d",ALUOP);*/

	*alu_Control = c;
}

void register_Alu(int* result,int* input1, int* input2 ,int alu_Control,int* nextPC, int* shamt, bool* error_buffer,int is_Branching){

	int c = alu_Control;
	bool handling[6];
	//int sh = toUInt(shamt,5);
	switch(c){
		case _OP_ADD: pc_ADD(result,input1,input2,error_buffer);
				break;
		case _OP_SUB: SUB_funt(result,input1,input2,is_Branching?handling:error_buffer);
				break;
		case _OP_AND: AND_funt(result,input1,input2);
				break;
		case _OP_OR: OR_funt(result,input1,input2);
				break;
		case _OP_XOR: XOR_funt(result,input1,input2);
				break;
		case _OP_NOR: NOR_funt(result,input1,input2);
						break;
		case _OP_NAND: NAND_funt(result,input1,input2);
						break;
		case _OP_SLT: SLT_funt(result,input1,input2);
				break;
		case _OP_SLL: SLL_funt(result,input2,shamt);
							break;
		case _OP_SRL: SRL_funt(result,input2,shamt);
						break;
		case _OP_SRA: SRA_funt(result,input2,shamt);
						break;
		case _OP_ADDU: ADDU_funt(result,input1,input2);
					break;
		case _OP_LUI:LUI_funt(result,input2);
				break;
	}
	/*if(c==_OP_ADD) pc_ADD(result,input1,input2,error_buffer);
	else if(c==_OP_SUB) SUB_funt(result,input1,input2,is_Branching?handling:error_buffer);	
	else if(c==_OP_AND) AND_funt(result,input1,input2);
	else if(c==_OP_OR) OR_funt(result,input1,input2);

	else if(c==_OP_XOR) XOR_funt(result,input1,input2);
	else if(c==_OP_NOR) NOR_funt(result,input1,input2);
	else if(c==_OP_NAND) NAND_funt(result,input1,input2);
	else if(c==_OP_SLT) SLT_funt(result,input1,input2);
	else if(c==_OP_SLL) SLL_funt(result,input2,shamt);
	else if(c==_OP_SRL) SRL_funt(result,input2,shamt);
	else if(c==_OP_SRA) SRA_funt(result,input2,shamt);
	else if(c==_OP_ADDU) ADDU_funt(result,input1,input2);
	else if(c==_OP_LUI) LUI_funt(result,input2);*/

}

int is_Zero(int* a, int len){
	int rect=1,i;
	for(i=0;i<len;i++){
		if(a[i]==1) rect = 0;
	}
	
	return rect;

}

void derive_JumpAddress(int* target , int* PC, int* address){

	int i;
	for(i=0;i<32;i++){
		if(i<4) target[i]=PC[i];
		else if(i<30) target[i] = address[i-4];
		else target[i] = 0;
	}

}


int is_Equal(int* a,int* b,int len){
	int i;
	int eq = 1;
	for(i=0;i<len;i++){
		if(a[i]!=b[i]) eq = 0;
	}
	return eq;
}


int can_Branch( int is_Zero, int* op , int* read_Reg){
	if(op[5]==0){ //beq
		return is_Zero;
	}
	else if(op[4]==0){ // bne
		return (is_Zero+1)%2;
	}
	else{ // bgtz
		int rs_Num = change_to_UInt(read_Reg,32);
		return rs_Num>0;
	}
}

int is_Nop(int* op,int* rt,int* rd,int* sh,int* f){
	if(is_Zero(op,6)&&is_Zero(rt,5)&&is_Zero(rd,5)&&is_Zero(sh,5)&&is_Zero(f,6)) return 1;
	else return 0;
}

int is_Halt(instruc* a){
	int i = change_to_UInt(a->opcode,6);
	if( i==0x3F) return 1;
	else return 0;
}


int is_Using(instruc* me, instruc* target, int double_Detect, instruc* sec){

	if(target->is_RegWrite==0 && target->is_Jal==0) return 0;
/**	
	// if I-type dst is 0 reg
	if( me->dstOfInstruction==0 && me->isSystemCall==0 && me->isBranching==0 && me->isMemWrite==0 && toUInt(me->rt,5)==0 ){
		return 0;
	}
	// R-type dst is 0 reg
	else if(me->dstOfInstruction==1 && toUInt(me->func,6)!=8 && toUInt(me->rd,5)==0 ){
		return 0;
	}
**/
	int flag1 = 1, flag2 = 1;
	int i;

	int *dstination, *sec_Dstination;
	int need1=1, need2=1;

	// no need to deal with zero terms including NOP instruction
	if(is_Zero(me->rs,5)==1) need1 = 0;
	if(is_Zero(me->rt,5)==1) need2 = 0;

	// JAL target is not writen in its own instruction
	if(target->is_Jal==1){
		dstination = RA_ADDR;
	}
	else if(target->dst_Of_Instruction==1 && is_Zero(target->rd,5)==0 ){
		dstination = target->rd;
	}
	else if(target->dst_Of_Instruction==0 && is_Zero(target->rt,5)==0 ){
		dstination = target->rt;
	}
	else{
		return 0;
	}

	if(double_Detect){
		int op_Num = change_to_UInt(sec->opcode,6);
		int fNum = change_to_UInt(sec->funct,6);

		// not process those not writing to registers instructions
		// JR
		if(fNum==8 && op_Num==0){
			goto cont;
		}
		// save to mem
		else if(sec->is_MemWrite){
			goto cont;
		}
		else if(sec->is_Branching){
			goto cont;
		}
		else if(sec->is_Jumping){
			goto cont;
		}
		else if(sec->is_SystemCall){
			goto cont;
		}

		// get destination and judge
		else if(sec->is_Jal==1){
			sec_Dstination = RA_ADDR;
		}
		else if(sec->dst_Of_Instruction==1 && is_Zero(sec->rd,5)==0 ){
			sec_Dstination = sec->rd;
		}
		else if(sec->dst_Of_Instruction==0 && is_Zero(sec->rt,5)==0 ){
			sec_Dstination = sec->rt;
		}
		else goto cont;
		//printf("detectint DB,dst=%d,secDst=%d\n",toUInt(dst,5),toUInt(secDst,5));
		if(is_Equal(dstination,sec_Dstination,5) ){
			//printf("double! fir=%d, sec=%d\n",toUInt(dst,5),toUInt(secDst,5));			
			return 0;
		}
	}

cont:;
	int fNum=change_to_UInt(me->funct,6);
	int op_Num=change_to_UInt(me->opcode,6);

	// these three no rs source
	if(op_Num==0&&(fNum==_SLL||fNum==_SRL||fNum==_SRA)){
		need1=0;
	}
	else if(op_Num==_LUI){
		need1=0;
	}

	int* src2;
	// JR has only one source
	if(op_Num==0&&fNum==_JR){
		need2 = 0;
	}
	// BGTZ has only one source
	else if(op_Num==_BGTZ){
		need2=0;
	}
	else if(me->is_AluSrcImme==0){
		src2 = me->rt;
	}
	// save word has two sources
	else if(me->is_MemWrite>0){
		src2 = me->rt;
	}
	else{
		need2 = 0;
	}
	
	if(need1){
		for(i=0;i<5;i++){
			if(me->rs[i]!=dstination[i] ) flag1 = 0;
		}
	}
	if(need2){
		for(i=0;i<5;i++){
			if(src2[i]!=dstination[i] ) flag2 = 0;
		}
	}

	//printf("need1=%d\nflag1=%d\nneed2=%d\nflag2=%d\n",need1,flag1,need2,flag2);
	//printf("dst=%d\n",toUInt(dst,5));
 	if(flag1&&need1) return 1;
	else if(flag1&&flag2&&need1&&need2) return 3;
	
	else if(flag2&&need2) return 2;

	return 0;

}

void hazard_Controller(){

	instruc* none = malloc(sizeof(instruc)) ;init(none);

	//printf("forward\n");

	//forward
	int a=0,b=0;
	//printf("forward a\n");
	if(DM->is_MemRead==0 && is_Halt(ID)==0 ) a = is_Using(ID,DM,1,EX);
	//printf("forward b\n");
	if(DM->is_MemRead==0 && is_Halt(EX)==0 ) b = is_Using(EX,DM,0,none);
	//printf("forward judge end\n");
	//printf("a=%d,b=%d\n",a,b);


	//forward
	if(a||b){
		// only branch and JR use ID, EX/DM forarding path
		if(a && (ID->is_Branching|| (change_to_UInt(ID->opcode,6)==0&&change_to_UInt(ID->funct,6)==_JR)  ) ){
			// branch need fix
			if(DM->is_Jal==1&&a==1){
				fect_32_Bits(ID->read_Reg1,DM->ra);
				forward1 = 1;
			}
			else if(a==1){
				fect_32_Bits(ID->read_Reg1,DM->register_AluResult);
				forward1 = 1;
			}
			else if(a==2){
				if(ID->is_AluSrcImme==0){
					fect_32_Bits(ID->read_Reg2,DM->register_AluResult);
				}
				forward1 = 2;
			}
			else if(a==3){
				fect_32_Bits(ID->read_Reg1,DM->register_AluResult);
					fect_32_Bits(ID->read_Reg2,DM->register_AluResult);
				forward1 = 3;
			}
		}

		// ALU to fwd?
		if(b){
			 if(b==2){
				if(EX->is_AluSrcImme==0){
					fect_32_Bits(EX->read_Reg2,DM->register_AluResult);
				}
				else if(EX->is_MemWrite>0){
					fect_32_Bits(EX->read_Reg2,DM->register_AluResult);
				}
				forward2 = 2;
			}
			else if(b==1){
				fect_32_Bits(EX->read_Reg1,DM->register_AluResult);
				forward2 = 1;
			}
			
			else if(b==3){
				fect_32_Bits(EX->read_Reg1,DM->register_AluResult);
					fect_32_Bits(EX->read_Reg2,DM->register_AluResult);
				forward2 = 3;
				
			}
		}
	}

	//printf("stall\n");
	int is_Jr = change_to_UInt(ID->opcode,6)==0 && change_to_UInt(ID->funct,6)==_JR;

	// data hazard
	if( 
		// eccept JR, jump instruction never stall
		(ID->is_Jumping==0||is_Jr) && 
		// HALT no need to stall, NOP is dealed in isUsing() function
		is_Halt(ID)==0 &&
		(
			// 1. ID is branch instruction, wait for forwarding
			// 2. JR is close to branch. JR need register target, so wait for forwarding
			// 3. source is using and EX/DM cannot forward because need memory read
			( is_Using(ID,EX,0,none) && (ID->is_Branching||EX->is_MemRead||is_Jr) ) || 
			// 1. other than JR or branch instruction will stall 
			// 2. JR and branch instruction will only stall if needed register is reading from memory
			( is_Using(ID,DM,1,EX) &&((ID->is_Branching==0&&is_Jr==0)||( (ID->is_Branching||is_Jr)&&DM->is_MemRead))) 
		) 
	){
		is_Stall = 1;
	}
	else is_Stall = 0;
	
	//printf("isUsing ID-EX=%d, isUsing ID-DM=%d\n",isUsing(ID,EX),(isUsing(ID,DM)&&DM->isMemRead) );

	free(none);

}
void flush(instruc** IF){
	instruc* trash = *IF;
	*IF = (instruc*)malloc(sizeof(instruc));
	init(*IF);
	free(trash);
}

void debug(void){
	
}
void initialiaze(){
	 FOUR[29]=1;
	// register segments
	
	empty=(instruc*)malloc(sizeof(instruc)); 
	
	
	
	ID=(instruc*)malloc(sizeof(instruc)); 
	EX=(instruc*)malloc(sizeof(instruc)); 
	DM=(instruc*)malloc(sizeof(instruc)); 
	WB=(instruc*)malloc(sizeof(instruc)); 

init(empty);
IF = empty;
init(ID);
init(EX);
init(DM);
init(WB);
}

void free_funct(){
	if(empty!=NULL){ free(empty); empty = NULL;}
	if(IF!=NULL){ free(IF); IF = NULL;}
	if(ID!=NULL){ free(ID); ID = NULL;}
	if(EX!=NULL){ free(EX); EX = NULL;}
	if(DM!=NULL){ free(DM); DM = NULL;}
	if(WB!=NULL){ free(WB); WB = NULL;}
}


#endif

