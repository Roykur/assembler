#include "linked_list.h"




/*given a pointer to the head of the macros list searches if a name of a macro already exists.*/
/*returns: pointer to the searched macro node. if doesn't exist return NULL*/

node_ptr search_name(char name[], node_ptr head)
{
	

	
	while((head!=NULL) && (strlen(name) > 0))
	{
	
		if((strncmp(head->macro_name, name, strlen(name))))
			head = head->next;
			
		else
			return head;
		
	}
	
	return NULL;

}/*end of search name*/


/*we will a new node to the begining of a list because the list is not sorted and there is no signifcance for order of nodes*/
void add_name_to_list(char str_temp[], node_ptr *head)
{
	node_ptr new_node;
	
	new_node = (node_ptr) malloc(sizeof(node));
	
	if (new_node == NULL)
	{
		printf("could not assign memory for new node");
		exit(0);
	}
	
	strcpy(new_node->macro_name, str_temp);
	
	
	
	/*list is empty*/
	if(*head == NULL)
	{
		*head = new_node;
	}
	
	
	else
	{
		new_node->next = *head;
		*head = new_node;
	}
	
}






void append_line_to_macro(char buffer[], node_ptr macro_list_head)
{
	str_node_ptr temp;
	temp = (str_node_ptr) malloc(sizeof(str_node));
	
	if (temp == NULL)
	{
		printf("could not assign memory for new node");
		exit(0);
	}
	
	
	strncpy(temp->key, buffer, MAX_LINE_SIZE);
	
	
	/*FIRST LINE IN MACRO*/
	
	if (macro_list_head->key_head == NULL)
	{
		macro_list_head->key_head = temp;
		macro_list_head->key_tail = temp;
		temp->next = NULL;
		temp->prev = NULL;
	}
	
	/*NOT FIRST LINE IN MACRO*/
	
	else
	{	
		temp->next = NULL;
		temp->prev = macro_list_head->key_tail;
		(temp->prev)->next = temp;
		macro_list_head->key_tail = temp;
	}
		
}/*end of append_line_to_macro*/




void free_list(node_ptr *head)
{
	node_ptr temp;
	while(*head!=NULL)
	{
		free_str_list(&((*head)->key_head));
		temp = *head;
		*head = (*head)->next;
		free(temp);
	
	}

}



void free_str_list(str_node_ptr *head)
{
	str_node_ptr temp;
	while(*head!=NULL)
	{
		temp = *head;
		*head = (*head)->next;
		free(temp);
	}


}








