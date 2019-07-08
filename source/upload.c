#include "common.h"

#include <math.h>

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

static char	*get_token(void)
{
	int		fd = 0;
	struct stat	st;
	char	*json_file = NULL;
	cJSON	*token = NULL;
	cJSON *json = NULL;

	fd = open("/switch/BackupNX/cred.json", O_RDONLY);
	if (fd == -1) {
		return (NULL);
	}

	if (fstat(fd, &st) == -1) {
		return (NULL);
	}

	json_file = (char *)calloc(sizeof(char), st.st_size + 1);
	if (json_file == NULL) {
		return (NULL);
	}

	if (read(fd, json_file, st.st_size) == -1) {
		free(json_file);
		json_file = NULL;
		return (NULL);
	}

	close(fd);

	json = cJSON_Parse(json_file);
	if (json == NULL) {
		free(json_file);
		json_file = NULL;
		return (NULL);
	}

	token = cJSON_GetObjectItemCaseSensitive(json, "key");
	if (!cJSON_IsString(token) && (token->valuestring == NULL)) {
		return (NULL);
	}

	/*cJSON_Delete(json);*/
	free(json_file);
	json_file = NULL;

	return (token->valuestring);
}

static void	format_dropbox_request(char *file, struct curl_slist **chunk)
{
	char	*authorization = NULL, *api_arg = NULL;
	char	*token = NULL;

	token = get_token();
	if (token == NULL) {
		printf("%s/switch/BackupNX/cred.json corrupted%s", CONSOLE_RED, CONSOLE_RESET);
		consoleUpdate(NULL);
		sleep(5);
		return ;
	}

	authorization = (char *)calloc(sizeof(char *), strlen(token) + REQUEST_AUTH_SIZE + 1);
	if (authorization == NULL) {
		return ;
	}

	strcat(authorization, "Authorization: Bearer ");
	strcat(authorization, token);

	*chunk = curl_slist_append(*chunk, authorization);
	free(authorization);
	authorization = NULL;

	api_arg = (char *)calloc(sizeof(char), strlen(file) + REQUEST_API_SIZE + 1);
	if (api_arg == NULL) {
		return ;
	}

	strcat(api_arg, "Dropbox-API-Arg: {\"path\": \"");
	strcat(api_arg, file);
	strcat(api_arg, "\",\"mode\": \"overwrite\",\"autorename\": true,\"mute\": false,\"strict_conflict\": false}");

	*chunk = curl_slist_append(*chunk, api_arg);
	*chunk = curl_slist_append(*chunk, "Content-Type: application/octet-stream");

	free(api_arg);
	api_arg = NULL;
}

void	upload(char *file)
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
		format_dropbox_request(file, &chunk);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		// data to send (set size and content)
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, up->size);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, up->data);

		/*curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);*/

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
		curl_slist_free_all(chunk);
	}

	free(up->data);
	up->data = NULL;
	free(up);
	up = NULL;
}

char	*upload_files(t_list_files *list)
{
	struct stat	st = {0};
	char		*file = NULL;

	for (int i = 0; list->files[i] != NULL; i++) {
		file = (char *)calloc(sizeof(char), strlen(list->directory) + strlen(list->files[i]) + 1);
		if (file == NULL) {
			return (NULL);
		}

		strcat(file, list->directory);
		strcat(file, list->files[i]);
		stat(file, &st);

		if (S_ISDIR(st.st_mode)) {
			listdir(file);
		} else if (S_ISREG(st.st_mode)) {
			upload(file);
		}

		free(file);
		file = NULL;
	}

	return (NULL);
}

