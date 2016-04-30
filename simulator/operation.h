#include <stdbool.h>

#ifndef OPERATION_H_INCLUDED
#define OPERATION_H_INCLUDED

void XOR_funt(int* result,int* input1,int* input2);
void NOR_funt(int* result,int* input1,int* input2);
void NAND_funt(int* result,int* input1,int* input2);
void SLT_funt(int* result,int* input1,int* input2);
void SLL_funt(int* result,int* sh,int* input2);
void SRL_funt(int* result,int* sh,int* input2);
void SRA_funt(int* result,int* sh,int* input2);

void AND_funt(int* result,int* input1,int* input2);
void OR_funt(int* result,int* input1,int* input2);

void JR_funt(int* PC,int* input1);
void LUI_funt(int* result, int* input2);
void NOT_funt(int* input);
void pc_ADD(int* result,int* input1,int* input2, bool* error_buffer);
void ADDU_funt(int* result,int* input1,int* input2);
void SUB_funt(int* result,int* input1,int* input2, bool* error_buffer);

typedef struct instruc{
	int inst[32];
	int opcode[6];
	int rs[5];
	int rt[5];
	int rd[5];
	int shamt[5];
	int imme[16];
	int exImme[32];
	int funct[6];
	int L[26];

	int read_Reg1[32];
	int read_Reg2[32];
	int read_Mem[32];
	int register_AluResult[32];
	int alu_Control;

        int dst_Of_Instruction;
	int is_Jumping;
        int is_Branching;
        int is_MemRead;
        int is_MemToReg;
        int ALUOP;
        int is_MemWrite;
        int is_AluSrcImme;
        int is_RegWrite;
	int is_SystemCall;

	int ra[32];
	int is_Jal;
}instruc;

// R-Typ
#define		_ADD	0x20	//	10 0000
#define		_ADDU	0x21	//	10 0001
#define		_SUB	0x22	//	10 0010
#define		_AND	0x24	//	10 0100
#define		_OR	0x25	//	10 0101
#define		_XOR	0x26	//	10 0110
#define		_NOR	0x27	//	10 0111
#define		_NAND	0x28	//	10 1000
#define		_SLT	0x2A	//	10 1010 
#define		_SLL	0x00	//	00 0000
#define		_SRL	0x02	//	00 0010
#define		_SRA	0x03	//	00 0011
#define		_JR	0x08	//	00 1000

// I-Type
#define		_ADDI	0x08	//	00 1000
#define		_ADDIU	0x09	//	00 1001
#define		_LW	0x23	//	10 0011
#define		_LH	0x21	//	10 0001 
#define		_LHU	0x25	//	10 0101
#define		_LB	0x20	//	10 0000
#define		_LBU	0x24	//	10 0100
#define		_SW	0x2B	//	10 1011
#define		_SH	0x29	//	10 1001
#define		_SB	0x28	//	10 1000
#define		_LUI	0x0F	//	00 1111
#define		_ANDI	0x0C	//	00 1100
#define		_ORI	0x0D	//	00 1101
#define		_NORI	0x0E	//	00 1110
#define		_SLTI	0x0A	//	00 1010
#define		_BEQ	0x04	//	00 0100
#define		_BNE	0x05	//	00 0101
#define		_BGTZ	0x07	//	00 0111

// J-Type
#define		_J	0x02	//	00 0010
#define		_JAL	0x03	//	00 0011

// S-Type
#define		_HALT	0x3F	//	11 1111


// ALU operation code
#define		_OP_ADD		0
#define		_OP_SUB		1
#define		_OP_AND		2
#define		_OP_OR		3
#define		_OP_XOR		4
#define		_OP_NOR		5
#define		_OP_NAND	6
#define		_OP_SLT		7
#define		_OP_SLL		8
#define		_OP_SRL		9
#define		_OP_SRA		10
#define		_OP_JR		11
#define		_OP_ADDU	12
#define		_OP_LUI		13



#endif
