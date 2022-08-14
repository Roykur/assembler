#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FIRST_MEMORY_SLOT 100
#define MAX_LINE_SIZE 80
#define MAX_WORDS 40
#define MAX_FILE_NAME 80
#define FILE_EXTENSION ".as"

/*
/	gets a string as argument and splits it to seperate words
/	first string will contain the number of words
*/

void split_line(char buffer[], char *line[]);

void copy_file_names(char *new_names[], char *old_names[], int argc);

void free_line(char **line);

void free_line_ptr(char ***line);
