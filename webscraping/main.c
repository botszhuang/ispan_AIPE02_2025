#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <string.h>
#include <locale.h>

// gcc main.c -o main.o  -lcurl

typedef struct {
   char * data ;
   unsigned int size ;
}streamStruct;

static size_t Write_to_a_string ( void * context , size_t size, size_t numb, void *stream ){ 
    size_t Length = size * numb ; 
    streamStruct * a = stream ;

    char * ptr = realloc ( a->data , a->size + Length + 1 ) ;
    if ( !ptr ) { return 0 ; }

    a->data = ptr ;
    memcpy ( a->data + ( a->size ) , context , Length ) ;
    a->size += Length ;
    a->data [ a->size ] = 0 ;
    return Length ;
}
int curl_get(  streamStruct * buffer , char *urlAddress ){

    if ( !urlAddress ){
        fprintf( stderr, "NO <URL>\n");
        return EXIT_FAILURE ;
    }

    /* init the curl session */
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res){   
        fprintf(stderr, "Could not init curl\n");   
        return (int)res;
    }
  
    CURL * curl = curl_easy_init();
    if (!curl){
        curl_global_cleanup();
        return EXIT_FAILURE;
    }

    /* set URL */
    curl_easy_setopt(curl, CURLOPT_URL, urlAddress);

    /* Switch on full protocol/debug output while testing */
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /* enable progress meter, set to 0L to enable it */
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Write_to_a_string );
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer );
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    res = curl_easy_perform(curl);
    fprintf ( stdout , "request status: %i\n", res ) ;

    /* cleanup curl stuff */
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return (int)res;
}
int main(int argc, char *argv[]){

    setlocale(LC_ALL, ".UTF-8");

    int res = 0;

    streamStruct * buffer = malloc ( sizeof ( streamStruct ) ) ;
    buffer[0].data = NULL ; 
    buffer[0].size = 0 ;

    char *urlAddress = "https://store.line.me/stickershop/product/36044/en" ;
    res = curl_get ( buffer , urlAddress );
    if (res == 1) { return EXIT_FAILURE; }


    
    htmlDocPtr doc = htmlReadMemory(
        buffer->data,         // your HTML text
        buffer->size,         // length of HTML
        NULL,               // base URL (optional)
        NULL,               // encoding (NULL = auto)
        HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING
    );

    //puts( buffer->data ) ;
    free( buffer->data ) ;
    free( buffer ) ;

    if (!doc) {
        fprintf(stdout ,  "Parse fail!\n");
        return  EXIT_FAILURE ;
    }
    
    xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
    xmlXPathObjectPtr r = xmlXPathEvalExpression((xmlChar*)"//h1", ctx);

    printf("found: %d h1\n", r && r->nodesetval ? r->nodesetval->nodeNr : 0);

    if (r && r->nodesetval && r->nodesetval->nodeNr > 0) {
        for ( unsigned int i = 0 ; i < r->nodesetval->nodeNr ; i++ ) {
            xmlNodePtr n = r->nodesetval->nodeTab[i];
            xmlChar *text = xmlNodeGetContent(n);
            printf("TEXT: %s\n", text);
            xmlFree(text);
        }
    }
    xmlXPathFreeObject(r);  
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
    xmlCleanupParser();
  


    
   
    return EXIT_SUCCESS;
}
