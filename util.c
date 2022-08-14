#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*typedef enum of all the opcodes. not is define as no because not is a reserved word*/

typedef enum {mov, cmp, add, sub, no, clr, lea, inc, dec, jmp, bne, get, prn, jsr, rts, hlt} opcodes;


/*names of all operations*/

const char op_names[][3] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};



/*number of operands each operation gets*/

const int num_of_args[] = {2, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1 , 1, 0, 0};



/*binary code of aperations*/

const char *op_bin_codes[] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};



/*register names*/

const char *reg_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};





/* array of the legal source addressing methods for each operation*/

const int is_valid_dest_op[][4] = {	
	{0,1,1,1},/*mov*/
	{1,1,1,1},/*cmp*/
	{0,1,1,1},/*add*/
	{0,1,1,1},/*sub*/
	{0,1,1,1},/*not*/
	{0,1,1,1},/*clr*/
	{0,1,1,1},/*lea*/
	{0,1,1,1},/*inc*/
	{0,1,1,1},/*dec*/
	{0,1,1,1},/*jmp*/
	{0,1,1,1},/*bne*/
	{0,1,1,1},/*get*/
	{1,1,1,1},/*prn*/
	{0,1,1,1},/*jsr*/
	{0,0,0,0},/*rts*/
	{0,0,0,0}/*hlt*/
};

/* array of the legal destination addressing methods for each operation*/

const int is_valid_source_op[][4] = {	
	{1,1,1,1},/*mov*/
	{1,1,1,1},/*cmp*/
	{1,1,1,1},/*add*/
	{1,1,1,1},/*sub*/
	{0,0,0,0},/*not*/
	{0,0,0,0},/*clr*/
	{0,1,1,0},/*lea*/
	{0,0,0,0},/*inc*/
	{0,0,0,0},/*dec*/
	{0,0,0,0},/*jmp*/
	{0,0,0,0},/*bne*/
	{0,0,0,0},/*get*/
	{0,0,0,0},/*prn*/
	{0,0,0,0},/*jsr*/
	{0,0,0,0},/*rts*/
	{0,0,0,0}/*hlt*/
};


/*
//	return index of operatin name
//	if operation name doesn't exist return -1
*/


DC_memory_slot dc_memory[255];

MEMORY_SLOT ic_memory[255];

/*
//	return index of operatin name
//	if operation name doesn't exist return -1
*/

int find_op(char arg[])
{
	int i=15;
	while(i>=0)
	{
		if(0==(strncmp(arg, op_names[i],3)))
			return i;
		
		
		i--;
	}
	return -1;
}



/*
//	return index of register name
//	if register name doesn't exist return -1
*/


int find_reg(char arg[])
{
	int i=7;
	while(i>=0)
	{
		if(!(strncmp(arg, reg_names[i], 2)))
			return i;
		i--;
	}
	
	return -1;
}


/*method for inner use
//prints int num in binary representation
*/
void print_bits(int num)
{
	int i;
	/*char temp[10];*/
	for(i=9; i>=0; i--)
	{
			printf("%d", ((num>>i)&1));
	}
	
	printf("\n");


}




















