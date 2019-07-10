#include "common.h"

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
