#include "common.h"

bool	isFileExist(const char *file)
{
	struct stat	st = {0};

	if (stat(file, &st) == -1) {
		return (false);
	}

	return (true);
}

t_files_select	*getFilesList(const char *path)
{
	t_files_select	*new_node = NULL;
	t_files_select	*begin = NULL;
	DIR				*dirp = NULL;
	struct dirent	*dirent = NULL;

	// open directory
	dirp = opendir(path);
	if (dirp == NULL) {
		return (NULL);
	}

	// store all filename in linked list
	while ((dirent = readdir(dirp)) != NULL) {
		new_node = create_node();
		if (new_node) {
			new_node->file_name = strdup(dirent->d_name);

			strcat(new_node->path, path);
			if (new_node->path[strlen(new_node->path) -1] != '/')
				strcat(new_node->path, "/");
			strcat(new_node->path, new_node->file_name);

			begin = insert_node(begin, new_node);
		}
	}

	closedir(dirp);

	return (begin);
}

void	chooseFileToUpload(t_files_select *files)
{
	(void)files;
}
