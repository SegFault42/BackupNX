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

static void	update_list_files(t_files_select *s_files, char **list_files)
{
	while (s_files) {
		printf("%s[%s]%s", CONSOLE_RED, s_files->file_name, CONSOLE_RESET);
		s_files = s_files->next;
	}
	consoleUpdate(NULL);
	sleep(1);
}

bool	input(u64 kDown, t_files *s_files, char **list_files)
{
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
		update_list_files(s_files->begin, list_files);
		if (s_files->begin) {
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

void	get_files(t_files *s_files)
{
	s_files->files = getFilesList(s_files->path);
	s_files->begin = s_files->files;
	s_files->nb_elem = count_elem_in_list(s_files->begin);
	s_files->cursor = 0;
}

// tab = 	{"dir_name", "file1", "file2"}
// 			{"dir_name", "file1", "file2"}

int main(void)
{
	t_files	*s_files = NULL;
	char	**list_files = NULL;

	s_files = init();
	if (s_files == NULL)
		{ return (EXIT_FAILURE); }

	get_files(s_files);

	while (1) {
		consoleClear();
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (s_files && s_files->begin) {
			printf("%s | %s\n", s_files->begin->path, s_files->path);
		}

		// Print all files in curent dir
		print_directory(s_files->begin, s_files->cursor, s_files->nb_elem);

		// point begin to begining of the lst
		if (s_files->begin == NULL) {
			s_files->begin = s_files->files;
		}

		if (input(kDown, s_files, list_files) == false)
			{ break ; }

		consoleUpdate(NULL);
	}

	consoleExit(NULL);
	socketExit();
	free_list(s_files->files);

	return 0;
}
