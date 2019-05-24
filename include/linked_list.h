#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct		t_files_select
{
	char					*file_name;	// name of file
	char					path[PATH_MAX];		// path of file
	bool					select;
	struct t_files_select	*next;
}					t_files_select;

//void			add_tails(t_files_select *files);
void			free_list(t_files_select *begin);
size_t			count_elem_in_list(t_files_select *list);
t_files_select	*create_node(void);
t_files_select	*insert_node(t_files_select *begin, t_files_select *node);

#endif
