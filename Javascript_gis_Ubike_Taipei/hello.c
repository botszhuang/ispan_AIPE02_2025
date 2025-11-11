// emcc hello.c -s WASM=1 -s SINGLE_FILE=1 -s EXPORTED_FUNCTIONS="['_say_hello']" -o hello.js
/*****************************************************
1) emcc hello.c -o hello.js   -s EXPORTED_FUNCTIONS='["_say_hello","_greet", "_add_ten"]'     -s EXPORTED_RUNTIME_METHODS='["cwrap"]'
2) $ python3 -m http.server 8000
   Then open: http://localhost:8000/index.html
3) NOTE:
Don’t <script src="hello.wasm"></script> → it’s binary, not JS.
Include hello.js only.
Access via http://localhost or another local server.
******************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

void say_hello()
{
    printf("Hello World from C!\n");
}

void greet(const char *name)
{
    printf("Hello, %s!\n", name);
}

int add_ten(int arg)
{
    return arg + 10;
}

// Callback function to write received data to a file
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void downoadFile()
{

    CURL *curl;
    FILE *fp;
    CURLcode res;
    const char *url = "http://example.com/path/to/your/file.txt"; // Replace with your URL
    const char *outfilename = "downloaded_file.txt";              // Replace with your desired filename

    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(outfilename, "wb");
        if (fp == NULL)
        {
            fprintf(stderr, "Error opening file %s\n", outfilename);
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        fclose(fp);
        curl_easy_cleanup(curl);
    }
    return EXIT_SUCCESS;
}
