#include <iostream>

#include "editor.h"
#include "input_handler.h"

int main(int argc, char** argv) {
    Editor* editor = new Editor(argv[1]);

    InputHandler input_handler;
    input_handler.handleInput(editor);

    delete editor;
    return 0;
}
