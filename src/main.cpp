#include <iostream>
#include <string>
#include <conio.h>

#include "editor.h"

using namespace std;

int main(int argc, char** argv) {
	char filepath[200];
	char ch = 0;
	int number;

	Editor* editor;

	if (argv[1] == NULL) {
        printf("Input filename");
        return 0;
	} else {
		strcpy(filepath, argv[1]);
		editor = new Editor(filepath);
	}
	editor->printEditor();
    getch();
	while (editor->getMode() < MODE_END) {
        if (editor->getMode() == MODE_LAST_LINE) {
            string command;
            getline(cin, command);
            
            if (command == "q") {
                editor->setMode(MODE_END);
                break;
            } else if (command == "w") {
                editor->writeFile();
            } else if (command == "wq") {
                editor->setMode(MODE_END);
                editor->writeFile();
                break;
            } else if (command == "set nu" || command == "set number") {
                editor->setLineNumber(true);
            } else if (command == "set nonumber") {
                editor->setLineNumber(false);
            }
            editor->setMode(MODE_COMMAND);
            ch = 0;
        } else {
            ch = getch();
        }
        if (ch == 27) { // ESC
            editor->setMode(MODE_COMMAND);
        } else if (editor->getMode() == MODE_COMMAND) {
            switch (ch) {
            case ':': {
                editor->setMode(MODE_LAST_LINE);
                break;
            }
            case 'a': {
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
            case 'd': {
                editor->deleteLine();
                break;
            }
            case 'k':    // key up
            case 'j':    // key down
            case 'l':    // key right
            case 'h':    // key left
                editor->moveCursor(ch);
                break;
            }
        } else if (editor->getMode() == MODE_INSERT) {
            editor->appendChar(ch);
        }
        editor->printEditor();
	}

	return 0;
}