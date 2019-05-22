#include "common.h"

size_t	count_elem_in_list(t_files_select *list)
{
	t_files_select	*tmp = list;
	size_t			count = 0;

	while (tmp) {
		count++;
		tmp = tmp->next;
	}

	return (count);
}

void	free_list(t_files_select *list)
{
	t_files_select	*first = list;
	t_files_select	*tmp = NULL;

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
t_files_select	*add_tails(t_files_select *node)
{
	t_files_select	*first = node; // keep first elem
	t_files_select	*new_node = NULL;

	// alloc new node
	new_node = calloc(sizeof(t_files_select), 1);
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

/*t_files_select	*add_tails(void *fil, const char *name)*/
/*{*/
	/*// keep ptr on first node*/
	/*t_files_select	*ptr = (t_files_select *)files;*/
	/*t_files_select	*files = (t_files_select *)fil;*/

	/*// create first node*/
	/*if (files == NULL) {*/
		/*files = calloc(sizeof(t_files_select), 1);*/
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
		/*files->next = calloc(sizeof(t_files_select), 1);*/
		/*if (files->next == NULL) {*/
			/*return (NULL);*/
		/*}*/
		/*files->next->file_name = strdup(name);*/
		/*files = ptr;*/
	/*}*/

	/*return (files);*/
/*}*/
