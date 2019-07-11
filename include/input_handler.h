#ifndef __INPUT_HANDLER__
#define __INPUT_HANDLER__

class Editor;

class InputHandler {
public:
    InputHandler();
    ~InputHandler();

    /*
     * Handle editor commands read from standard input.
     */
    void handleInput(Editor* editor);
};

#endif