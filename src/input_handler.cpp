#include "input_handler.h"

#include <conio.h>

#include <iostream>
#include <regex>

#include "editor.h"
#include "string_utils.h"


InputHandler::InputHandler() {
}

InputHandler::~InputHandler() {
}

void InputHandler::handleInput(Editor* editor) {
    char ch = 0;
    char prev_ch = 0;
    std::string num = "";

    std::smatch m;
    std::regex pattern("([\\S]*)\\s?([\\W*\\w*]*)?");

    while (editor->getMode() < EDITOR_MODE::END) {
        editor->printEditor();
        if (editor->getMode() == EDITOR_MODE::LAST_LINE) {
            std::string input, command1, command2, str;
            getline(std::cin, input);

            if (regex_match(input, m, pattern)) {
                command1 = m[0].str();
                command2 = (m[0].str() == m[1].str()) ? "" : m[1].str();
                str = m[2].str();
            }
            if (command1 == "q") {
                editor->setMode(EDITOR_MODE::END);
                break;
            } else if (command1 == "w") {
                editor->writeFile();
            } else if (command1 == "wq") {
                editor->setMode(EDITOR_MODE::END);
                editor->writeFile();
                break;
            } else if (command2 == "wq") {
                editor->writeFile(str.c_str());
            } else if (command2 == "e") {
                editor->editFile(str.c_str());
            } else if (command1 == "set nu" || command1 == "set number") {
                editor->setShowLineNumber(true);
            } else if (command1 == "set nonumber") {
                editor->setShowLineNumber(false);
            } else if (isDigit(command1)) {
                int n = std::stoi(command1);
                editor->moveCursorToSpecifiedLine(n-1);
            }
            editor->setMode(EDITOR_MODE::COMMAND);
            ch = 0;
        } else if (editor->getMode() == EDITOR_MODE::COMMAND) {
            ch = getch();
            switch (ch) {
            case ':': {
                editor->setMode(EDITOR_MODE::LAST_LINE);
                break;
            }
            case 'a': {
                editor->setMode(EDITOR_MODE::INSERT);
                editor->moveCursorToRight();
                break;
            }
            case 'A': {
                editor->moveCursorToEnd();
                editor->setMode(EDITOR_MODE::INSERT);
                break;
            }
            case 'i': {
                editor->setMode(EDITOR_MODE::INSERT);
                break;
            }
            case 'I': {
                editor->moveCursorToBeginning();
                editor->setMode(EDITOR_MODE::INSERT);
                break;
            }
            case 'w': {
                if (prev_ch == 'd') {
                    editor->deleteWord();
                } else {
                    editor->moveCursorToRightOneWord();
                }
                break;
            }
            case 'b': {
                if (prev_ch == 'd') {
                    editor->deleteWordBefore();
                } else {
                    editor->moveCursorToLeftOneWord();
                }
                break;
            }
            case 'x': {
                editor->deleteChar();
                break;
            }
            case 'o': {
                editor->insertLine();
                break;
            }
            case 'O': {
                editor->insertLineBefore();
                break;
            }
            case 'd': {
                if (prev_ch == 'd') {
                    editor->deleteLine();
                }
                break;
            }
            case '0': {
                if (num.empty()) {
                    editor->moveCursorToBeginning();
                } else {
                    num += ch;
                }
                break;
            }
            case '^': {
                if (prev_ch == 'd') {
                    editor->deleteToBeginningOfLine();
                }
                break;
            }
            case '$': {
                if (prev_ch == 'd') {
                    editor->deleteToEndOfLine();
                } else {
                    editor->moveCursorToEnd();
                }
                break;
            }
            case 'k': {
                editor->moveCursorToUp();
                break;
            }
            case 'j':
            case '\r': {
                editor->moveCursorToDown();
                break;
            }
            case 'l': {
                editor->moveCursorToRight();
                break;
            }
            case 'h':
            case '\b': {
                editor->moveCursorToLeft();
                break;
            }
            case 'G': {
                if (isDigit(num)) {
                    const int n = std::stoi(num);
                    editor->moveCursorToSpecifiedLine(n - 1);
                }
                break;
            }
            }

            handleArrowKeys(editor, prev_ch, ch);

            if (prev_ch == 'd' && ch == 'd') {
                prev_ch = 0;
            } else {
                prev_ch = ch;
            }

            if (ch >= '1' && ch <= '9') {
                num += ch;
            } else {
                num = "";
            }
        } else if (editor->getMode() == EDITOR_MODE::INSERT) {
            ch = getch();
            if (ch == 27) {  // ESC
                editor->moveCursorToLeft();
                editor->setMode(EDITOR_MODE::COMMAND);
            } else if (ch == -32) {
            } else if (prev_ch == -32) {
                handleArrowKeys(editor, prev_ch, ch);
            } else if (ch == '\r') {
                if (editor->moveCursorToDown() != 0) editor->insertLine();
            } else if (ch == '\b') {
                editor->deleteCharBefore();
            } else {
                editor->insertChar(ch);
            }
            prev_ch = ch;
        }
    }
}

void InputHandler::handleArrowKeys(Editor* editor, const char prev_ch, const char ch) {
    if (prev_ch != -32) return;
    switch (ch) {
    case 'H': {
        editor->moveCursorToUp();
        break;
    }
    case 'P': {
        editor->moveCursorToDown();
        break;
    }
    case 'M': {
        editor->moveCursorToRight();
        break;
    }
    case 'K': {
        editor->moveCursorToLeft();
        break;
    }
    }
}
