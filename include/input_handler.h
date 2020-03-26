#ifndef INPUT_HANDLER_H_
#define INPUT_HANDLER_H_

#include <string>

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
    void handleInputInCommandMode(Editor* editor, char* prev_ch, std::string* num);
    void handleInputInInsertMode(Editor* editor, char* prev_ch);
    void handleArrowKeys(Editor* editor, const char prev_ch, const char ch);
};

#endif  // INPUT_HANDLER_H_
