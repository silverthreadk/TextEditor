#ifndef __INPUT_HANDLER__
#define __INPUT_HANDLER__

class Editor;

class InputHandler {
private:
    Editor* editor;
public:
    InputHandler(Editor* editor);
    ~InputHandler();

    /*
     * Handle editor commands read  from standard input.
     */
    void handleInput();
};

#endif