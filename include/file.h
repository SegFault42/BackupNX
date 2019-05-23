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

bool			isFileExist(const char *file);
t_files_select	*getFilesList(const char *path);
void			chooseFileToUpload(t_files_select *);

#endif
