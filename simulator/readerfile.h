#include <stdio.h>
#include <stdlib.h>
#include "operation.h"

#ifndef READERFILE_H_INCLUDED
#define READERFILE_H_INCLUDED

extern FILE* assembly;

void read_IImage(int buff[][32],int *PC);
void read_DImage(int buff[][32],int *sp);

void register_output( int cycle);
void write_Stage(int* cur,instruc* ID,instruc* EX,instruc* DM,instruc* WB,int is_Stall,int forward1,int forward2,int is_Flush);


/**
	error handlers
**/
	void error_output(bool* error_buffer,int cycle);


/**
    open all needed files
**/
void open_Files(void);

/**
	close all files
**/
void closeFiles(void);

/**
    decoder for unsigned binaries
**/
unsigned int unsigned_Decoder(int* a , int len);

/**
    decoder for signed binaries
**/
int signed_Decoder(int*a , int len);



#endif // READERFILE_H_INCLUDED
