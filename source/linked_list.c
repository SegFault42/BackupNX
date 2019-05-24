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

t_files_select	*create_node(void)
{
	t_files_select	*new_node = calloc(sizeof(t_files_select), 1);

	if (new_node == NULL)
		{ return (NULL); }

	return (new_node);
}


// add new node and return it
static void	add_tails(t_files_select *begin, t_files_select *node)
{
	t_files_select	*tmp = begin; // keep first elem

	while (tmp->next) {
		tmp = tmp->next;
	}
	tmp->next = node;
}

t_files_select	*insert_node(t_files_select *begin, t_files_select *node)
{
	t_files_select	*tmp = begin;
	int				cmp = 0;

	if (begin == NULL) {
		begin = node;
	} else {
		while (tmp) {
			cmp = strcmp(node->file_name, tmp->file_name);

			if (cmp < 0) {
				if (begin == tmp) {
					// add_head();
					node->next = begin;
					begin = node;
				} else if (tmp->next == NULL) {
					add_tails(tmp, node);
				} else {
					// add_after();
					node->next = tmp->next;
					tmp->next = node;
				}
				break ;
			}
			tmp = tmp->next;
		}
	}

	return (begin);
	// a						hello
	// bonjour, ca_va, dear,        makaveli
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
