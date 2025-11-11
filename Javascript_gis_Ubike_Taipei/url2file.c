/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/
/* <DESC>
 * Download a given URL into a local file named page.out.
 * </DESC>
 */
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

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
#define MAX_BLOCKS 100
typedef struct
{
    size_t blockSize;
    size_t currentLen;
    int insideBlock;
    int blockCount;

} numStruct;

char exam_a_char(char c, numStruct *num)
{

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
}

int main(int argc, char *argv[])
{
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
    int blockCount = 0;

    char chunk[CHUNK_SIZE + 1];
    size_t bytesRead;

    char *currentBlock = NULL;
    size_t blockSize = 0;
    size_t currentLen = 0;
    int insideBlock = 0;

    while ((bytesRead = fread(chunk, 1, CHUNK_SIZE, fp)) > 0)
    {
        chunk[bytesRead] = '\0';

        for (size_t i = 0; i < bytesRead; i++)
        {
            char c = chunk[i];

            if (c == '{')
            {
                insideBlock = 1;
                blockSize = 100;
                currentBlock = malloc(blockSize);
                currentLen = 0;
            }
            else if (c == '}' && insideBlock)
            {
                insideBlock = 0;

                // null-terminate
                if (currentLen + 1 > blockSize)
                {
                    currentBlock = realloc(currentBlock, currentLen + 1);
                }
                currentBlock[currentLen] = '\0';

                // store block safely
                if (blockCount < MAX_BLOCKS)
                {
                    blocks[blockCount++] = currentBlock;
                    printf("blockCount = %d\n", blockCount);
                }
                else
                {
                    printf("blockCount = %d\n", blockCount);
                    free(currentBlock);
                    break;
                }

                currentBlock = NULL;
                currentLen = 0;
            }
            else if (insideBlock)
            {
                // append char
                if (currentLen + 1 >= blockSize)
                {
                    blockSize += 100;
                    currentBlock = realloc(currentBlock, blockSize);
                }
                currentBlock[currentLen++] = c;
            }
        }
    }

    fclose(fp);

    // Print blocks
    /*printf("Total blocks found: %d\n", blockCount);
    for (int i = 0; i < blockCount; i++)
    {
        printf("Block %d:\n%s\n\n", i + 1, blocks[i]);
        free(blocks[i]);
    }*/

    return EXIT_SUCCESS;
}

