#include <stdio.h>
#include <stdlib.h>
#include "pre_asm.h"
#include "asm_1.h"
#include "command_line.h"
#include "asm_2.h"
#include "label_list.h"


int main(int argc,char *argv[])
{
	FILE *fp, *fp2;
	char **file_names;
	int i, str_len;
	
	
	file_names = (char**) malloc(sizeof(char*) * (argc-1));
	
	if(file_names == NULL)
	{
		printf("error! failed to allocate memory\n");
		exit(0);
	}
	
	
	copy_file_names(file_names, argv, argc);
	
	
	for(i=0; i<argc-1; i++)
	{
		str_len = strlen(file_names[i]);
		
		strcpy(file_names[i]+str_len, FILE_EXTENSION);
		fp = fopen( file_names[i], "r");

		if (fp == NULL)
		{
			printf("ERROR! could not find file '%s'\n", file_names[i]);
			exit(0);
		}
		
		strcpy(file_names[i]+str_len, ".am"); 
		fp2 = fopen(file_names[i], "w+");

		if (fp2 == NULL)
  		{
			printf("ERROR! could not create file '%s'\n", file_names[i]);
			exit(0);
		}
		
		read_macro(fp, fp2);
		rewind(fp2);
		
		translate(fp2);
		rewind(fp2);
		
		/* if there were no errors during translation then create output files*/
		if(translate_2(fp2))
		{

			strcpy(file_names[i]+str_len, ".ent");
			write_output_ent(file_names[i]);

			strcpy(file_names[i]+str_len, ".ext");
			write_output_ext(file_names[i]);

			strcpy(file_names[i]+str_len, ".ob");
			write_output_ob(file_names[i]);
		}
		
		
		fclose(fp);
		fclose(fp2);
	}
	
	free(file_names);
	
	return 0;
}
