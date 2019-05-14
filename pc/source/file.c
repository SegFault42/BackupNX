#include "common.h"

	/*s_files_select *first = files;*/

	/*int idx = 0;*/
	/*while (1) {*/
		/*consoleClear();*/
		/*printf("\x1b[0;0H");*/
		/*hidScanInput();*/
		/*u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);*/

		/*for (int i = 0; files; i++) {*/
			/*if (i == idx) {*/
				/*printf("> ");*/
			/*} else {*/
				/*printf("  ");*/
			/*}*/
			/*if (files->select == true) {*/
				/*printf("[X] ");*/
			/*} else {*/
				/*printf("[ ] ");*/
			/*}*/
			/*printf("%s\n", files->file_name);*/
			/*files = files->next;*/
		/*}*/

		/*if (files == NULL) { files = first;}*/

		/*if (kDown & KEY_DOWN) {*/
			/*idx++;*/
			/*if (idx == 4) {*/
				/*idx = 0;*/
			/*}*/
		/*}*/
		/*if (kDown & KEY_A) {*/
			/*for (int i = 0; i < idx; i++) {*/
				/*files = files->next;*/
			/*}*/
			/*// flip bool*/
			/*files->select = !files->select;*/
		/*}*/
		/*consoleUpdate(NULL);*/
	/*}*/

s_files_select	*getFilesList(const char *path)
{
	s_files_select	*new_node = NULL;
	s_files_select	*begin = NULL;
	DIR				*dirp = NULL;
	struct dirent	*dirent = NULL;

	// open directory
	dirp = opendir(path);
	if (dirp == NULL) {
		return (NULL);
	}

	// store all filename in linked list
	while ((dirent = readdir(dirp)) != NULL) {
		new_node = add_tails(begin);
		if (new_node) {
			if (begin == NULL)
				{ begin = new_node; }
			new_node->file_name = strdup(dirent->d_name);
		}
	}

	closedir(dirp);
	/*s_files_select *first = begin;*/

	/*int idx = 0;*/
	/*while (1) {*/
		/*consoleClear();*/
		/*printf("\x1b[0;0H%s\n", begin->file_name);*/
		/*hidScanInput();*/
		/*u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);*/

		/*for (int i = 0; begin; i++) {*/
			/*if (i == idx) {*/
				/*printf("> ");*/
			/*} else {*/
				/*printf("  ");*/
			/*}*/
			/*if (begin->select == true) {*/
				/*printf("[X] ");*/
			/*} else {*/
				/*printf("[ ] ");*/
			/*}*/
			/*printf("%s\n", begin->file_name);*/
			/*begin = begin->next;*/
		/*}*/

		/*if (begin == NULL) { begin = first;}*/

		/*if (kDown & KEY_DOWN) {*/
			/*idx++;*/
			/*if (idx == 4) {*/
				/*idx = 0;*/
			/*}*/
		/*}*/
		/*if (kDown & KEY_A) {*/
			/*for (int i = 0; i < idx; i++) {*/
				/*begin = begin->next;*/
			/*}*/
			/*// flip bool*/
			/*begin->select = !begin->select;*/
		/*}*/
		/*consoleUpdate(NULL);*/
	/*}*/
	return (begin);
}
