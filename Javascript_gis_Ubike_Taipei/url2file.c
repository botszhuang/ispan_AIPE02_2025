#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <locale.h>
//#include <cjson/cJSON.h>
#include "cJSON.h"
// gcc cJSON.c url2file.c -o ur2file.o  -lcurl


static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}
int donwloadFileFromURL(const char *fName, char *urlAddress)
{
    // const char *fName = "Ubike.json";
    // char *urlAddress = "https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json";

    CURLcode res;
    CURL *curl;

    if (fName == NULL || urlAddress == NULL)
    {
        printf("NO <URL>\n");
        return 1;
    }

    res = curl_global_init(CURL_GLOBAL_ALL);
    if (res)
    {
        fprintf(stderr, "Could not init curl\n");
        return (int)res;
    }

    /* init the curl session */
    curl = curl_easy_init();
    if (!curl)
    {
        curl_global_cleanup();
        return EXIT_FAILURE;
    }

    /* open the file */
    FILE *fp = fopen(fName, "wb");
    if (!fp)
    {
        fprintf(stdout, "I cannot open %s.\n", fName);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return (int)res;
    }

    /* set URL to get here */
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
char *read_file(const char *fName) {
    
    FILE *fp = NULL;
    long fp_length = 0;


    fp = fopen(fName, "r"); // Open file in binary mode for cross-platform compatibility
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s\n", fName );
        return NULL;
    }
    
    fseek ( fp, 0 , SEEK_END ) ;
    fp_length = ftell ( fp ) ;
    fseek ( fp, 0 , SEEK_SET ) ;

    char *content =  malloc ( fp_length + sizeof ('\0') ) ;
    if ( content == NULL ) {
        fprintf ( stderr , "Memory allocation errer!\n" ) ;
        fclose ( fp ) ;
        return NULL ;
    }

    size_t readbytes = fread ( content, 1, ( size_t) fp_length , fp ) ;
    if ( readbytes != (size_t) fp_length) {
        fprintf ( stderr , "Unable to reading file %s!\n", fName ) ;
        free ( content ) ;
        fclose ( fp ) ;
        return NULL ;
    }

    content [ fp_length ] = '\0' ;
    fclose ( fp ) ;
    return content ;
}

//int main(int argc, char *argv[])
int main()
{
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
<<<<<<< HEAD

        if (!json_data) {
        fprintf(stderr, "Failed to download JSON\n");
        return EXIT_FAILURE;
    }

    // Parse JSON
    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        fprintf(stderr, "JSON parse error: %s\n", cJSON_GetErrorPtr());
        free(json_data);
        return EXIT_FAILURE;
    }

    if (!cJSON_IsArray(root)) {
        fprintf(stderr, "Expected top-level JSON array\n");
        cJSON_Delete(root);
        free(json_data);
        return EXIT_FAILURE;
    }

    // Loop through each station object
    int station_count = cJSON_GetArraySize(root);
    printf("Total stations: %d\n\n", station_count);

    for (int i = 0; i < station_count; i++) {
        cJSON *station = cJSON_GetArrayItem(root, i);
        if (!cJSON_IsObject(station)) continue;

        cJSON *sna = cJSON_GetObjectItem(station, "sna");
        cJSON *sareaen = cJSON_GetObjectItem(station, "sareaen");

        const char *sna_str = cJSON_IsString(sna) ? sna->valuestring : "(null)";
        const char *sareaen_str = cJSON_IsString(sareaen) ? sareaen->valuestring : "(null)";

        printf("[%3d] %-30s | %s\n", i + 1, sna_str, sareaen_str);
    }

    // Cleanup
    cJSON_Delete(root);
    free(json_data);


=======
    char * jsonTxt = read_file ( fName ) ;
    if ( jsonTxt == NULL ) { return EXIT_FAILURE ; }

    cJSON * root = cJSON_Parse( jsonTxt ) ;
    free ( jsonTxt ) ;

    if ( root == NULL ) {
        const char * errPtr= cJSON_GetErrorPtr();
        if ( errPtr != NULL ) {
            fprintf ( stderr , "Error: %s\n", errPtr ) ;
        }
        return EXIT_FAILURE ;
    }
    /* --- Process the JSON data here --- */
    printf("Successfully parsed JSON file.\n");
 
    const char * target [] = { "sna" , "ar" , "latitude" , "longitude" } ;

    const unsigned int jSize = sizeof(target) / sizeof(char*) ;
    printf ( " jSzie = %i\n", jSize ) ;
 
    cJSON *element = root->child;
    cJSON *item = NULL ;
    for ( unsigned int i = 0 ; element != NULL; element = element->next , i++){
        printf ( "[ % 3i ]", i ) ;
        for ( unsigned int k = 0 ; k < jSize ; k++ ) {
            item = cJSON_GetObjectItemCaseSensitive( element , target [ k ] );
            if ( item != NULL ) {
                       if ( cJSON_IsString (item) ) { printf("| %s" , item->valuestring );
                } else if ( cJSON_IsNumber (item) ) { printf("| %lf", item->valuedouble );
                }
            }       
        }puts("");    
        

    }
    fflush( stdout );
    cJSON_Delete(root); 
>>>>>>> refs/remotes/origin/main
    return EXIT_SUCCESS;
}

