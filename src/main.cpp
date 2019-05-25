#include <iostream>
#include <string.h>
#include <regex>
#include "editor.h"
#include "cursor.h"

using namespace std;

int main(int argc, char** argv) {
	//�Է�
	string input;	//ǥ���Է����� �޴� ��ɾ� ����
	char command = 0;	//��ɾ�
	char filepath[200];	//���� ���
	char ch;	//������ ����
	int number;	//�̵��� ��, �� ����

	smatch m;
	regex pattern("(\\w)\\s?([\\W*\\w*]*)?");

	//������
	Editor* editor;
	Editor* editor_temp;

	if (argv[1] == NULL) {
		editor = new Editor();
		editor->editNewFile();
	}
	else {
		strcpy(filepath, argv[1]);
		editor = new Editor();
		editor->editFile(filepath);
	}
	editor->printEditor();

	while (getline(cin, input)) {
		if (regex_match(input, m, pattern)) {
			command = m[1].str()[0];

			//cout << m[1] << '\n' << endl;
			//cout << m[2] << '\n' << endl;
		}
		else {
			printf("Input a correct command.\n");
		}

		switch (command) {
		case 'h':	//Ŀ�� ���� �̵�
		case 'j':	//Ŀ�� �������� �̵�
		case 'k':	//Ŀ�� ���������� �̵�
		case 'l':	//Ŀ�� �Ʒ��� �̵�
			editor->getCursor()->moveCursor(command, editor->getRowSize());
			break;

		case 'e':	//���� ����
			editor_temp = new Editor();
			//scanf("%s", filepath);
			strcpy(filepath, m[2].str().c_str());
			if (editor_temp->readFile(filepath)){
				delete editor_temp;
				printf("Input filepath to open.\n");
				}
			else {
				delete editor;
				editor = editor_temp;
			}
			break;

		case 'w':	//���� ����
			if (m[2].length() > 1) {
				strcpy(filepath, m[2].str().c_str());
				editor->writeFile(filepath);
				printf("File is saved.\n");
			}
			else if (!strcmp(editor->getFilepath(), ""))
				printf("Input filepath to write.\n");
			else if (editor->writeFile()) {
				printf("File isn't opening.\n");
			}
			else
				printf("File is saved.\n");
			break;

		case 'b':	//���� Ŀ�� ��ġ ���� ����, ���� ����
			switch (editor->backspace()) {
			case 1:
				printf("Before the cursor, character doesn't exist.\n");
				break;
			}
			break;

		case 'x':	//���� Ŀ�� ��ġ ���� ���� ����
			switch (editor->deleteChar()) {
			case 1:
				printf("Next to the cursor, character doesn't exist.\n");
				break;
			}
			break;

		case 'i':	//���� Ŀ�� ��ġ�� ���� �Է�	
			if (m[2].length() == 0) {
				printf("Input a character at the cursor.\n");
				continue;
			}
			else if (m[2].length() == 1)
				ch = m[2].str()[0];
			else if (m[2].length() > 1) {
				int i;
				for (i = 0; i < m[2].length() && m[2].str()[i] == ' '; ++i);
				ch = m[2].str()[i];
			}

            editor->insertChar(ch);
			break;

		case 'o':	//���� Ŀ�� ��ġ���� ����
            editor->insertLine();
			break;
		case 'd':	//���� Ŀ�� ��ġ���� ���� ����
            editor->deleteLine();
			break;

		case 'g':	//N��° ������ �̵�
			if (m[2].length() > 0) {
				number = atoi(m[2].str().c_str());	//���ڿ� ->����
                editor->getCursor()->moveRow(number - 1, editor->getRowSize());
			}
			else
				printf("Input a correct row number.\n");
			break;

		case 'c':	//N��° ���� �̵�
			if (m[2].length() > 0) {
				number = atoi(m[2].str().c_str());	//���ڿ� ->����

                editor->getCursor()->moveCol(number - 1);
			}
			else
				printf("Input a correct column number.\n");
			break;

		case 'q':	//���� ����
			delete editor;
			return 0;
		}
		editor->printEditor();
		command = 0;

	}

	return 0;
}