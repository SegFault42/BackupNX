#include "common.h"

s_files_select	*add_tails(s_files_select *files, const char *name)
{
	// keep ptr on first node
	s_files_select	*ptr = files;

	// create first node
	if (files == NULL) {
		files = calloc(sizeof(s_files_select), 1);
		if (files == NULL) {
			return (NULL);
		}
		files->file_name = strdup(name);
	} else {
		// go to last node
		while (files->next) {
			files = files->next;
		}

		// Create new node
		files->next = calloc(sizeof(s_files_select), 1);
		if (files->next == NULL) {
			return (NULL);
		}
		files->next->file_name = strdup(name);
		files = ptr;
	}

	return (files);
}
