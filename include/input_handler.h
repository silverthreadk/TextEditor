#ifndef INPUT_HANDLER_H_
#define INPUT_HANDLER_H_

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

#endif  // INPUT_HANDLER_H_
