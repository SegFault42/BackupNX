#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct		s_files_select
{
	char					*file_name;	// list of files in directory
	bool					select;
	struct s_files_select	*next;
}					s_files_select;

s_files_select	*add_tails(s_files_select *files, const char *name);

#endif
