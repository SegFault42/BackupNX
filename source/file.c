#include "common.h"

bool	isFileExist(const char *file)
{
	struct stat	st = {0};

	if (stat(file, &st) == -1) {
		return (false);
	}

	return (true);
}

// Get files in current directory
static t_files_select	*getFilesList(const char *path)
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
				{ strcat(new_node->path, "/"); }
			strcat(new_node->path, new_node->file_name);

			begin = insert_node(begin, new_node);
		}
	}

	closedir(dirp);

	return (begin);
}

// Get files in current directory and fill s_files struct
void	get_files(t_files *s_files)
{
	s_files->files = getFilesList(s_files->path);
	s_files->begin = s_files->files;
	s_files->nb_elem = count_elem_in_list(s_files->begin);
	s_files->cursor = 0;
}


void	chooseFileToUpload(t_files_select *files)
{
	(void)files;
}

t_list_files	*update_list_files(t_files_select *s_files, char *cur_path)
{
	t_list_files	*list = NULL;

	list = (t_list_files *)calloc(sizeof(t_list_files), 1); // alloc struct
	if (!list) {
		return (NULL);
	}

	// add first elem (the directory name)
	list->directory = strdup(cur_path);
	if (!list->directory) {
		return (NULL);
	}

	// alloc just NULL ptr
	list->files = (char **)calloc(sizeof(char *), 1);
	if (!list->files) {
		return (NULL);
	}

	// Add selected elem in array
	for (int nb_elem = 0; s_files; s_files = s_files->next) {
		if (s_files->select == true) {
			// nb_elem + new_elem + NULL
			list->files = (char **)realloc(list->files, sizeof(char *) * (nb_elem + 1 + 1));
			if (list->files == NULL) {
				return (NULL);
			}
			list->files[nb_elem] = strdup(s_files->file_name);
			if (list->files[nb_elem] == NULL) {
				return (NULL);
			}
			nb_elem++;
		}
	}

	// nothing to add, no files selected
	if (list->files[0] == NULL) {
		free(list->directory);
		list->directory = NULL;
		free_2d_array(&list->files);
		free(list);
		list = NULL;
	}

	return (list);
}

void	change_directory(t_files *s_files, bool mode)
{
	t_files_select	*s_files_save = s_files->begin;
	char			save_dir[PATH_MAX] = {0};
	struct stat		st;

	strcpy(save_dir, s_files->path);

	// enter to selected dir and free linked list

	if (mode == SELECT_DIR) {
		for (size_t i = 0; i < s_files->cursor; i++)
			{ s_files->begin = s_files->begin->next; }
		if (s_files->path[strlen(s_files->path) -1] != '/')
			{ strcat(s_files->path, "/"); }
		strcat(s_files->path, s_files->begin->file_name);
		strcat(s_files->path, "/");
	}

	stat(s_files->path, &st);

	if (S_ISDIR(st.st_mode) && chdir(s_files->path) != -1) {
		free_list(s_files->files);
		get_files(s_files);
	} else {
		memset(s_files->path, 0, PATH_MAX);
		strcpy(s_files->path, save_dir);
		s_files->begin = s_files_save;
	}
}


// go back in tree
void	back_directory(t_files *s_files)
{
	char	*tmp = NULL;
	size_t	len = 0;

	// if already in / return
	if (!strcmp(s_files->path, "/")) {
		return ;
	}

	len = strlen(s_files->path);

	// remove last /
	if (s_files->path[len -1] == '/')
		s_files->path[len -1] = '\0';

	// if "/switch/dir" else ../ is "/"
	if (strchr(&s_files->path[1], '/')) {
		tmp = strrchr(s_files->path, '/');
		if (tmp) {
			*tmp = '\0';
		}
	} else {
		strcpy(s_files->path, "/");
	}

	change_directory(s_files, 0);
}

void	select_file(t_files *s_files)
{
	t_files_select	*s_tmp = s_files->begin;

	if (s_files->begin) {
		for (size_t i = 0; i < s_files->cursor; i++) {
			s_files->begin = s_files->begin->next;
		}
		// flip bool
		s_files->begin->select = !s_files->begin->select;
	}

	s_files->begin = s_tmp;
}

