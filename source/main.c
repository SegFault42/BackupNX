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
		printf("%s/switch/BackupNX/cred.json missing !%s", CONSOLE_RED, CONSOLE_WHITE);
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

void	print_header(void)
{
	printf("\x1b[0;0H");
	printf("%s", CONSOLE_WHITE);
	printf("================================================================================");
	printf("%s%52s%s", CONSOLE_GREEN, "BackupNX v0.1 (By SegFault42)\n", CONSOLE_WHITE);
	printf("================================================================================");
}

void	print_footer(void)
{
	printf("\x1b[43;0H");
	printf("================================================================================");
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			CONSOLE_RED, "A", CONSOLE_WHITE, " = ", CONSOLE_MAGENTA, "Enter directory", CONSOLE_WHITE, " | ",
			CONSOLE_RED, "B", CONSOLE_WHITE, " = ", CONSOLE_MAGENTA, "Back directory", CONSOLE_WHITE, " | ",
			CONSOLE_RED, "X", CONSOLE_WHITE, " = ", CONSOLE_MAGENTA, "Select files", CONSOLE_WHITE, " | ",
			CONSOLE_RED, "Y", CONSOLE_WHITE, " = ", CONSOLE_MAGENTA, "Upload Files\n", CONSOLE_WHITE
			);
	printf("================================================================================");
}

int main(void)
{
	t_files	*s_files = NULL;

	s_files = init();
	if (s_files == NULL)
		{ return (0); }

	get_files(s_files);

	/*PrintConsole mainWindow, titleWindow, errorWindow;*/
	/*consoleInit(&titleWindow);*/
	/*consoleInit(&mainWindow);*/
	/*consoleInit(&errorWindow);*/

	/*consoleSetWindow(&titleWindow, 0, 0, 80, 3);*/
	/*consoleSetWindow(&mainWindow, 0, 3, 80, 43);*/
	/*consoleSetWindow(&errorWindow, 0, 42, 80, 3);*/

	/*consoleSelect(&titleWindow);*/
	/*print_header();*/


	while (1) {
		/*consoleSelect(&mainWindow);*/
		consoleClear();
		print_header();
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		// Print all files in curent dir
		print_directory(s_files->begin, s_files->cursor, s_files->nb_elem);

		// point begin to begining of the lst
		if (s_files->begin == NULL) {
			s_files->begin = s_files->files;
		}

		if (input(kDown, s_files) == false) {
			break ;
		}

		/*consoleSelect(&errorWindow);*/
		print_footer();

		consoleUpdate(NULL);
	}

	de_init(s_files);

	return 0;
}
