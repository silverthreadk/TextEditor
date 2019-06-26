#include <iostream>
#include <conio.h>
#include <regex>

#include "editor.h"

using namespace std;

int main(int argc, char** argv) {
    char filepath[200];
    char ch = 0;
    char prev_ch = 0;
    int number;

    Editor* editor;

    smatch m;
    regex pattern("(\\w)\\s?([\\W*\\w*]*)?");

    if (argv[1] == NULL) {
        printf("Input filename");
        return 0;
    } else {
        strcpy(filepath, argv[1]);
        editor = new Editor(filepath);
    }

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
                editor->moveCursorToBeginning();
                break;
            }
            case '$': {
                editor->moveCursorToEnd();
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

    return 0;
}