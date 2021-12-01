#include <stdio.h> // maloc..
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h> // boolean C
#include <stdlib.h>
#include <string.h> // string C
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h> // SDL2
#include "SDL2/SDL_image.h" // SDL2_Image
#include "SDL2/SDL_ttf.h" // SDL2_Font
#include <enet/enet.h> // Lib ENET / UDP
#include <curl/curl.h> // Lib CURL / Request HTTP.
#include "SDL2/SDL_mixer.h"
#include <iostream> // C++
#include <vector> // tableau dynamique - C++
using namespace std; // C++

struct memory {
   char *response;
   size_t size;
 };

 struct memory resultResponse;
 
 static size_t cb(void *data, size_t size, size_t nmemb, void *userp)
 {
   size_t realsize = size * nmemb;
   struct memory *mem = (struct memory *)userp;
 
   char *ptr = (char*)realloc(mem->response, mem->size + realsize + 1);

   if(ptr == NULL)
   {
	   return 0;  /* out of memory! */
   }

   mem->response = ptr;
   memcpy(&(mem->response[mem->size]), data, realsize);
   mem->size += realsize;
   mem->response[mem->size] = 0;
 
   return realsize;
}

/*void requestHTTP_POST()
{
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_URL, "https://crz-gamestudio.com/jeu/keyAccess.php?keyAccessUser=grjidfnjikhf33203dsfhjWSSJEZN");
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
	struct curl_slist *headers = NULL;
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_mime *mime;
	curl_mimepart *part;
	mime = curl_mime_init(curl);
	part = curl_mime_addpart(mime);
	curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb); // Ecrit les données
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resultResponse); // Callback qui récupère les données.
	res = curl_easy_perform(curl); // Code HTTP (0 si OK)

	if(res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform () a échoué : %s\n", curl_easy_strerror (res));
	}
	else
	{
		printf("Request HTTP OK \n");
		printf("Response back-end : %s \n", resultResponse); // Response back-end.
	}
			
	curl_mime_free(mime);
	curl_easy_cleanup(curl);
}*/
