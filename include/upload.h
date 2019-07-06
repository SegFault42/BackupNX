#ifndef UPLOAD_H
#define UPLOAD_H

typedef struct	upload_file
{
	char		*data;
	size_t		size;
}				upload_file;

char	*upload_files(t_list_files *list);

#endif
