
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "asm_1.h"
#include "encode.h"
#include "util.h"

extern MEMORY_SLOT ic_memory[255];
extern DC_memory_slot dc_memory[255];

extern int is_valid_source_op[][4];
extern int is_valid_dest_op[][4];
extern int num_of_args[];
extern int line_count;


/*
/ returns true if a string is reserved word in the assembler
*/
int is_reserved_word(char *str)
{
	if(find_op(str)!=-1 || find_reg(str)!=-1)
		return 1;
	return 0;
}




/*
// encodes the an instruction line to binary code
// return 0 if there was an error
// return 1 if encode was succussful
*/

int encode_op(char *line[], int i, int op_code)
{

	int args, op1_type, op2_type;
	
	
	args = count_args(line, i);
	
	

	
	if(args == -1)
	{
		printf("line %d: ", line_count);
		printf("error! missing comma between arguments\n");
		return 0;
	}
	
	
	else if(args>num_of_args[op_code])
	{
		printf("line %d: ", line_count);
		printf("error! too many arguments\n");
		return 0;
	}
	
	
	
	else if(args==num_of_args[op_code])
	{
		
		if(num_of_args[op_code] == 0)
		{
		
			ic_memory[IC].ic_memory_slot.op = op_code;
			IC++;
			
		}
		else if(num_of_args[op_code] == 1)
		{	

			op1_type = find_operand_type(line[i]);
			
			if(op1_type == -1)
				return 0;
				
			if( is_valid_dest_op[op_code][op1_type] ) 
			{
				ic_memory[IC].ic_memory_slot.op = op_code;
				ic_memory[IC].ic_memory_slot.extern_op = op1_type;
				IC++;
				if(check_extra_info(line, i, op1_type, -1, num_of_args[op_code]))
					return 0;
				
				
			}
			
			else
			{
				printf("line %d: ", line_count);
				printf("error! invalid destination operand\n");
				return 0;
			}
		}
		
		else if(num_of_args[op_code] == 2)
		{
		
		
			op1_type = find_operand_type(line[i]);
			op2_type = find_operand_type(line[i+2]); /*skip comma cause already checked*/
			
			/*error occured*/
			if(op1_type ==-1 || op2_type == -1)
				return 0;
					
			if( is_valid_source_op[op_code][op1_type] ) 
			{
				
				
				if( is_valid_dest_op[op_code][op2_type] ) 
				{
					ic_memory[IC].ic_memory_slot.op = op_code;
					ic_memory[IC].ic_memory_slot.entry_op = op1_type;
					ic_memory[IC].ic_memory_slot.extern_op = op2_type;
					IC++;
					
					if(check_extra_info(line, i, op1_type, op2_type, num_of_args[op_code]))
						return 0;
					
				}
			
				else
				{
					printf("line %d: ", line_count);
					printf("error! invalid destination operand\n");
					return 0;
				}
				
				
			}
			
			
			else
			{
				printf("line %d: ", line_count);
				printf("error! invalid source operand\n");
				return 0;
			}
			
			
		}
		
		
		
	}	
		
	
	else
	{
		printf("line %d: ", line_count);
		printf("error! too few arguments\n");
		return 0;
	}
	
	return 1;
}


/*checks if instruction needs to store extra information in IC
*/
int check_extra_info(char **line, int i, int op1_type, int op2_type, int args)
{
	int num, struct_field;
	
	
	switch(args)
	{
	case(1):
	
		switch(op1_type)
		{
			case(0): 
				num = get_num(line[i]);
				ic_memory[IC].num_memory_slot.num = num;
			
				IC++;
			
				break;
		
			case(1):	
			
				IC++; /*unkown address*/
			
				break;


			case(2):
		
				IC++; /*unkown address*/
				
				/*the number of the field of the struct*/
				struct_field = (line[i][strlen(line[i])-1]-'0');
				/*check struct field num is valid*/
				if(struct_field>0 && struct_field<3)
					ic_memory[IC].num_memory_slot.num = struct_field; 
					
				else
				{
					printf("line %d: ", line_count);
					printf("error! trying to approach an invalid struct field\n");
					return -1;
				}
			
				IC++;
				
				break;


			case(3):
			
				ic_memory[IC].reg_memory_slot.dest_reg = find_reg(line[i]);

				IC++;
			
			
				break;

		}/*end of case only 1 operand*/
	break;

	
	case(2):
	
		switch(op1_type)
		{
			case(0): 
				num = (int)get_num(line[i]);
				ic_memory[IC].num_memory_slot.num = num;
			
				IC++;
			
				break;
		
			case(1):
			
				IC++; /*unkown address*/
			
				break;


			case(2):
			
				IC++; /*unkown address*/
			
				/*the number of the field of the struct*/
				struct_field = line[i][strlen(line[i])-1]-'0';
				/*check struct field num is valid*/
				if(struct_field>0 && struct_field<3)
					ic_memory[IC].num_memory_slot.num = struct_field; 
				
				else
				{
					printf("line %d: ", line_count);
					printf("error! trying to approach an invalid struct field\n");
					return -1;
				}
				
				IC++;
				
				break;


			case(3):
			
				ic_memory[IC].reg_memory_slot.src_reg = find_reg(line[i]);
				if(op2_type==3)
				{
					ic_memory[IC].reg_memory_slot.dest_reg = find_reg(line[i+2]);
					IC++;
					return 0;
				}
				else
					IC++;
				
			
			
				break;
		}/*end of switch 1st operand*/

		i = i+2; /*skip the comma*/		
		
		/*switch destination operand*/
		switch(op2_type)
		{
		

		
			case(0): 
				num = (int)get_num(line[i]);
				ic_memory[IC].num_memory_slot.num = num;
			
				IC++;
			
				break;
		
			case(1):
			

				IC++; /*unkown address*/
			
				break;


			case(2):
		
				IC++; /*unkown address*/
				
				/*the number of the field of the struct*/
				struct_field = line[i][strlen(line[i])-1]-'0';
				/*check struct field num is valid*/
				if(struct_field>0 && struct_field<3)
					ic_memory[IC].num_memory_slot.num = struct_field; 
				else
				{
					printf("line %d: ", line_count);
					printf("error! trying to approach an invalid struct field\n");
					return -1;
				}
				IC++;
				break;


			case(3):
			
				ic_memory[IC].reg_memory_slot.dest_reg = find_reg(line[i]);
			
				IC++;
			
			
				break;
	
	
	
	
	
		}/*end of switch 2nd operand*/
	
	break;
	
	}
	return 0;
	
}





/*returns the number of the type of the operand or -1 if error occurs*/

int find_operand_type(char *operand)
{
	int i;	
	
	if (operand[0]=='#') /* immidiatie addressing*/
	{
		if(is_num(copy_str(operand, 1))) /*check that a valid number comes after #*/
			return 0;
		else
		{
			printf("line %d: ", line_count);
			printf("error! invalid operand\n");
			return -1;
		}
	}	
	else if(-1 != find_reg(operand))
	{
		return 3;
	}	
	else if(search_dot(operand))
	{
		return 2;
	}
	
	/*check if it is a legit label name*/
	else 
	{
		if(!isalpha(operand[0]))
		{
			/*invalid operand*/
			printf("line %d: ", line_count);
			printf("error! invalid operand\n");
			return -1;
		}
		
		for(i=1; operand[i]!='\0'; i++)
			if(!isalnum(operand[i]))
			{
				/*invalid operand*/
				printf("line %d: ", line_count);
				printf("error! invalid operand\n");
				return -1;
			}
	}
	
	/*op is label*/
	return 1;
	
	
}

/*copies a string starting from index i*/
char* copy_str(char *str, int i)
{
	char *new_str;
	int j=0;
	
	
	new_str = (char*) malloc(sizeof(char)*(strlen(str)-i));
	
	if(NULL == new_str)
	{
		printf("memory allocation failed!\n");
		exit(0);
	}
	
	while(str[i]!='\0')
	{
		
		new_str[j] = str[i];	
		i++;
		j++;
	}
	
	return new_str;

}


/*if str contaions the char '.' return true*/
int search_dot(char* str)
{
	int i;
	for(i=0; str[i]!='\0' ; i++)
	{
		if(str[i]=='.')
			return 1;
	}
	return 0;

}


/*
//	counts number of argument starting from line[i] and checks arguments has a comma in between one another
*/

int count_args(char *line[], int i)
{
	int args=0, comma_flag=0, k=0;

	

	while(line[i]!=NULL)
	{
		k++;
		if(comma_flag)
		{
			if(!(equals(line[i], ",")))		
				return -1;
			comma_flag=0;
		}
		else
		{
			args++;
			comma_flag++;
		}
		
		i++;
	}
	
	return args;
}


/*assuming str is in the following format: #num
// num is a str of an integer that can include + or - before
// return num as an integer
*/

int get_num(char str[])
{
	int i=1, j=0, num;
	char *temp;
	
	temp = (char*) malloc(sizeof(char)*(strlen(str)-1));
	
	if(temp==NULL)
	{
		printf("memory allocatin falied\n");
		exit(0);
	}
	
	if(str[i]=='-' || str[i]=='+'|| isdigit(str[i]))
	{
		while(str[i]!='\0')
		{
			temp[j] = str[i];
			i++;
			j++;
		}
	}
	else
	{
		printf("line %d: ", line_count);
		printf("error! invalid number!\n");
		return 0;
	}
	
	num = atof(temp);
	free(temp);
	return num;

}







