#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct		s_files_select
{
	char					*file_name;	// name of file
	char					path[PATH_MAX];		// path of file
	bool					select;
	struct s_files_select	*next;
}					s_files_select;

s_files_select	*add_tails(s_files_select *files);
void			free_list(s_files_select *begin);
size_t			count_elem_in_list(s_files_select *list);

#endif
