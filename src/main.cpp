#include <iostream>
#include <string.h>
#include <regex>
#include "editor.h"
#include "cursor.h"

using namespace std;

int main(int argc, char** argv) {
	//입력
	string input;	//표준입력으로 받는 명령어 라인
	char command = 0;	//명령어
	char filepath[200];	//파일 경로
	char ch;	//삽입할 문자
	int number;	//이동할 행, 열 숫자

	smatch m;
	regex pattern("(\\w)\\s?([\\W*\\w*]*)?");

	//에디터
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
		case 'h':	//커서 위로 이동
		case 'j':	//커서 왼쪽으로 이동
		case 'k':	//커서 오른쪽으로 이동
		case 'l':	//커서 아래로 이동
			editor->getCursor()->moveCursor(command, editor->getRowSize());
			break;

		case 'e':	//파일 열기
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

		case 'w':	//파일 저장
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

		case 'b':	//현재 커서 위치 이전 문자, 라인 삭제
			switch (editor->backspace()) {
			case 1:
				printf("Before the cursor, character doesn't exist.\n");
				break;
			}
			break;

		case 'x':	//현재 커서 위치 다음 문자 삭제
			switch (editor->deleteChar()) {
			case 1:
				printf("Next to the cursor, character doesn't exist.\n");
				break;
			}
			break;

		case 'i':	//현재 커서 위치에 문자 입력	
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

		case 'o':	//현재 커서 위치에서 개행
            editor->insertLine();
			break;
		case 'd':	//현재 커서 위치에서 라인 삭제
            editor->deleteLine();
			break;

		case 'g':	//N번째 행으로 이동
			if (m[2].length() > 0) {
				number = atoi(m[2].str().c_str());	//문자열 ->숫자
                editor->getCursor()->moveRow(number - 1, editor->getRowSize());
			}
			else
				printf("Input a correct row number.\n");
			break;

		case 'c':	//N번째 열로 이동
			if (m[2].length() > 0) {
				number = atoi(m[2].str().c_str());	//문자열 ->숫자

                editor->getCursor()->moveCol(number - 1);
			}
			else
				printf("Input a correct column number.\n");
			break;

		case 'q':	//편집 종료
			delete editor;
			return 0;
		}
		editor->printEditor();
		command = 0;

	}

	return 0;
}