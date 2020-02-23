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

 private:
    void handleArrowKeys(Editor* editor, const char prev_ch, const char ch);
};

#endif  // INPUT_HANDLER_H_
