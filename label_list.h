#ifndef label_list_H_

#define label_list_H_



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_LABEL_SIZE 31

#define MAX_WORDS 40

#define equals(str1, str2) (strcmp(str1, str2) == 0)

typedef enum {data , entry, external, code} type;

typedef struct label_node *label_node_ptr;

typedef struct label_node
{
	label_node_ptr next;
	char name[MAX_LABEL_SIZE];
	int address;
	type label_type;
	type label_src_type;
}label_node;

/*prototypes*/

label_node_ptr search_label_name(char str[]);

int is_label(char *line[]);

void copy_label_name(char *label, char *str);

int append_label(char label[], type label_type);

void free_label_list();

void free_entry_list();

void free_ext_list();

void append_extern_label(char *line[], int i);

void append_entry_label(char *line[], int i);

void offset_data_labels(int i);

void append_ext_address(label_node_ptr extern_node);


#endif /*label_list_H_*/
