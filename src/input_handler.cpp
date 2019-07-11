#include "input_handler.h"

#include <iostream>
#include <conio.h>
#include <regex>

#include "editor.h"
#include "utils.h"

using namespace std;

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

void InputHandler::handleInput(Editor* editor)
{
    char ch = 0;
    char prev_ch = 0;
    string num = "";

    smatch m;
    regex pattern("(\\w)\\s?([\\W*\\w*]*)?");

    while (editor->getMode() < MODE_END) {
        editor->printEditor();
        if (editor->getMode() == MODE_LAST_LINE) {
            string input, command1, command2, str;
            getline(cin, input);

            if (regex_match(input, m, pattern)) {
                command1 = m[0].str();
                command2 = (m[0].str() == m[1].str()) ? "" : m[1].str();
                str = m[2].str();
            }
            if (command1 == "q") {
                editor->setMode(MODE_END);
                break;
            } else if (command1 == "w") {
                editor->writeFile();
            } else if (command2 == "w") {
                editor->writeFile(str.c_str());
            } else if (command1 == "wq") {
                editor->setMode(MODE_END);
                editor->writeFile();
                break;
            } else if (command1 == "set nu" || command1 == "set number") {
                editor->setLineNumber(true);
            } else if (command1 == "set nonumber") {
                editor->setLineNumber(false);
            } else if(isDigit(command1)){
                int n = atoi(command1.c_str());
                editor->moveCursorToSpecifiedLine(n-1);
            }
            editor->setMode(MODE_COMMAND);
            ch = 0;
        } else if (editor->getMode() == MODE_COMMAND) {
            ch = getch();
            switch (ch) {
            case ':': {
                editor->setMode(MODE_LAST_LINE);
                break;
            }
            case 'a': {
                editor->setMode(MODE_INSERT);
                editor->moveCursorToRight();
                break;
            }
            case 'A': {
                editor->moveCursorToEnd();
                editor->setMode(MODE_INSERT);
                break;
            }
            case 'i': {
                editor->setMode(MODE_INSERT);
                break;
            }
            case 'I': {
                editor->moveCursorToBeginning();
                editor->setMode(MODE_INSERT);
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
                    prev_ch = 0;
                } else {
                    prev_ch = ch;
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
                    prev_ch = 0;
                }
                break;
            }
            case '$': {
                if (prev_ch == 'd') {
                    editor->deleteToEndOfLine();
                    prev_ch = 0;
                } else {
                    editor->moveCursorToEnd();
                }
                break;
            }
            case 'k': {
                editor->moveCursorToUp();
                break;
            }
            case 'j': {
                editor->moveCursorToDown();
                break;
            }
            case 'l': {
                editor->moveCursorToRight();
                break;
            }
            case 'h': {
                editor->moveCursorToLeft();
                break;
            }
            case 'G': {
                int n = atoi(num.c_str());
                editor->moveCursorToSpecifiedLine(n - 1);
            }
            }

            if (ch >= '1' && ch <= '9') {
                num += ch;
            } else {
                num = "";
            }
        } else if (editor->getMode() == MODE_INSERT) {
            ch = getch();
            if (ch == 27) { // ESC
                editor->moveCursorToLeft();
                editor->setMode(MODE_COMMAND);
            } else {
                editor->insertChar(ch);
            }
        }
    }
}