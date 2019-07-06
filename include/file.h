#ifndef FILE_H
#define FILE_H

#include <dirent.h>
#include "linked_list.h"

typedef struct		s_files
{
	t_files_select	*files;				// head pointer
	t_files_select	*begin;				// pointer to move in linked list
	size_t			cursor;				// cursor to select file
	size_t			nb_elem;			// nb of files in cur dir
	char			path[PATH_MAX];		// current path name
	char			old_path[PATH_MAX];	// old path name
}					t_files;

typedef struct	s_list_files
{
	char		*directory;
	char		**files;
}				t_list_files;

bool			isFileExist(const char *file);
void			get_files(t_files *s_files);
void			chooseFileToUpload(t_files_select *);
t_list_files	*update_list_files(t_files_select *s_files, char *cur_path);
void			change_directory(t_files *s_files, bool mode);
void			back_directory(t_files *s_files);
void			select_file(t_files *s_files);

#endif
