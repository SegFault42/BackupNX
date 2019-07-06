#include "common.h"

static void	upload(char *file)
{
	CURL				*curl;
	CURLcode			res;
	struct curl_slist	*chunk = NULL;
	char				*data = "Hello\n";

	  curl = curl_easy_init();

	  if(curl) {

		// append all header
		chunk = curl_slist_append(chunk, "Authorization: Bearer <TOKEN_HERE>");
		chunk = curl_slist_append(chunk, "Dropbox-API-Arg: {\"path\": \"/filename\",\"mode\": \"add\",\"autorename\": true,\"mute\": false,\"strict_conflict\": false}");
		chunk = curl_slist_append(chunk, "Content-Type: application/octet-stream");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);


		// data to send (set size and content)
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 6L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

		curl_easy_setopt(curl, CURLOPT_URL, "https://content.dropboxapi.com/2/files/upload");

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 			// skipping cert. verification, if needed
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); 			// skipping hostname verification, if needed

		//[> Perform the request, res will get the return code <]
		res = curl_easy_perform(curl);
		//[> Check for errors <]
		if(res != CURLE_OK)
			  fprintf(stderr, "curl_easy_perform() failed: %s\n",
					  curl_easy_strerror(res));

		//[> always cleanup <]
		curl_easy_cleanup(curl);
	  }
	  //[>curl_global_cleanup();<]
}

void	upload_files(t_list_files *list)
{
	/*for (int i = 0; list->files[i] != NULL; i++) {*/
		/*printf("[%s] - [%s]\n", list->directory, list->files[i]);*/
	/*}*/
	print_2d_array(list->files);
}

