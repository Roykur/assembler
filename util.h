#ifndef UTIL_H_

#define UTIL_H_



typedef enum {A, E, R} ARE;


typedef struct {
	signed int num:10;

}DC_memory_slot;



typedef struct {

	unsigned int ARE:2;
	unsigned int extern_op:2;
	unsigned int entry_op:2;
	unsigned int op:4;

}IC_memory_slot;

typedef struct{

	unsigned int ARE:2;
	signed int num:8;

}NUM_memory_slot;

typedef struct{

	unsigned int ARE:2;
	unsigned int dest_reg:4;
	unsigned int src_reg:4;

}REG_memory_slot;


typedef union{

	NUM_memory_slot num_memory_slot;
	REG_memory_slot reg_memory_slot;
	IC_memory_slot ic_memory_slot;
	DC_memory_slot	dc_memory_slot;


}MEMORY_SLOT;



int IC, DC;



int find_op(char arg[]);

int find_reg(char arg[]);

void print_bits(int num);

#endif /* UTIL_H_ */











