#ifndef ASM2_H_

#define ASM2_H_


#define MAX_32BASE_NUM_SIZE 2



int translate_2(FILE *fp);

int final_encode(char *line[], int i);

char* decimal_to_32(unsigned int num);

void write_output_files(char file_name[]);

void write_output_files(char file_name[]);

void write_output_ob(char file_name[]);

void write_output_ext(char file_name[]);

void write_output_ent(char file_name[]);


#endif /*ASM2_H_*/
