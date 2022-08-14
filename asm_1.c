#include "asm_1.h"
#include "util.h"
#include "encode.h"
#include "command_line.h"
#include "label_list.h"

extern MEMORY_SLOT ic_memory[255];
extern DC_memory_slot dc_memory[255];

extern label_node_ptr label_list_head;
extern label_node_ptr label_list_tail;
extern label_node_ptr ext_list_head;
extern label_node_ptr ext_list_tail;


/*flag that changes to true if an error occured during the run*/
int error_flag=0;

/*keeps the number of line*/
int line_count;
	
	
/*translates the source file to machine code
// translates everything except for the label addresses and entry labels
*/
void translate(FILE *fp)
{
	char **line;
	char buffer[MAX_LINE_SIZE];
	char label[MAX_LABEL_SIZE];
	int i, label_flag, op_code;
	
	IC=0;
	DC=0;
	
	line_count=0;
		
	if(!(line = (char**) malloc(sizeof(char*)*MAX_WORDS)))	
	{
		printf("memory allocatin failed!\n");
		exit(0);
	}
	
	/*read a line*/
	while(fgets(buffer ,MAX_LINE_SIZE, fp)!=NULL)
	{
		
		line_count++;

		/*reset flags*/
		i = 0;
		

		split_line(buffer, line);	
		

		
		/*NOT AN EMPTY LINE OR COMMENT LINE*/
		if((line[0]!=NULL) && (line[0][0]!=';'))
		{
			
			/*check if defining a label*/
			if((label_flag = is_label(line)))
			{
				copy_label_name(label, line[i]);
				i++;
			}

			/*check if data stroage command*/
			if(equals(line[i], ".data") || equals(line[i], ".string") || equals(line[i], ".struct"))
			{
			
				if(line[i+1] == NULL) /*if no arguments after the data type name*/
				{
					printf("line %d: ", line_count);
					printf("error! trying to define %s with no arguments\n", line[i]);
					error_flag =1;
				}
				
				else
				{	
					if(label_flag)
						append_label(label, data); /*if label than add it to list*/
					
					
					if(equals(line[i], ".data"))
						store_data(i, line);
						
						
					else if(equals(line[i], ".string"))
						store_string(i+1, line);
					
					else /* .struct */
						store_struct(i+1, line);
					
				}
				
			}
			

			
			else if(equals(line[i], ".extern"))
			{
				append_extern_label(line, i+1);
			}	
			
					
					/*instruction*/
			else
			{
				if(label_flag)
				{
					append_label(label, code);

				}

				op_code = find_op(line[i]);
				
				if(-1 != op_code)
				{
					i++;
					if(!(encode_op(line, i, op_code)))
						error_flag=1;
					
				}
						
				else if(!(equals(line[i], ".entry")))
				{
					printf("line %d: ", line_count);
					printf("error! invalid operation name!\n");
					error_flag =1; 
				}
			}
		}
	
	
		
				
		
		/*end of line*/
		
		
		

	}

	free_line(line);
	free(line);
	
	/*if there are errors don't go on to 2nd translation*/
	if(error_flag)
	{
		printf("errors occurd during translation\n stopping assembler\n");
		exit(0);
	}  /*there were errors then need to stop!"*/
	
	offset_data_labels(IC);
	
	append_DC_to_IC();
	

}







/*appends a data data type to dc memory*/

void store_data(int i, char *line[])
{
	
	
	
	
	while(line[i] != NULL)
	{
		i++;
		
		if(!(is_num(line[i])))
		{
			printf("line %d: ", line_count);
			printf("error! trying to define .data with a non decimal argument!\n");
			error_flag=1;
			return;	
		}
		
		
	
		dc_memory[DC].num = atof(line[i]);
		
		
		
		DC++;
		i++;
		
		if(line[i] != NULL && (!(equals(line[i], ","))))
		{
			printf("line %d: ", line_count);
			printf("error! missing comma after argument!\n");
			error_flag=1;
			return;
		}
		
		
		
	}
	



}

/*appends a string data type to dc memory*/

void store_string(int i, char *line[])
{
	
	int j=0;
	
	if(line[i+1]!=NULL)
	{
		printf("line% d: ", line_count);
		printf("error! too many arguments while defining .string!\n");
		error_flag = 1;
		return;
	}
	
	if(line[i][j]!='"')
	{
		printf("line %d: ", line_count);
		printf("error! missing quatation mark!!\n");
		error_flag = 1;
		return;
	}
	
	for(j=1; (line[i][j]!='\0') && (isalnum(line[i][j])); j++)
	{
	
		
		dc_memory[DC].num = line[i][j];
		
		DC++;
	
	}
	
	if(line[i][j]!='"')
	{
		printf("line %d: ", line_count);
		printf("error! missing quatation mark!!\n");
		error_flag = 1;
		return;
	}
	
	DC++;

}




/*appends a struct data type to dc memory*/

void store_struct(int i, char *line[])
{
	
	int j=0;
	
	
	if(line[i] == NULL || !(is_num(line[i])))
	{
		printf("line %d: ", line_count);
		printf("error! trying to define .struct with a non decimal argument!\n");
		error_flag=1;
		return;	
	}
	
	dc_memory[DC].num = atof(line[i]);
	DC++;
	i++;
	
	if(line[i] == NULL || (!(equals(line[i], ","))))
	{
		printf("line %d: ", line_count);
		printf("error! missing comma after argument!\n");
		error_flag=1;
		return;
	}
	
	i++;
	
	if(line[i] == NULL || line[i][j]!='"')
	{
		printf("line %d: ", line_count);
		printf("error! missing quatation mark!!\n");
		error_flag = 1;
		return;
	}
	
	for(j=1; (line[i][j]!='\0') && (isalnum(line[i][j])); j++)
	{
	
		
		dc_memory[DC].num = line[i][j];
		
		DC++;
	
	}
	
	if(line[i][j]!='"')
	{
		printf("line %d: ", line_count);
		printf("error! missing quatation mark!!\n");
		error_flag = 1;
		return;
	}

	DC++;


}


/*checks if a string is a numeric */

int is_num(char str[])
{
	int i=0;
	
	if(str[i]=='-' || str[i]=='+')
			i++;
	
	for(; str[i]!='\0'; i++)
	{
		if(!(isdigit(str[i])))
			return 0;
	
	}
	return 1;

}







/*appends the dc memory to the ic memory*/

void append_DC_to_IC()
{
	int i;
	for(i=0; i<DC; i++)
	{
		ic_memory[IC].dc_memory_slot.num = dc_memory[i].num;
		IC++;
	}

}



/*prints the ic memory as bits*/

void print_output()
{
	int i;
	
	for(i=0; i<IC; i++)
	{
		print_bits(ic_memory[i].dc_memory_slot.num);
	}


}













