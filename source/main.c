#include "common.h"

int main(void)
{
	s_files_select	*files = NULL; // head pointer
	s_files_select	*begin = NULL; // pointer to move in linked list
	size_t			cursor = 0;
	size_t			nb_elem = 0;
	char			path[PATH_MAX] = "/";

	consoleInit(NULL);
	socketInitializeDefault();

	// get all files in /
	files = getFilesList(path);
	begin = files;
	nb_elem = count_elem_in_list(begin);

	while (1) {
		consoleClear();
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		printf("Current dir = %s\n\n", path);

		// Print all files in curent dir
		for (size_t i = 0; begin; i++) {
			if (i == cursor) { printf("> "); }
			else { printf("  "); }
			if (begin->select == true) { printf("[X] "); }
			else { printf("[ ] "); }

			printf("%s\n", begin->file_name);

			begin = begin->next;
		}

		// point begin to begining of the lst
		if (begin == NULL) { begin = files;}

		if (kDown & KEY_DOWN) {
			cursor++;
			if (cursor == nb_elem)
				{ cursor = 0; }
		}

		if (kDown & KEY_X) {
			for (size_t i = 0; i < cursor; i++) {
				begin = begin->next;
			}
			// flip bool
			begin->select = !begin->select;
		}

		if (kDown & KEY_A) {
			// enter to selected dir and free linked list
			for (size_t i = 0; i < cursor; i++) {
				begin = begin->next;
			}
			memset(path, 0, PATH_MAX);
			strcat(path, begin->path);
			strcat(path, begin->file_name);
			chdir(path);
			free_list(files);
			files = getFilesList(path);
			begin = files;
			nb_elem = count_elem_in_list(begin);
			cursor = 0;
		}

		if (kDown & KEY_PLUS) {
			break; // break in order to return to hbmenu
		}
		consoleUpdate(NULL);
	}

	consoleExit(NULL);
	socketExit();

	return 0;
}
