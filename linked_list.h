#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 80





/*a 2 way linked list of strings. each node holds as a key a line of size MAX_LINE_SIZE (defined in "readmacro.h").*/

typedef struct str_node *str_node_ptr;

typedef struct str_node{
	str_node_ptr next;
	str_node_ptr prev;
	char key[MAX_LINE_SIZE];
}str_node;


/*A NODE THAT REPRESENTS A MACRO.*/
/*macro_name: hold the name of the macro*/
/*key: holds a pointer to a linked list that holds the lines of the macro*/


typedef struct node *node_ptr;

typedef struct node{

	node_ptr next;
	char macro_name[MAX_LINE_SIZE];
	str_node_ptr key_head;
	str_node_ptr key_tail;

}node;


node_ptr search_name(char name[], node_ptr head);

void add_name_to_list(char str_temp[], node_ptr *head);

void append_line_to_macro(char buffer[], node_ptr macro_list_head);

void free_list(node_ptr *head);

void free_str_list(str_node_ptr *head);






