#ifndef FILE_H
#define FILE_H

#include <dirent.h>
#include "linked_list.h"

s_files_select	*getFilesList(const char *path);
void	chooseFileToUpload(files);

#endif
