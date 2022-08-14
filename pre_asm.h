
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "linked_list.h"


int read_macro(FILE *fp, FILE *fp_new);

int is_macro_name(char buffer[], int index, FILE *fp_new);

void apply_macro(node_ptr macro_node, FILE *fp_new);


int append_macro_name(int index, char buffer[]);

int is_end_macro(int index, char buffer[]);

int is_macro(int index, char buffer[]);

int is_defining_macro(int index, char buffer[]);

