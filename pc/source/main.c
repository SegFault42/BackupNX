#include "common.h"

int main(void)
{
	s_files_select	*files = NULL;


	// get all files in /
	files = getFilesList("/");

	if (files) {
	while (files->next)
	{
		if (files->file_name)
			printf("%s\n", files->file_name);
		files = files->next;
	}
	}



	return 0;
}
