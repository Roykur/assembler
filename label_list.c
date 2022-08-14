#include "asm_1.h"
#include "util.h"
#include "encode.h"
#include "command_line.h"
#include "label_list.h"

/*label list and entry list heads and tails
//used as global variables
*/
label_node_ptr label_list_head = NULL;
label_node_ptr label_list_tail = NULL;
label_node_ptr ext_list_head = NULL;
label_node_ptr ext_list_tail = NULL;

extern int error_flag, line_count;


/*check if line is a label defining line*/
int is_label(char *line[])
{
	int i=0;
	
	/*check it starts with a letter*/
	if(!(isalpha(line[0][i])))
		return 0;
	
	i++;
	
	/*max size of label is 30!*/
	while(isalnum(line[0][i]))
	{
		i++;
		
		/*label excedes maximum label size*/
		if(i>MAX_LABEL_SIZE)
		{
			printf("line %d: ", line_count);
			printf("error! label name too big\n");
			error_flag=1;
			return 0;
		}
	}
		
	if(line[0][i]=='\0')
		return 0; /*not a label*/

	if(line[0][i]==':' && (line[0][i+1]=='\0' || isspace(line[0][i+1])))/*it is a label*/
	{
		/*EMPTY LABEL*/
		if(line[1]==NULL)
		{
			printf("line %d: ", line_count);
			printf("error! empty label\n");
			error_flag=1;
			return 0;
		}

		return 1;
		
	}
	return 0;
}



/*copy str to label without the last char*/
void copy_label_name(char *label, char *str)
{
	int i;
	for(i=0; str[i+1]!='\0'; i++)
		label[i] = str[i];
	label[i] = '\0';
}



/*
/ appends a .data / .string / .struct data label to list.
*/

int append_label(char label[], type label_type)
{

	label_node_ptr temp;
	
	
	/*check that label name is valid*/
	if(search_label_name(label))
	{
		printf("line %d: ", line_count);
		printf("error! label name '%s' already exists!\n", label);
		error_flag=1;
		return 0;
	}
		
	/*is label name a reserved word*/
	if(is_reserved_word(label))
	{
		printf("line %d: ", line_count);
		printf("error! trying to define a label with a reserved word: %s\n", label);
		error_flag=1;
		return 0;
	}
	
	
	temp = (label_node_ptr) malloc(sizeof(label_node));
	
	if (temp == NULL)
	{
		printf("line %d: ", line_count);
		printf("memory allocation failed!");
		exit(0);
	}
	
	if(label_type == data)
	{
		temp->address = DC;
		temp->label_type = data;
		strcpy(temp->name, label);
	}
	
	if(label_type == code)
	{
		temp->address = IC;
		temp->label_type = code;
		strcpy(temp->name, label);
	}
	
	
	
	if(label_list_head == NULL)/*LIST EMPTY*/
	{
		label_list_head = temp;
		label_list_tail = temp;
	}
	
	else
	{
		label_list_tail->next = temp;
		label_list_tail = temp;
		temp->next = NULL;
	}
	
	return 1;

}





/*
/ gets string as argument and searches the labels list to find if a label with such name exists.
/ returns pointer to label node
*/


label_node_ptr search_label_name(char str[])
{
	label_node_ptr temp = label_list_head;


	while((temp!=NULL) && (strlen(str) > 0))
	{
		if((strncmp(temp->name, str, strlen(str))))
			temp = temp->next;
			
		else
			return temp;
		
	}
	
	return NULL;

}



/*appends extern label to the label list*/

void append_extern_label(char *line[], int i)
{
	label_node_ptr temp;
	
	if((line[i] != NULL) && (line[i+1]==NULL)) /*EXTERN GETS ONLY 1 ARGUMENT*/
	{
		if(NULL==(search_label_name(line[i])))
		{
			temp = (label_node_ptr) malloc(sizeof(label_node));
	
			if (temp == NULL)
			{
				printf("line %d: ", line_count);
				printf("memory allocation failed!\n");
				exit(0);
			}
			
			temp->label_src_type = external;
			temp->address = E;
			strcpy(temp->name, line[i]);
			
			if(label_list_head == NULL)/*LIST EMPTY*/
			{
				label_list_head = temp;
				label_list_tail = temp;
			}
	
			else
			{
				label_list_tail->next = temp;
				label_list_tail = temp;
				temp->next = NULL;
			}
			
		}
		
		/*if the label exists in input file*/
		else
		{
			printf("line %d: ", line_count);
			printf("Trying to define .extern label with an existing label!\n");
			error_flag=1;
		}
	}
	
	else
	{
		if(line[i] == NULL)
		{
			printf("line %d: ", line_count);
			printf("Missing arguments when defining .extern !\n");
			error_flag=1;
		}
		else
		{
			printf("line %d: ", line_count);
			printf("Too many arguments when defining .extern !\n");
			error_flag=1;
		}
	}	
}


/* apppends an entry label to the label list*/

void append_entry_label(char *line[], int i)
{
	label_node_ptr temp;
	
	
	if((line[i] != NULL) && (line[i+1]==NULL)) /*ENTRY GETS ONLY 1 ARGUMENT*/
	{
		temp = search_label_name(line[i]);
		
		/*label is not known*/
		if (temp==NULL)
		{
			
			printf("line %d: ", line_count);
			printf("Error! trying to define .entry on a non existing label!\n");
			error_flag=1;
		
		/*label is already defined as .extern*/	
		}
		else if(temp->label_src_type == external)
		{
			printf("line %d: ", line_count);
			printf("Error! Trying to define an .extern label as .entry!\n");
			error_flag=1;
		}
		
		/*label is known*/
		else
		{
			temp->label_src_type = entry;		
		}
		
	}
	
	/*invalid amount of arguments*/
	else
	{
		if(line[i] == NULL)
		{
			printf("line %d: ", line_count);
			printf("Error! Missing arguments when defining .entry !\n");
			error_flag=1;
		}
		else
		{
			printf("line %d: ", line_count);
			printf("Error! Too many arguments when defining .entry !\n");
			error_flag=1;
		}
	}	
}



/*moves the adresses of all data labels by i*/

void offset_data_labels(int i)
{
	label_node_ptr temp = label_list_head;


	while(temp!=NULL)
	{
		if(temp->label_type == data)
			temp->address = (temp->address)+i;
		temp = temp->next;
		
	}

}





/*gets an extern label node and appends it to extern list*/
void append_ext_address(label_node_ptr extern_node)
{
	label_node_ptr temp;
	
	temp = (label_node_ptr) malloc(sizeof(label_node));
	
	if (temp == NULL)
	{
		printf("line %d: ", line_count);
		printf("memory allocation failed!\n");
		exit(0);
	}
	
	strcpy(temp->name, extern_node->name);
	temp->address = IC+START_OF_MEMORY;
	
	/*APPEND THE NODE TO LIST*/
	
	if(ext_list_head == NULL)/*LIST EMPTY*/
	{
		ext_list_head = temp;
		ext_list_tail = temp;
	}
	
	else
	{
		ext_list_tail->next = temp;
		ext_list_tail = temp;
		temp->next = NULL;
	}

}






/*frees the label list*/

void free_label_list()
{
	label_node_ptr temp;

	while(label_list_head!=NULL)
	{
		temp = label_list_head;
		label_list_head = label_list_head->next;
		free(temp);
	}
}



/*frees the external list*/

void free_ext_list()
{
	label_node_ptr temp;

	while(ext_list_head!=NULL)
	{
		temp = ext_list_head;
		ext_list_head = ext_list_head->next;
		free(temp);
	}
}











