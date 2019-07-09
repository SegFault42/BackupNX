#include "common.h"

// Incr linked list to scroll
static t_files_select	*scrolling(size_t cursor, t_files_select *begin)
{
	int incr = cursor - MAX_LINE;

	if (cursor >= MAX_LINE) {
		while (incr >= 0) {
			begin = begin->next;
			--incr;
		}
	}

	return (begin);
}

// Print cursor
static void	print_cursor(size_t elem, size_t cursor, int i)
{
	if (elem == cursor) {
		printf("> ");
	} else if (cursor >= MAX_LINE -1 && i == MAX_LINE -1) {
		printf("> ");
	} else {
		printf("  "); }
}

// Print selected file
static void	print_selected_file(bool select)
{
	if (select == true) {
		printf("[X] ");
	} else {
		printf("[ ] ");
	}
}

// Print file name
static void	print_filename(t_files_select *begin)
{
	struct stat		st = {0};

	if (begin->path == NULL || stat(begin->path, &st) == -1) {
		printf("%sFile error%s\n", CONSOLE_RED, CONSOLE_WHITE);
	} else {
		if (S_ISDIR(st.st_mode)) {
			printf("%s%s%s\n", CONSOLE_YELLOW, begin->file_name, CONSOLE_WHITE);
		} else if (S_ISREG(st.st_mode)) {
			printf("%s%s%s\n", CONSOLE_CYAN, begin->file_name, CONSOLE_WHITE);
		} else {
			printf("%s\n", begin->file_name);
		}
	}
}

void	print_directory(t_files_select *begin, size_t cursor, size_t nb_elem)
{
	size_t			elem = 0;

	if (begin == NULL) {
		printf("%s%s%s", CONSOLE_RED, "No such file or directory\n", CONSOLE_WHITE);
	} else {
		begin = scrolling(cursor, begin);

		for (int i = 0; begin && i < MAX_LINE; elem++, i++, begin = begin->next) {
			print_cursor(elem, cursor, i);
			print_selected_file(begin->select);
			print_filename(begin);
		}
	}
}
