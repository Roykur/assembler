#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_LABEL_SIZE 31

#define MAX_WORDS 40

#define START_OF_MEMORY 100

#define equals(str1, str2) (strcmp(str1, str2) == 0)



void translate(FILE *fp);



void store_struct(int i, char *line[]);

void store_string(int i, char *line[]);

void store_data(int i, char *line[]);

int is_num(char str[]);


/* unfinished functions*/




void append_DC_to_IC();
	
void print_output();

