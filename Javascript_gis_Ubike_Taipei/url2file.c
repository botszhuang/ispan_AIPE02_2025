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


int main(int argc, char *argv[])
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

    
    struct jsonObjectItemStruct{
        char * key ;
        cJSON * ObjectItem ;
    };
    
   
    const char * target [] = { "sna" , "ar" , "latitude" , "longitude" } ;
    const unsigned int jSize = sizeof(target) / sizeof(char*) ;
    struct jsonObjectItemStruct * j = calloc (  jSize , sizeof ( struct jsonObjectItemStruct ) ) ;
    for ( unsigned int i = 0 ; i < jSize ; i ++ ) {
        j [ i ].key = ( char * ) target [ i ] ;
    }
    cJSON *element = root->child;
    for ( unsigned int i = 0 ; element != NULL; element = element->next , i++){
        printf ( "[ %i ]", i ) ;
        for ( unsigned int k = 0 ; k < jSize ; k++ ) {
            j[k].ObjectItem = cJSON_GetObjectItemCaseSensitive( element , j[k].key );
            if (cJSON_IsString( j[k].ObjectItem ) && (j[k].ObjectItem)->valuestring != NULL ) {
                printf("| %s",(j[k].ObjectItem)->valuestring);
            }       
        }puts("");    
        

    }
    fflush( stdout );
    free ( j );
    cJSON_Delete(root); 
    return EXIT_SUCCESS;
}

