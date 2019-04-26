#include "common.h"

int main(void)
{
	s_files_select	*files = NULL;

	consoleInit(NULL);
	socketInitializeDefault();

	while(appletMainLoop()) {
		hidScanInput();

		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (kDown & KEY_PLUS) {
			break; // break in order to return to hbmenu
		}
		if (kDown & KEY_A) {
			/*upload();*/
		}
		files = getFilesList("/");
		chooseFileToUpload(files);
		consoleUpdate(NULL);
	}

	consoleExit(NULL);
	socketExit();

	return 0;
}
