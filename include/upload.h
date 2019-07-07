#ifndef UPLOAD_H
#define UPLOAD_H

#include "cJSON.h"

#define REQUEST_AUTH_SIZE	22
#define REQUEST_API_SIZE	102

typedef struct	upload_file
{
	char		*data;
	size_t		size;
}				upload_file;

char	*upload_files(t_list_files *list);
void	upload(char *file);

#endif
