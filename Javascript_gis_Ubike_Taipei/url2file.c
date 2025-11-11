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

#define CHUNK_SIZE 1024
#define MAX_BLOCKS 10
typedef struct
{
    size_t blockSize;
    size_t currentLen;
    int insideBlock;
    int blockCount;

} numStruct;
void exam_a_char(char c, numStruct *num , char **currentBlockIn ,  char *blocks[] )
{

    char * currentBlock = currentBlockIn [0] ;

    if (c == '{')
    {
        num->insideBlock = 1;
        num->blockSize = 100;
        currentBlock = malloc(num->blockSize);
        num->currentLen = 0;
    }
    else if (c == '}' && num->insideBlock)
    {
        num->insideBlock = 0;

        // null-terminate
        if (num->currentLen + 1 > num->blockSize)
        {
            currentBlock = realloc(currentBlock, num->currentLen + 1);
        }
        currentBlock[num->currentLen] = '\0';

        // store block safely
        if (num->blockCount < MAX_BLOCKS)
        {
            blocks[(num->blockCount)++] = currentBlock;
            printf("blockCount = %d\n", num->blockCount);
        }
        else
        {
            printf("blockCount = %d\n", num->blockCount);
            free(currentBlock);
        }

        currentBlock = NULL;
        num->currentLen = 0;
    }
    else if (num->insideBlock)
    {
        // append char
        if (num->currentLen + 1 >= num->blockSize)
        {
            num->blockSize += 100;
            currentBlock = realloc(currentBlock, num->blockSize);
        }
        currentBlock[(num->currentLen)++] = c;
    }

    * currentBlockIn = currentBlock ; 
    
}
void print_block( numStruct * num , char * blocks[] ){
    unsigned int blockCount = num->blockCount ;
    printf("Total blocks found: %d\n", blockCount);
    for (unsigned int i = 0; i < blockCount; i++)
    {
        printf("Block %d:\n%s\n\n", i , blocks[i]);
        free(blocks[i]);
    }
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
    char *currentBlock = NULL;

    numStruct num ;
    num.blockCount = 0 ;
    num.blockSize = 0 ;
    num.currentLen = 0 ;
    num.insideBlock = 0 ;

    while ((bytesRead = fread(chunk, 1, CHUNK_SIZE, fp)) > 0)
    {
        chunk[bytesRead] = '\0';

        for (size_t i = 0; i < bytesRead; i++)
        {
            char c = chunk[i];
            if ( num.blockCount < MAX_BLOCKS ) { 
                exam_a_char ( c , &num , &currentBlock , blocks ) ;
            }
        }
    }

    fclose(fp);

    print_block( &num , blocks ) ;

    return EXIT_SUCCESS;
}

