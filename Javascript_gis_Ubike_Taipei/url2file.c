#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <locale.h>

static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}
int donwloadFileFromURL(char *fName, char *urlAddress)
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

#define CHUNK_SIZE 1024
#define MAX_BLOCKS 5000
#define STR_SIZE 512

char ** get_formatList ( char ** str0 , const unsigned int str0Len ) {

    const char * FORMAT = ": \"%[^\"]\"" ;
    const unsigned int FORMATLEN = strlen ( FORMAT ) ;
    char ** formatList = malloc ( sizeof (char*) * str0Len ) ;

    for ( unsigned int i = 0 ; i < str0Len ; i ++ ) {
        formatList [ i ] = malloc (  sizeof ( char ) * ( FORMATLEN + strlen ( str0 [ i ] ) ) ) ;
        sprintf ( formatList [i] , "%s%s" , str0 [i] , FORMAT ) ;
        printf ( "format[%i]: %s\n" , i , formatList[i] ) ;
    }

    return formatList ;
}
void free_formatList ( char ** x , const unsigned int xSize ) {

    for ( unsigned int i = 0 ; i < xSize ; i++ ) { free ( x [ i ] ) ; }
    free ( x ) ;
}

unsigned int * get_str0SizeList ( char ** str0 , const unsigned int str0Len ) {

    unsigned int * str0Size =  calloc ( str0Len, sizeof ( unsigned int ) ) ;

    for ( unsigned int i = 0 ; i < str0Len ; i++ ) {
        str0Size [ i ] =  strlen ( str0 [ i ] ) ;
    }

    for ( unsigned int i = 0 ; i < str0Len ; i++ ) {
        printf ( "str [%i] = %s, %i\n", i, str0[i] , str0Size [ i ] ) ;
    }

    return str0Size ;
}

char ** get_out( const unsigned int outSize ){
 return ( malloc ( sizeof( char * ) * outSize ) ) ;
}
char * get_value( char * cPtr , char * format ){

    char * newStr = malloc( STR_SIZE * sizeof(char) );

    sscanf ( cPtr , format , newStr  );

    const unsigned int intTmp = strlen ( newStr ) ;

    if ( intTmp+1 < STR_SIZE ) {
        newStr = realloc ( newStr, sizeof ( char ) * (intTmp+1) ) ;
    }

    return newStr ;
}

char ** check_outSize( char ** out , unsigned int  * outSize , const unsigned int blockCount ){

    if ( *outSize == blockCount ) {
        *outSize += 100 ;
        out = realloc( out, sizeof(char*) * (*outSize) ) ;
    }

    return out ;
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
    FILE *fp = fopen(fName, "r");
    if (fp == NULL)
    {
        printf("Error: Unable to open the file, %s.\n", fName);
        return 1;
    }

    char *blocks[MAX_BLOCKS];
    char chunk[CHUNK_SIZE + 1];
    size_t bytesRead;

    char *cPtr  ;
    int insideBlock = 0 ;

    char *str0[] = {"\"sna\"","\"sareaen\"" };
    const unsigned int str0Len = 2 ;

    unsigned int * str0Size   = get_str0SizeList ( str0 , str0Len ) ;
           char ** formatList = get_formatList   ( str0 , str0Len ) ;

    //unsigned int outSize = 100 ;
    //char ** out = get_out ( outSize ) ;
    char *** outList = malloc ( sizeof( char ** ) * str0Len ) ;
    unsigned int * outSize = malloc ( sizeof ( unsigned int ) * str0Len ) ;
    for ( unsigned int i = 0 ; i < str0Len ; i ++ ) {
        outSize [i] = 100 ;
        outList [ i ]= get_out ( outSize [ i ] ) ;
    }


    char *str0Ptr =NULL ;
    unsigned int intTmp = 0 ;
    unsigned int blockCount = 0 ;
    while ((bytesRead = fread(chunk, 1, CHUNK_SIZE, fp)) > 0)
    {
        chunk[bytesRead] = '\0';

        cPtr = chunk ;
        for (size_t i = 0; i < bytesRead; i++, cPtr++ ) {

            if       ( *cPtr == '{' ){   insideBlock++; blockCount++;
            }else if ( *cPtr == '}' ){   insideBlock--;
            }else {
                
                for ( unsigned int iStr = 0 ; iStr < str0Len ; iStr++ ) {
                    
                    if ( strncmp ( cPtr , str0 [ iStr] , str0Size[iStr]) ) { continue; }

                    outList [ iStr ] = check_outSize ( outList[ iStr ] , outSize+iStr , blockCount ) ;

                    outList[ iStr ][ blockCount ] = get_value( cPtr , formatList [ iStr ] );
                    //printf ( "get :: %s:%s\n", str0 [ iStr] , outList [ iStr ] [ blockCount ]  );

                }        
            }


        }

    }

    if ( insideBlock != 0 ) {
        fprintf ( stderr, "the JSON file is broken.\n") ;
    }

    for ( unsigned int i = 0 ; i < blockCount ; i++ ) {
        printf ( "[%i] [", i ) ;
        for ( unsigned int j = 0 ; j < str0Len ; j++ )  {
            printf ( "%s\t ", outList[j][i] ) ;
        }puts("]");
    }

   // fclose(fp);
   // free ( str0Size ) ;

    int i = 0 ;


   //free( outSize );


    return EXIT_SUCCESS;
}

