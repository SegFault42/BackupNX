#include "common.h"

void	print_directory(t_files_select *begin, size_t cursor, size_t nb_elem)
{
	size_t			elem = 0;
	struct stat		st = {0};

	if (begin == NULL) {
		printf("No such file or directory\n");
	} else {
		// Scrolling
		int incr = cursor - MAX_LINE;

		/*PRINT_DEBUG*/
		/*printf("%d", incr);*/
		if (cursor >= MAX_LINE) {
			while (incr >=0) {
				begin = begin->next;
				--incr;
			}
		}
		for (int i = 0; begin && i < MAX_LINE; elem++, i++) {
			// Print cursor
			if (elem == cursor) {
	 			printf("> ");
			} else if (cursor >= MAX_LINE -1 && i == MAX_LINE -1) {
	 			printf("> ");
			} else {
				printf("  "); }

			// Print selected
			if (begin->select == true) {
				printf("[X] ");
			} else {
				printf("[ ] ");
			}

			if (begin->path == NULL || stat(begin->path, &st) == -1) {
				printf("%sFile error%s\n", CONSOLE_RED, CONSOLE_RESET);
			} else {
				// Print filename
				if (S_ISDIR(st.st_mode)) {
					printf("%s%s%s\n", CONSOLE_YELLOW, begin->file_name, CONSOLE_RESET);
				} else if (S_ISREG(st.st_mode)) {
					printf("%s%s%s\n", CONSOLE_CYAN, begin->file_name, CONSOLE_RESET);
				} else {
					printf("%s\n", begin->file_name);
				}
			}

			begin = begin->next;
		}
	}
}
