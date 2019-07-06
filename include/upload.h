#ifndef UPLOAD_H
#define UPLOAD_H

#define REQUEST_SIZE	102

typedef struct	upload_file
{
	char		*data;
	size_t		size;
}				upload_file;

char	*upload_files(t_list_files *list);

#endif
