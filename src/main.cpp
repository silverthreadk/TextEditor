#include <iostream>

#include "editor.h"
#include "input_handler.h"

using namespace std;

int main(int argc, char** argv) {
    if (argv[1] == NULL) {
        printf("Input filename");
        return 0;
    }

    Editor* editor = new Editor(argv[1]);

    InputHandler input_handler(editor);
    input_handler.handleInput();

    delete editor;
    return 0;
}