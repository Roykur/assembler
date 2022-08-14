#include "command_line.h"
#include <ctype.h>



void copy_file_names(char *new_names[], char *old_names[], int argc)
{
	int i;
	
	for(i=0; i<argc-1 ; i++)
	{
		
		new_names[i] = (char*) malloc(sizeof(char) * (strlen(old_names[i+1]) + strlen(FILE_EXTENSION)));
		
		if(new_names[i] == NULL)
		{
			printf("error! failed to allocate memory");
			exit(0);
		}
		
		strcpy(new_names[i], old_names[i+1]);
	
	}




}


/*
/	gets a string as argument and splits it to seperate words
/	first string will contain the number of words
*/

void split_line(char buffer[], char *line[])
{
	int i=0, j=0, k=0;
	char temp_str[MAX_LINE_SIZE];
	
	while(isspace(buffer[i]))
		{
			i++;
		}
	
	
	while(buffer[i]!='\0')
	{
		
	
		while((buffer[i]!='\0') && ((!(isspace(buffer[i]))) && (buffer[i]!=',')))
		{
			temp_str[j] = buffer[i];
			j++;
			i++;
		}
		
		
		while(isspace(buffer[i]))
		{
			i++;
		}
		
		
		line[k] = (char*) malloc(j*sizeof(char));
		
		if(line[k] == NULL)
		{
			printf("memory allocatin failed!\n");
			exit(0);
		}
		
		
		
		strncpy(line[k], temp_str, j);
		k++;
		j=0;
		
		/*if next char is comma*/
		if(buffer[i]==',')
		{
			line[k] = (char*) malloc(sizeof(','));
		
			if(line[k] == NULL)
			{
				printf("memory allocatin failed!\n");
				exit(0);
			}
			

			strcpy(line[k], ",");
			k++;
			i++;
			

		}
		
		
		
		while(isspace(buffer[i]))
		{
			i++;
		}
	
	}
	
	
	line[k] = NULL;

}


/* frees all the string from line */

void free_line(char **line)
{
	int i=0;



	while(line[i])
	{
		free(line[i]);
		i++;
	}



}





