#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <string.h>
#include <locale.h>
//#include <cjson/cJSON.h>
#include "cJSON.h"
// gcc cJSON.c url2file.c -o ur2file.o  -lcurl


static size_t Write_to_a_string ( void * context , size_t size, size_t numb, void *stream ){
    size_t newLength = size * numb ;
    memcpy (  stream , context  ) ;
    return newLength ;
}
int donwloadFileFromURL(const char *fName, char *urlAddress)
{
    // const char *fName = "Ubike.json";
    // char *urlAddress = "https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json";

    CURLcode res;
    CURL *curl;

    if (fName == NULL || urlAddress == NULL){
        fprintf( stderr, "NO <URL>\n");
        return EXIT_FAILURE ;
    }

    /* init the curl session */
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (res){   
        fprintf(stderr, "Could not init curl\n");   
        return (int)res;
    }
 
    curl = curl_easy_init();
    if (!curl){
        curl_global_cleanup();
        return EXIT_FAILURE;
    }

    /* set URL */
    curl_easy_setopt(curl, CURLOPT_URL, urlAddress);

    /* Switch on full protocol/debug output while testing */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /* disable progress meter, set to 0L to enable it */
    // curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    /* enable progress meter, set to 0L to enable it */
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);

    /* write the page body to this file handle */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    /* get it! */
    res = curl_easy_perform(curl);

    /* close the header file */
    fclose(fp);

    /* cleanup curl stuff */
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return (int)res;
}
int main(int argc, char *argv[])

    setlocale(LC_ALL, ".UTF-8");

    int res = 0;

    const char *fName = "Ubike.json";
    char *urlAddress = "https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json";

    /*    res = donwloadFileFromURL(fName, urlAddress);
        if (res == 1)
        {
            return EXIT_FAILURE;
        }
    */
    // open the file
   
    return EXIT_SUCCESS;
}
