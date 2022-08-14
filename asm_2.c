#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<math.h>

#include "asm_2.h"
#include "util.h"
#include "encode.h"
#include "command_line.h"
#include "asm_1.h"
#include "label_list.h"

const char base32[32] = {'!' ,'@' ,'#' ,'$' ,'%' ,'^' ,'&' ,'*' ,'<' ,'>' ,'a' ,'b' ,'c' ,'d' ,'e' ,'f' ,'g' ,'h' ,'i' ,'j' ,'k' ,'l' ,'m' ,'n'
		,'o' ,'p' ,'q' ,'r' ,'s' ,'t' ,'u' ,'v'};


extern MEMORY_SLOT ic_memory[255];
extern DC_memory_slot dc_memory[255];


extern label_node_ptr label_list_head;
extern label_node_ptr label_list_tail;

extern label_node_ptr ext_list_head;
extern label_node_ptr ext_list_tail;

extern int num_of_args[];
extern int IC;

int error_flag, line_count; /*might need to call it from other function as extern*/


/*goes through the file line by line and encodes it if it was not encoded in 1st translation
//
*/

int translate_2(FILE *fp)
{
	char *line[MAX_WORDS];
	char buffer[MAX_LINE_SIZE];
	int i;
	
	IC = 0;
	
	error_flag = 0;
	line_count=0;
	
	/*read a line*/
	while(fgets(buffer ,MAX_LINE_SIZE, fp)!=NULL)
	{
		line_count++;
		
		i=0;
		split_line(buffer, line);	
		
		

		
		/*NOT AN EMPTY LINE OR COMMENT LINE*/
		if((line[0]!=NULL) && (line[0][0]!=';') )
		{

			/*check if first word is a definition of a label and if so then skip*/
			if(is_label(line))
				i++;

			if(!(equals(line[i], ".data") || equals(line[i], ".string") || equals(line[i], ".struct") || equals(line[i], ".extern")))
			{
			
				if(equals(line[i], ".entry"))
				{
					append_entry_label(line, i+1);
				}
				
				else
				{
					if(!(final_encode(line, i+1)))
					{
					printf("line %d: ", line_count);
					printf("error! undefined label\n"); 
					error_flag=1;
					}
				}
			
			}
			
			
		}
		
		

			
	}
	
	free_line(line);
	
	IC=IC+DC;
	

	
	/*if there are errors don't go on to 2nd translation*/
	if(error_flag)
	{
		printf("\nerrors were found while translating!\n stopping assembler\n");
		return 0;
	}  /*there were errors then need to stop!"*/
		
	
	
	return 1;
}



/*
// encodes a code line to binary
// return 0 if error occured else return 1
*/

int final_encode(char *line[], int i)
{
	int num_of_operands, extern_op, entry_op, num, curr_op, reg_flag=0, err_flag=0;
	char temp_str[MAX_LABEL_SIZE];
	label_node_ptr temp_label;
	
	num_of_operands = num_of_args[ic_memory[IC].ic_memory_slot.op];
	extern_op = ic_memory[IC].ic_memory_slot.extern_op;
	entry_op = ic_memory[IC].ic_memory_slot.entry_op;
	
	
	/*skip the command line*/
	IC++;
	
	for( ; num_of_operands>0; num_of_operands--)
	{
		
		if(num_of_operands == 2)
			curr_op = entry_op;
		else
			curr_op = extern_op;
		
		switch(curr_op)
		{
			case(0):
			
				IC++;
			break;
			case(1):
			
				
				temp_label = search_label_name(line[i]);
				
				if(temp_label == NULL)
					err_flag=1;
					
				else if(temp_label->label_src_type == external)
				{
					ic_memory[IC].num_memory_slot.ARE = E;
					
					/*add current address to external list*/
					append_ext_address(temp_label);
				}

				else
				{
					ic_memory[IC].num_memory_slot.num = temp_label->address+START_OF_MEMORY;
					ic_memory[IC].num_memory_slot.ARE = R;

				}
				IC++;
				
			break;	
				
			case(2):
				
				
				num = strcspn(line[i], ".");
				strncpy(temp_str, line[i], num);
				temp_str[num] = '\0';

				if((temp_label = search_label_name(temp_str)) == NULL)
					err_flag=1;
				
				else if(temp_label->label_src_type == external)
				{
					ic_memory[IC].num_memory_slot.ARE = E;
					
					/*add current address to external list*/
					append_ext_address(temp_label);
				}	
				else
				{
					ic_memory[IC].num_memory_slot.num = temp_label->address+START_OF_MEMORY;
					ic_memory[IC].num_memory_slot.ARE = R;
				}
				IC = IC+2;
					
			break;	
					
			case(3):
			
				/*first reg operand in this line*/
				if(!reg_flag)
				{
					IC++;
					reg_flag=1;
				}
			break;
		}
		
		/*if the are 2 operands then need to skip the comma; if not then it doesn't matter*/
		i = i+2;
			
	}
	
	if(err_flag==1)
		return 0;
		
	return 1;
}









/*translates an int from decimal to base32
// get an int as the number to translates and translates it into an str that is passed as a variable
*/
char* decimal_to_32(unsigned int num)
{
	int i;
	char *temp;
	
	if(NULL == (temp = (char*) malloc(sizeof(char)*MAX_32BASE_NUM_SIZE)))
	{
		printf("memory allocation failed!");
		exit(0);
	}
	
	for(i=0; i<MAX_32BASE_NUM_SIZE; i++)
	{
		temp[MAX_32BASE_NUM_SIZE-i-1] = base32[num%32];
		num = num/32;
	}
	
	
	return temp;
	
}




/*writes the .ob output file*/
void write_output_ob(char file_name[])
{

	int i;
	FILE *new_fp;
	char temp_str[MAX_32BASE_NUM_SIZE];

	new_fp = fopen(file_name ,"w");

	if (new_fp == NULL)
	{
		printf("ERROR! could not create file '%s.ob'\n", file_name);
		exit(0);
	}
	
	/*write IC AND DC to first line*/
	
	/*write ic*/
	strncpy(temp_str, decimal_to_32(IC-DC), MAX_32BASE_NUM_SIZE);/*AT THIS POINT IC HOLDS THE IMAGE+INSTRUCTIONS size*/
	fprintf(new_fp, "%s\t", temp_str);
	
	/*write dc*/
	strncpy(temp_str, decimal_to_32(DC), MAX_32BASE_NUM_SIZE);/*AT THIS POINT IC HOLDS THE IMAGE+INSTRUCTIONS size*/
	fprintf(new_fp, "%s\n", temp_str);
	
	for(i=0; i<IC; i++)
	{	
		strncpy(temp_str, decimal_to_32(i+START_OF_MEMORY), MAX_32BASE_NUM_SIZE);
		temp_str[MAX_32BASE_NUM_SIZE] = '\0';
		fprintf(new_fp, "%s\t", temp_str);
		
	
		strncpy(temp_str, decimal_to_32(ic_memory[i].dc_memory_slot.num), MAX_32BASE_NUM_SIZE);
		temp_str[MAX_32BASE_NUM_SIZE] = '\0';
	
		fprintf(new_fp, "%s\n", temp_str);
	
	
	}

	

	fclose(new_fp);
	
	free_label_list();
}


/*write the .ent output file
//if there are no entry labels - does'nt write
*/
void write_output_ent(char file_name[])
{
	int flag=1;
	FILE *new_fp;
	char temp_str[MAX_32BASE_NUM_SIZE];
	label_node_ptr temp = label_list_head;
	
	
	/*search labels*/
	while(temp!=NULL)
	{
		/*if label is entry*/
		if(temp->label_src_type==entry)
		{
		
			if(flag) /*create file*/
			{
				new_fp = fopen(file_name ,"w");

				if (new_fp == NULL)
				{
					printf("ERROR! could not create file '%s.ent'\n", file_name);
					exit(0);
				}
				flag=0;
				
			}
			
			/*translate data and add to file*/
			strncpy(temp_str, decimal_to_32(temp->address+START_OF_MEMORY), MAX_32BASE_NUM_SIZE);
			temp_str[MAX_32BASE_NUM_SIZE] = '\0';
		
			fprintf(new_fp, "%s\t", temp->name);
			fprintf(new_fp, "%s\n", temp_str);
		}
		
		temp = temp->next;		
	}
	
	/*close file*/
	if(!flag)
		fclose(new_fp);
	
}



/*write the .ext output file
//if there are no extern labels - does'nt write
*/
void write_output_ext(char file_name[])
{
	FILE *new_fp;
	char temp_str[MAX_32BASE_NUM_SIZE];
	label_node_ptr temp = ext_list_head;
	
	/*ext list is empty - don't create .ext file*/
	if(temp==NULL)
		return;
	
	/*create .ext file*/
	new_fp = fopen(file_name ,"w");
	if (new_fp == NULL)
	{
		printf("ERROR! could not create file '%s.ent'\n", file_name);
		exit(0);
	}
		
	/*search labels*/
	while(temp!=NULL)
	{	
		/*translate data and add to file*/
		strncpy(temp_str, decimal_to_32(temp->address), MAX_32BASE_NUM_SIZE);
		temp_str[MAX_32BASE_NUM_SIZE] = '\0';
		
		fprintf(new_fp, "%s\t", temp->name);
		fprintf(new_fp, "%s\n", temp_str);
		
		
		temp = temp->next;	
	}
	
	free_ext_list();
	
	/*close file*/
	fclose(new_fp);
}















