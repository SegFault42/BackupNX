#include "common.h"

t_cred	*cred = NULL;

static char	*map_file(char *path)
{
	char		*file = NULL;
	struct stat	st = {0};
	int			fd = 0;

	fd = open(path, O_RDONLY);
	if (fd == -1) {
		return (NULL);
	}

	if (fstat(fd, &st) == -1) {
		close(fd);
		return (NULL);
	}

	file = (char *)calloc(sizeof(char), st.st_size + 1);
	if (file == NULL) {
		close(fd);
		return (NULL);
	}

	if (read(fd, file, st.st_size) == -1) {
		free(file);
		file = NULL;
		close(fd);
		return (NULL);
	}

	close(fd);

	return (file);
}

bool	parse_json(char *path)
{
	char		*data = NULL;
	cJSON		*json = NULL;
	cJSON		*token = NULL;
	cJSON		*files = NULL;
	cJSON		*file = NULL;

	data = map_file(path);
	if (data == NULL) {
		return (false);
	}

	cred = (t_cred *)calloc(sizeof(t_cred), 1);
	if (cred == NULL) {
		free(data);
		data = NULL;
		return (false);
	}

	json = cJSON_Parse(data);
	if (json == NULL) {
		free(data);
		data = NULL;
		free(cred);
		cred = NULL;
		return (false);
	}

	token = cJSON_GetObjectItemCaseSensitive(json, "key");
	if (!cJSON_IsString(token) && (token->valuestring == NULL)) {
		free(data);
		data = NULL;
		free(cred);
		cred = NULL;
		return (false);
	} else {
		cred->api_key = strdup(token->valuestring);
	}

	files = cJSON_GetObjectItemCaseSensitive(file, "files");
	cJSON_ArrayForEach(file, files) {
		printf("%s\n", files->valuestring);
	}

	cJSON_Delete(json);
	free(file);
	file = NULL;

	return (true);
}

// Init console, socket and struct with path point to sdmc:/
t_files	*init(void)
{
	t_files	*ptr = NULL;

	consoleInit(NULL);
	
	if (isFileExist("/switch/BackupNX") == false) {
		mkdir("/switch/BackupNX", 0777);
	}
	if (isFileExist("/switch/BackupNX/cred.json") == false) {
		printf("%s/switch/BackupNX/cred.json missing !%s", CONSOLE_RED, CONSOLE_WHITE);
		consoleUpdate(NULL);
		sleep(5);
		consoleExit(NULL);
		return (NULL);
	}

	socketInitializeDefault();

	if (parse_json("/switch/BackupNX/cred.json") == false) {
		printf("%sfailed to parse cred.json%s", CONSOLE_RED, CONSOLE_WHITE);
		return (NULL);
	}

	ptr = calloc(sizeof(t_files), 1);
	if (!ptr)
		{ return (NULL); }

	ptr->path[0] = '/';

	return (ptr);
}

void	de_init(t_files *s_files)
{
	consoleExit(NULL);
	socketExit();
	free_list(s_files->files);
}
