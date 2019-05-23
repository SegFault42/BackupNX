#include "common.h"

void	print_directory(t_files_select *begin, size_t cursor)
{
	size_t	elem = 0;

	for (; begin; elem++) {
		// Print cursor
		if (elem == cursor) {
 			printf("> ");
		} else {
			printf("  "); }

		// Print selected
		if (begin->select == true) {
			printf("[X] ");
		} else {
			printf("[ ] ");
		}

		// Print filename
		printf("%s\n", begin->file_name);

		begin = begin->next;
	}

	if (elem == 0) {
		printf("No such file or directory\n");
	}
}

void	change_directory(t_files *s_files, char *dir)
{
	char	save_dir[PATH_MAX] = {0};
	t_files_select	*s_files_save = s_files->begin;

	strcpy(save_dir, s_files->path);

	// enter to selected dir and free linked list
	memset(s_files->path, 0, PATH_MAX);

	if (dir == NULL) {
		for (size_t i = 0; i < s_files->cursor; i++) {
			s_files->begin = s_files->begin->next;
		}
		strcat(s_files->path, s_files->begin->path);
		strcat(s_files->path, s_files->begin->file_name);
	} else {
		strcat(s_files->path, dir);
	}

	struct stat	st;

	stat(s_files->path, &st);

	if (S_ISDIR(st.st_mode) && chdir(s_files->path) != -1) {
		free_list(s_files->files);
		s_files->files = getFilesList(s_files->path);
		s_files->begin = s_files->files;
		s_files->nb_elem = count_elem_in_list(s_files->begin);
		s_files->cursor = 0;
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
	if (s_files->begin) {
		for (size_t i = 0; i < s_files->cursor; i++) {
			s_files->begin = s_files->begin->next;
		}
		// flip bool
		s_files->begin->select = !s_files->begin->select;
	}
}

// go back in tree
void	back_directory(t_files *s_files)
{
	char	old_path[PATH_MAX] = {0};
	char	*tmp = NULL;

	// if "/switch/dir" else ../ is "/"
	if (strchr(&s_files->path[1], '/')) {
		strcpy(old_path, s_files->path);
		tmp = strrchr(old_path, '/');
		if (tmp) {
			*tmp = '\0';
		}
	} else {
		old_path[0] = '/';
	}

	change_directory(s_files, old_path);
}

int main(void)
{
	t_files			*s_files = NULL;

	consoleInit(NULL);
	socketInitializeDefault();

	s_files = calloc(sizeof(t_files), 1);
	if (!s_files) {
		return (-1);
	}

	s_files->path[0] = '/';
	// get all files in /
	s_files->files = getFilesList(s_files->path);
	s_files->begin = s_files->files;
	s_files->nb_elem = count_elem_in_list(s_files->begin);


	while (1) {
		consoleClear();
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		// DEBUG
		if (s_files->begin)
			printf("[DEBUG] Current dir = %s\n, path = %s\n\n", s_files->path, s_files->begin->path);

		// Print all files in curent dir
		print_directory(s_files->begin, s_files->cursor);

		// point begin to begining of the lst
		if (s_files->begin == NULL) {
			s_files->begin = s_files->files;
		}

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
				change_directory(s_files, NULL);
			}
		}

		if (kDown & KEY_B) {
			back_directory(s_files);
		}

		consoleUpdate(NULL);

		if (kDown & KEY_PLUS) {
			break; // break in order to return to hbmenu
		}
		consoleUpdate(NULL);
	}

	consoleExit(NULL);
	socketExit();
	free(s_files);

	return 0;
}
