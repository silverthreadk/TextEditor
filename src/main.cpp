#include <iostream>
#include <string>
#include <conio.h>

#include "editor.h"
#include "cursor.h"

using namespace std;

int main(int argc, char** argv) {
	//�Է�
	char filepath[200];	//���� ���
	char ch = 0;	//������ ����
	int number;	//�̵��� ��, �� ����

	//������
	Editor* editor;

	if (argv[1] == NULL) {
        printf("Input filename");
        return 0;
	} else {
		strcpy(filepath, argv[1]);
		editor = new Editor();
		editor->editFile(filepath);
	}
	editor->printEditor();
    getch();
	while (editor->getMode() < MODE_END) {
        if (editor->getMode() == MODE_LAST_LINE) {
            string command;
            std:cin >> command;
            if (command.compare("q") == 0) {
                editor->setMode(MODE_END);
            } else if (command.compare("wq") == 0) {
                editor->setMode(MODE_SAVE);
                editor->writeFile();
            } else {
                editor->setMode(MODE_COMMAND);
            }
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
                editor->getCursor()->moveCursor(ch, editor->getRowSize());
                break;
            }
        } else if (editor->getMode() == MODE_INSERT) {
            editor->insertChar(ch);
        }
        editor->printEditor();
	}

	return 0;
}