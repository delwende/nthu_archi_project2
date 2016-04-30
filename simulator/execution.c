#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "simulator.h"
#include "readerfile.h" // get decoders
#include "execution.h"
#include "operation.h"
#include "declaration.h"
#include "function.h"




void execute(){

	
	bool exit=false;
	
	initialiaze();
	// record sp to registers
	fect_32_Bits(reg[29],sp);
	
int PcNum ;
	int cycle = -1;
	while(1){

		cycle ++;
		
		
		Memory_instruction(cur);
	
		forward1=0; forward2=0; is_Flush=0;	

		// hazard control
		hazard_Controller();

		register_output(cycle);

		// block idiots without halt
		 PcNum = PCtoUInt(PC);
		if( PcNum>1024 || PcNum<0){
			printf("\n\n		PC out of bound \n\n\n");
			return;
		}
		else if(cycle>500000){
			printf("\n\n		Over 500000 cycles! Program terminated!\n");
			return;
		}
		
		// PC adder
		pc_ADD(nextPC,PC,FOUR,error_buffer);

		//printf("\nprocess cycle %d, PC=%d\n",cycle,PcNum);

		if(stall_Rec==0){
			IF = (instruc*)malloc(sizeof(instruc));
			init(IF);
		}

		//WB

		//write register

		//						write destination		write data
		if(WB->is_RegWrite){
			 if(!is_Nop(WB->opcode,WB->rt,WB->rd,WB->shamt,WB->funct)) 
				write_To_Register(reg, ( WB->dst_Of_Instruction? WB->rd:WB->rt ) , (WB->is_MemToReg)? WB->read_Mem:WB->register_AluResult ,error_buffer);
		}
		else if(WB->is_Jal){
			fect_32_Bits(reg[31],WB->ra);
		}


		//DM

		// data mem
		//printf("	Start working on read/write data memory...\n");
		if(DM->is_MemRead && DM->is_MemToReg) read_From_Mem(d_memory,DM->register_AluResult,DM->read_Mem,DM->is_MemRead,DM->opcode,error_buffer);
		else if(DM->is_MemWrite) write_To_Mem(d_memory,DM->register_AluResult,DM->read_Reg2,DM->is_MemWrite,error_buffer);


		//EX
		

		// register ALU
		register_Alu(EX->register_AluResult,EX->read_Reg1, (EX->is_AluSrcImme==1? EX->exImme:EX->read_Reg2 ) , EX->alu_Control, nextPC, EX->shamt,error_buffer,EX->is_Branching);

//ID
	
		if(is_Stall == 0){	
			// extend imme
			extend_Imme(ID->imme,ID->exImme,ID->opcode);

			// read register
			alu_Control_Identify( &(ID->alu_Control),ID->ALUOP,ID->funct,ID->opcode,ID->rs);
			read_From_Register(reg,ID->rs,ID->rt, ID->read_Reg1 ,ID->read_Reg2, forward1);
			
	
			// branch
			if(ID->is_Branching &&  can_Branch( is_Equal(ID->read_Reg1,ID->read_Reg2,32) , ID->opcode , ID->read_Reg1 )   ){
				//printf("		Branch in this cycle!\n");
				SLL_funt( l2ExImme,ID->exImme,TWO);
				pc_ADD( branch_AluResult , PC , l2ExImme ,error_buffer);
				fect_32_Bits(nextPC,branch_AluResult);
				is_Flush = 1;
				flush(&IF);
			}
			// jump
			else if(ID->is_Jumping){
				//printf("		Jump in this cycle!\n");
				if(change_to_UInt(ID->funct,6)==_JR && change_to_UInt(ID->opcode,6)==0){
					fect_32_Bits(nextPC,ID->read_Reg1);
					is_Flush = 1;
					flush(&IF);
				}
				else{
					if(ID->is_Jal){
						fect_32_Bits(ID->ra,PC);
						fect_32_Bits(ID->register_AluResult,PC);
					}
					derive_JumpAddress(jumpAddress,nextPC,ID->L);
					fect_32_Bits(nextPC,jumpAddress);
					is_Flush = 1;
					flush(&IF);
				}
			}
   		
		}
		//printf("isBranching =%d, canBranch=%d\n",ID->isBranching,canBranch(isEqual(ID->readReg1,ID->readReg2,32),ID->op,ID->readReg1) );
   
		//IF


		if(is_Stall == 0 && is_Flush==0){

			// read instruction from instruction memory
			fect_32_Bits(IF->inst,cur);
			
			// divide each colume from current instruction
        		function_derive_Columes(cur,IF->opcode,IF->rs,IF->rt,IF->rd,IF->shamt,IF->funct,IF->imme,IF->L);

			// derive all control regs
			function_controller(IF->opcode, IF->funct, &(IF->dst_Of_Instruction), &(IF->is_Jumping), &(IF->is_Branching), &(IF->is_MemRead), &(IF->is_MemToReg), &(IF->ALUOP), &(IF->is_MemWrite), &(IF->is_AluSrcImme), &(IF->is_RegWrite), &(IF->is_SystemCall) );
			

			int temp = change_to_UInt(IF->opcode,6);
			if(temp==_JAL){
				IF->is_Jal=1;
			}

		}

		
/*
			Static
		*/
		write_Stage(cur,ID,EX,DM,WB,is_Stall,forward1,forward2,is_Flush);


			
		if(error_buffer[0]){
			if(error_buffer[1]){
				error_output(error_buffer, cycle+1);
				
				error_buffer[0] = error_buffer[1] = false;
				

			}
			if(error_buffer[3]){
				error_output(error_buffer, cycle+1);
				
				exit = true;
				error_buffer[0] = error_buffer[3] = false;
				

			}
			if(error_buffer[4]){	
			error_output(error_buffer, cycle+1);				
				exit = true;
				error_buffer[0] = error_buffer[4] = false;
			}
			if(error_buffer[2]){
				error_output(error_buffer, cycle+1);
				
				error_buffer[0] = error_buffer[2] = false;
				

			}

		}

		// derive system calls
		if(WB->is_SystemCall && DM->is_SystemCall && EX->is_SystemCall && ID->is_SystemCall && IF->is_SystemCall){
			
			exit = true;
		}

		if(exit){
			break;//halt or error handling occur
		}
	
		//move Temps
		if(is_Stall==1){
			instruc* trash;
			trash = WB;
			WB = DM;
			DM = EX;
			EX = (instruc*)malloc(sizeof(instruc));
			init(EX);
			free(trash); 
			
		}
		else{
			
			instruc* trash;
			trash = WB;
			WB = DM;
			DM = EX;
			EX = ID;
			ID = IF;
			free(trash); 
		}

		
		if(is_Stall == 0) fect_32_Bits(PC,nextPC);
		stall_Rec = is_Stall;

	}

	free_funct();
}

