#include "common.h"

void	print_directory(t_files_select *begin, size_t cursor)
{
	size_t	elem = 0;

	if (begin == NULL) {
		printf("No such file or directory\n");
	} else {
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
	}
}

void	change_directory(t_files *s_files, bool mode)
{
	t_files_select	*s_files_save = s_files->begin;
	char			save_dir[PATH_MAX] = {0};
	struct stat		st;

	strcpy(save_dir, s_files->path);

	// enter to selected dir and free linked list

	if (mode == SELECT_DIR) {
		for (size_t i = 0; i < s_files->cursor; i++) {
			s_files->begin = s_files->begin->next;
		}
		strcat(s_files->path, s_files->begin->file_name);
		strcat(s_files->path, "/");
	}

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
				change_directory(s_files, SELECT_DIR);
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
