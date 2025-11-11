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
#include <stdio.h>

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
