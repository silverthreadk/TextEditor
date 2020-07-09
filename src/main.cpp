#include <iostream>

#include "editor.h"
#include "input_handler.h"

int main(int argc, char** argv) {
    std::unique_ptr<Editor> editor = std::unique_ptr<Editor>(new Editor(argv[1]));

    InputHandler input_handler;
    input_handler.handleInput(editor.get());

    return 0;
}
