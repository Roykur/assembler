
#include "pre_asm.h"


node_ptr macro_list_head = NULL;

/*READING THE INITIAL INPUT AND IDENFYING AND TRANSLATING MACROS*/

int read_macro(FILE *fp, FILE *fp_new)
{
	
	
	int is_macro_flag=0, index, j; /*flag for if we're currently reading a macro*/
	char buffer[MAX_LINE_SIZE];
	

		
	while(fgets(buffer, MAX_LINE_SIZE, fp) != NULL)
	{
		index = 0;
		/*skip white chars*/
		while(isspace(buffer[index]))
		{
			index++;
		}
		/*empty line*/
		if(buffer[index]=='\0')
			fputs(buffer, fp_new);
		
		else if(is_macro_flag)
		{
		
			if (is_end_macro(index, buffer))
				is_macro_flag = 0;
			
			else
			{
				append_line_to_macro(buffer, macro_list_head);
			}
		}
		
		else
		{
			/*if line is starting as difinging macro*/
			if(is_defining_macro(index, buffer))
			{
				/*skip the word macro*/
				index = index+5;
				
				/*no error*/
				if (!(j = append_macro_name(index, buffer)))
				{		
					is_macro_flag = 1;			
				}	
				
				
				/*there's an error*/
				else
				{
					/*macro name already exists*/
					if(j==1)
					{
					printf("error! trying to define macro twice!\n");
					}
					
					/*editional text after macro name*/
					else
					{
					printf("error! additional text after macro name!\n");
					}
				}
			}
			
			/*normal line - check if it is a macro name */
			else
			{
				if (!(is_macro_name(buffer, index, fp_new)))
					fputs(buffer, fp_new);
				
			}
			
		}/*end of else: is_macro_flag == false */
		
		

	
	}/*END OF FILE*/


	free_list(&macro_list_head);
	
	return 0;
}/*end of read_macro*/



/*checks that buffer holds a valid macro name*/
int is_macro_name(char buffer[], int index, FILE *fp_new)
{
	node_ptr macro_ptr;
	char temp_name[MAX_LINE_SIZE];
	int i=0;

	while(!(isspace(buffer[index])))
	{
		temp_name[i] = buffer [index];
		i++;
		index++;
	}

	/*does this line starts defining a macro?*/
	
	macro_ptr = search_name(temp_name, macro_list_head);

	if (macro_ptr != NULL)
	{
		
		while(isspace(buffer[index]))
			index++;
		
		
		if(buffer[index]=='\0')
		{
			apply_macro(macro_ptr, fp_new);
			return 1;
		}
		
		else
			return 0; /*there is additional text after macro name*/
	}
	
	return 0;
	
}


/*applying the macro from macro_node to file*/
void apply_macro(node_ptr macro_node, FILE *fp_new)
{
	str_node_ptr temp = macro_node->key_head;
	
	while(temp != NULL)
	{
		fputs(temp->key, fp_new);
		temp = temp->next;
	}


}







/*check if first word in the line is "macro"*/

int is_defining_macro(int index, char buffer[])
{
	int j=0, flag=1;
	char str[5] = "macro";
	while (((j<5) && (buffer[j]!='\0')) && (flag))
	{
		if(str[j]!=buffer[index])
			flag=0;
		index++;
		j++;
	}
	
	if(flag)
		return 1;
		
	return 0;
	
}

/*checks if buffer hold "endmacro"*/
int is_end_macro(int index, char buffer[])
{
	int j=0, flag=1;
	char str[8] = "endmacro";
	while (((j<7) && (buffer[j]!='\0')) && (flag))
	{
		if(str[j]!=buffer[index])
			flag=0;
		index++;
		j++;
	}
	
	if(flag)
		return 1;
	return 0;

}




/*get the macro name and add it to data base*/
int append_macro_name(int index, char buffer[])
{
	int j=0;
	char temp_str[MAX_LINE_SIZE];
	
	while(isspace(buffer[index]))
		index++;
		
	/*copying the macro name to tmep_str*/
	while(!(isspace(buffer[index])))
	{
		temp_str[j] = buffer[index];
		index++;
		j++;
		/*need to assign a database for macro names*/
	}
	

	
	/*skip whitechars after macro name*/
	while(isspace(buffer[index]))
	{
		index++;
	}
	
	if(buffer[index] == '\0')/* macro is valid*/
	{
		/*append the macro name that is in temp_str to the macro database*/
		
		
		
		if ((macro_list_head == NULL) || (NULL == (search_name(temp_str, macro_list_head)))) /*check if name exists already*/
		{

			add_name_to_list(temp_str, &macro_list_head); /*create and add new macro name to begining of list*/
			return 0; /*valid*/
		}
		else
		{
			return 1;/*error! trying to define a macro for the second time!*/
		}
	}	
		
	return 2; /*there was additional text after macro name therefore not a macro*/


}























