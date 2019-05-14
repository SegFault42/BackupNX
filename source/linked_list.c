#include "common.h"

size_t	count_elem_in_list(s_files_select *list)
{
	s_files_select	*tmp = list;
	size_t			count = 0;

	while (tmp) {
		count++;
		tmp = tmp->next;
	}

	return (count);
}

void	free_list(s_files_select *list)
{
	s_files_select	*first = list;
	s_files_select	*tmp = NULL;

	if (first != NULL) {
		while (first->next) {
			tmp = first->next;
			free(first->file_name);
			free(first);
			first = tmp;
		}
	}
}

// add new node and return it
s_files_select	*add_tails(s_files_select *node)
{
	s_files_select	*first = node; // keep first elem
	s_files_select	*new_node = NULL;

	// alloc new node
	new_node = calloc(sizeof(s_files_select), 1);
	if (new_node == NULL)
		{ return (NULL); }

	if (node == NULL) {
		node = new_node;
		return (node);
	} else {
		while (node->next)
			{ node = node->next; }
		node->next = new_node;
	}

	node = first;

	return (new_node);
}

/*s_files_select	*add_tails(void *fil, const char *name)*/
/*{*/
	/*// keep ptr on first node*/
	/*s_files_select	*ptr = (s_files_select *)files;*/
	/*s_files_select	*files = (s_files_select *)fil;*/

	/*// create first node*/
	/*if (files == NULL) {*/
		/*files = calloc(sizeof(s_files_select), 1);*/
		/*if (files == NULL) {*/
			/*return (NULL);*/
		/*}*/
		/*files->file_name = strdup(name);*/
	/*} else {*/
		/*// go to last node*/
		/*while (files->next) {*/
			/*files = files->next;*/
		/*}*/

		/*// Create new node*/
		/*files->next = calloc(sizeof(s_files_select), 1);*/
		/*if (files->next == NULL) {*/
			/*return (NULL);*/
		/*}*/
		/*files->next->file_name = strdup(name);*/
		/*files = ptr;*/
	/*}*/

	/*return (files);*/
/*}*/
