#include <string.h>
#include <stdio.h>

#include <switch.h>

#include <curl/curl.h>
#include <sys/socket.h>
#include <dirent.h>

void	upload()
{
	CURL				*curl;
	CURLcode			res;
	struct curl_slist	*chunk = NULL;
	char				*data = "Hello\n";

  	curl = curl_easy_init();

  	if(curl) {

		// append all header
		chunk = curl_slist_append(chunk, "Authorization: Bearer _G3bZFuCKiAAAAAAAAAADbubss3eXFcWBIJb5awT7_zNVKUuh68WHN_G8BHsPbpc");
		chunk = curl_slist_append(chunk, "Dropbox-API-Arg: {\"path\": \"/filename\",\"mode\": \"add\",\"autorename\": true,\"mute\": false,\"strict_conflict\": false}");
		chunk = curl_slist_append(chunk, "Content-Type: application/octet-stream");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);


		// data to send (set size and content)
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 6L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

		curl_easy_setopt(curl, CURLOPT_URL, "https://content.dropboxapi.com/2/files/upload");

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 			// skipping cert. verification, if needed
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); 			// skipping hostname verification, if needed

    	/* Perform the request, res will get the return code */
    	res = curl_easy_perform(curl);
    	/* Check for errors */
    	if(res != CURLE_OK)
      		fprintf(stderr, "curl_easy_perform() failed: %s\n",
              		curl_easy_strerror(res));

    	/* always cleanup */
    	curl_easy_cleanup(curl);
  	}
      /*curl_global_cleanup();*/
}

void	listDirectory(const char *path)
{
	DIR		*dirp = NULL;
	struct dirent	*dirent = NULL;

	dirp = opendir(path);
	if (dirp != NULL) {
		while ((dirent = readdir(dirp)) != NULL) {
			printf("%s\n", dirent->d_name);
		}
	}
}

int main(int argc, char **argv)
{
	consoleInit(NULL);
	socketInitializeDefault();

	while(appletMainLoop()) {
		hidScanInput();

		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (kDown & KEY_PLUS) {
			break; // break in order to return to hbmenu
		}
		if (kDown & KEY_A) {
			upload();
		}
		if (kDown & KEY_B) {
			listDirectory("/switch");
		}
		consoleUpdate(NULL);
	}

	consoleExit(NULL);
	socketExit();

	return 0;
}
