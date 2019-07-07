#include "common.h"

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

bool	input(u64 kDown, t_files *s_files)
{
	t_list_files	*list = NULL;

	if (kDown & KEY_DOWN) {
		move_down(s_files);
	}
	if (kDown & KEY_UP) {
		move_up(s_files);
	}

	// select or unselect file
	if (kDown & KEY_X) {
		select_file(s_files);
	}

	if (kDown & KEY_A) {
		change_directory(s_files, SELECT_DIR);
		// TODO : LEAKS here !!
	}

	if (kDown & KEY_Y) {
		// get all selected files and directory
		if (s_files->begin) {
			list = update_list_files(s_files->begin, s_files->path);
		}
		// if files in queu, upload them
		if (list == NULL) {
			printf("No files selected\n");
		} else {
			upload_files(list);
			// TODO: set selected field to false in list
			free_2d_array(&list->files);
			free(list->directory);
			list->directory = NULL;
			free(list);
			list = NULL;
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
	
	if (isFileExist("/switch/BackupNX") == false) {
		mkdir("/switch/BackupNX", 0777);
	}
	if (isFileExist("/switch/BackupNX/cred.json") == false) {
		printf("%s/switch/BackupNX/cred.json missing !%s", CONSOLE_RED, CONSOLE_RESET);
		consoleUpdate(NULL);
		sleep(5);
		consoleExit(NULL);
		return (NULL);
	}

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
		{ return (0); }

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
