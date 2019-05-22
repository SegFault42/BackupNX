#include "common.h"

void	print_directory(t_files_select *begin, size_t cursor)
{
	for (size_t i = 0; begin; i++) {
		if (i == cursor) {
 			printf("> ");
		} else {
			printf("  ");
		}

		if (begin->select == true) {
			printf("[X] ");
		} else {
			printf("[ ] ");
		}

		printf("%s\n", begin->file_name);

		begin = begin->next;
	}
}

void	change_directory(t_files *s_files)
{
	// enter to selected dir and free linked list
	for (size_t i = 0; i < s_files->cursor; i++) {
		s_files->begin = s_files->begin->next;
	}

	memset(s_files->path, 0, PATH_MAX);

	strcat(s_files->path, s_files->begin->path);
	strcat(s_files->path, s_files->begin->file_name);

	if (chdir(s_files->path) != -1) {
		free_list(s_files->files);
		s_files->files = getFilesList(s_files->path);
		s_files->begin = s_files->files;
		s_files->nb_elem = count_elem_in_list(s_files->begin);
		s_files->cursor = 0;
	}
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
	/*s_files->old_path[0] = '/';*/
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
		if (s_files->begin == NULL) { s_files->begin = s_files->files; }

		if (kDown & KEY_DOWN) {
			s_files->cursor++;
			if (s_files->cursor == s_files->nb_elem)
				{ s_files->cursor = 0; }
		}
		if (kDown & KEY_UP) {
			s_files->cursor--;
			if (s_files->cursor == (size_t)-1)
				{ s_files->cursor = s_files->nb_elem -1; }
		}

		// select or unselet file
		if (kDown & KEY_X) {
			if (s_files->begin) {
				for (size_t i = 0; i < s_files->cursor; i++) {
					s_files->begin = s_files->begin->next;
				}
				// flip bool
				s_files->begin->select = !s_files->begin->select;
			}
		}

		if (kDown & KEY_A) {
			if (s_files->begin) {
				change_directory(s_files);
			}
		}

		if (kDown & KEY_B) {
			char	old_path[PATH_MAX] = {0};
			char	*tmp = NULL;

			strcpy(old_path, s_files->path);
			tmp = strrchr(old_path, '/');
			if (tmp) {
				*tmp = '\0';
				memset(s_files->begin->path, 0, PATH_MAX);
				memset(s_files->begin->file_name, 0, PATH_MAX);
				strcat(s_files->begin->path, old_path);
				change_directory(s_files);
			}
			printf("tmp = %s\n", tmp);
			printf("old_path = %s\n", old_path);
			printf("path = %s\n", s_files->path);
			consoleUpdate(NULL);
			sleep(2);
		}

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
