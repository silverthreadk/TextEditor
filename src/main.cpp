#include <iostream>
#include <string>
#include <conio.h>

#include "editor.h"

using namespace std;

int main(int argc, char** argv) {
	//입력
	char filepath[200];	//파일 경로
	char ch = 0;	//삽입할 문자
	int number;	//이동할 행, 열 숫자

	//에디터
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
            std:cin >> command;
            if (command.compare("q") == 0) {
                editor->setMode(MODE_END);
                break;
            } else if (command.compare("w") == 0) {
                editor->writeFile();
            } else if (command.compare("wq") == 0) {
                editor->setMode(MODE_END);
                editor->writeFile();
                break;
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