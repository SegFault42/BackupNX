#include "common.h"

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

void	move_down(t_files *s_files)
{
	s_files->cursor++;
	if (s_files->cursor == s_files->nb_elem)
		{ s_files->cursor = 0; }
}

void	move_up(t_files *s_files)
{
	s_files->cursor--;
	if (s_files->cursor == (size_t)-1)
		{ s_files->cursor = s_files->nb_elem -1; }
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

static void	free_2d_array(char ***array)
{
	for (size_t idx = 0; array[idx]; idx++) {
		free(*array[idx]);
		*array[idx] = NULL;
	}

	free(*array);
	*array = NULL;
}

static char	**update_list_files(t_files_select *s_files)
{
	char	**list_files = NULL;

	// Alloc 2d array
	if (list_files == NULL) {
		list_files = calloc(sizeof(char *), 1 + 1); // 1st ptr to content, 2nd to NULL
		if (list_files == NULL) {
			// TODO : print error if alloc fail
			return (NULL);
		}
	}

	// add first elem (the directory name)
	list_files[0] = strdup(s_files->path);

	// Add selected elem in array
	for (int nb_elem = 1; s_files; s_files = s_files->next) {
		if (s_files->select == true) {
			// nb_elem + new_elem + NULL
			list_files = realloc(list_files, sizeof(char *) * (nb_elem + 1 + 1));
			if (list_files == NULL) {
				return (NULL);
			}
			list_files[nb_elem] = strdup(s_files->file_name);
			nb_elem++;
		}
	}

	// nothing to add, no files selected
	if (list_files[1] == NULL) {
		// free array
		free_2d_array(&list_files);
	}

	return (list_files);
}

typedef struct	s_list_files
{
	char	directory[PATH_MAX];
	char	**files;
}				t_list_files;

bool	input(u64 kDown, t_files *s_files)
{
	char	**list_files = NULL;

	if (kDown & KEY_DOWN) {
		move_down(s_files);
	}
	if (kDown & KEY_UP) {
		move_up(s_files);
	}

	// select or unselet file
	if (kDown & KEY_X) {
		select_file(s_files);
	}

	if (kDown & KEY_A) {
		if (s_files->begin) {
			list_files = update_list_files(s_files->begin);
			if (list_files != NULL) {
				// Print do debug
				print_2d_array(list_files);
				consoleUpdate(NULL);
				sleep(4);
				// free temporary
				free_2d_array(&list_files);
			}

			change_directory(s_files, SELECT_DIR);
		}
	}

	if (kDown & KEY_B) {
		back_directory(s_files);
	}

	if (kDown & KEY_PLUS) {
		return (false); // break in order to return to hbmenu
	}


	return (true);
}

// Init console, socket and struct with path point to sdmc:/
t_files	*init(void)
{
	t_files	*ptr = NULL;

	consoleInit(NULL);
	socketInitializeDefault();

	ptr = calloc(sizeof(t_files), 1);
	if (!ptr)
		{ return (NULL); }

	ptr->path[0] = '/';

	return (ptr);
}

void	de_init(t_files *s_files)
{
	consoleExit(NULL);
	socketExit();
	free_list(s_files->files);
}

// tab = 	{"dir_name", "file1", "file2"}
// 			{"dir_name", "file1", "file2"}

int main(void)
{
	t_files	*s_files = NULL;

	s_files = init();
	if (s_files == NULL)
		{ return (EXIT_FAILURE); }

	get_files(s_files);

	while (1) {
		consoleClear();
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		// Only for debug
		if (s_files && s_files->begin) {
			printf("%s | %s\n", s_files->begin->path, s_files->path);
		}

		// Print all files in curent dir
		print_directory(s_files->begin, s_files->cursor, s_files->nb_elem);

		// point begin to begining of the lst
		if (s_files->begin == NULL) {
			s_files->begin = s_files->files;
		}

		if (input(kDown, s_files) == false) {
			break ;
		}

		consoleUpdate(NULL);
	}

	de_init(s_files);

	return 0;
}
