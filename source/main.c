#include "common.h"

int main(void)
{
	t_files	*s_files = NULL;

	s_files = init();
	if (s_files == NULL)
		{ return (0); }

	get_files(s_files);

	while (1) {
		consoleClear();
		print_header();
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		// Print all files in curent dir
		print_directory(s_files->begin, s_files->cursor);

		// point begin to begining of the lst
		if (s_files->begin == NULL) {
			s_files->begin = s_files->files;
		}

		if (input(kDown, s_files) == false) {
			break ;
		}

		print_footer();

		consoleUpdate(NULL);
	}

	de_init(s_files);

	return 0;
}
