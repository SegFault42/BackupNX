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
		if (s_files->nb_elem > 0) {
			change_directory(s_files, SELECT_DIR);
			// TODO : Potential LEAKS here !!
		}
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

