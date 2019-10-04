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
	char			*short_name = NULL;

	if (strlen(begin->file_name) > 74) {
		short_name = (char *)calloc(sizeof(char *), MAX_LINE_PRINT + 1);
		if (short_name != NULL) {
			strncat(short_name, begin->file_name, 71);
			strcat(short_name, "...");
		}
	}

	if (begin->path == NULL || stat(begin->path, &st) == -1) {
		printf("%sFile error%s\n", CONSOLE_RED, CONSOLE_WHITE);
	} else {
		if (S_ISDIR(st.st_mode)) {
			if (short_name == NULL) {
				printf("%s%s%s\n", CONSOLE_YELLOW, begin->file_name, CONSOLE_WHITE);
			} else {
				printf("%s%s%s", CONSOLE_YELLOW, short_name, CONSOLE_WHITE);
			}
		} else if (S_ISREG(st.st_mode)) {
			if (short_name == NULL) {
				printf("%s%s%s\n", CONSOLE_CYAN, begin->file_name, CONSOLE_WHITE);
			} else {
				printf("%s%s%s", CONSOLE_CYAN, short_name, CONSOLE_WHITE);
			}
		} else {
			printf("%s\n", begin->file_name);
		}
	}

	free(short_name);
	short_name = NULL;
}

void	print_directory(t_files_select *begin, size_t cursor)
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

void	print_header(void)
{
	printf("\x1b[0;0H");
	printf("%s", CONSOLE_WHITE);
	printf("================================================================================");
	printf("%s%52s%s", CONSOLE_GREEN, "BackupNX v0.2 (By SegFault42)\n", CONSOLE_WHITE);
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

