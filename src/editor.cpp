#include "editor.h"

#include <stdlib.h>
#include <stdio.h>
#include <list>

#ifdef WINDOWS
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

#include "cursor.h"
#include "utils.h"

using namespace std;




Editor::Editor() {
	this->filepath = "";
    mode = MODE_COMMAND;

    editNewFile();
}

Editor::Editor(const char* filepath) {
    this->filepath = "";
    mode = MODE_COMMAND;

    editFile(filepath);
}

Editor::~Editor() {
	delete cp;
}
/**
* 새 파일을 편집하는 함수
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::editNewFile() {
    list<char> temp;

    text_list.push_back(temp);

    this->cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());

	return 0;
}
/**
* 파일 경로에 있는 파일을 편집하는 함수
* @param const char* filepath 파일 경로
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::editFile(const char* filepath) {
	FILE* fp;
	char c;
	list<char> temp;
	this->filepath = filepath;

	if ((fp = fopen(this->filepath, "r")) == NULL) {
		text_list.push_back(temp);

        this->cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());
	}
	else {
		while (fscanf(fp, "%c", &c) != EOF) {
			if (c == '\n' || c == '\r') {
				text_list.push_back(temp);
				temp.clear();
			} else {
				temp.push_back(c);
			}
		}
		text_list.push_back(temp);

        this->cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());
		
		fclose(fp);
	}
	return 0;
}

/**
* 기존 파일 경로에서 파일을 저장하는 함수
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::writeFile() {
	FILE *fp;
	Cursor* cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());
	
	if ((fp = fopen(this->filepath, "w")) == NULL)
		return 1;
	else {
		for (cp->setRow(text_list.begin()); cp->getRow() != text_list.end(); cp->incRow()) {
			for (cp->setCol((*cp->getRow()).begin()); cp->getCol() != (*cp->getRow()).end(); cp->incCol()) {
				fputc(*(cp->getCol()), fp);
			}
			fputc('\n', fp);
		}

		fclose(fp);
	}

	return 0;
}
/**
* 새 파일경로에 파일을 저장하는 함수
* @param const char* filepath 파일 경로
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::writeFile(const char* filepath) {
	FILE *fp;
	this->filepath = filepath;
    Cursor* cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());

	if ((fp = fopen(this->filepath, "w")) == NULL) return 1;
	else {
		for (cp->setRow(text_list.begin()); cp->getRow() != text_list.end(); cp->incRow()) {
			for (cp->setCol((*cp->getRow()).begin()); cp->getCol() != (*cp->getRow()).end(); cp->incCol()) {
				fputc(*(cp->getCol()), fp);
			}
			fputc('\n', fp);
		}

		fclose(fp);
	}

	return 0;
}


/**
* 편집 화면을 출력하는 함수
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::printEditor() {
    clearScreen();

    //printf("------------------------------------------------------------------------\n");
    //printf("filepath : %s\n", this->filepath);
    //printf("------------------------------------------------------------------------\n");

    int screen_size = getScreenSize();
    list<list<char> >::iterator row = cp->getScrollPosition().first;
    int row_idx = cp->getScrollPosition().second;

    for (; screen_size - getConsoleCursor() - 1 > 0 && row != text_list.end(); ++row_idx, ++row) {
        printf("%d", row_idx);
        printf(" ");
        int col_idx = 0;
        list<char>::iterator col = (*row).begin();
        for (; col != (*row).end(); ++col_idx, ++col) {
            if (row_idx == cp->getRowIndex() && col_idx == this->cp->getColIndex()) printf("|");
            else printf("%c", *(col));
        }
        if (row_idx == cp->getRowIndex() && col_idx == this->cp->getColIndex()) printf("|");

        printf("\n");
    }

    int padding = screen_size - getConsoleCursor() - 1;
    for (int i = 0; i < padding; ++i)
        printf("\n");

    if (mode == MODE_LAST_LINE) {
        printf(":");
    }
    return 0;
}

int Editor::moveCursor(const char c) {
    return cp->move(c, this->text_list.size());
}

/**
* 현재 커서에서 문자를 추가하는 함수
* @param const char c
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::insertChar(const char c) {
    this->cp->setCol(++(*this->cp->getRow()).insert(this->cp->getCol(), c));
    this->cp->incColIndex();

    return 0;
}
/**
* 현재 커서에서 문자를 제거하는 함수
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::deleteChar() {
    if ((this->cp->getCol()) != (*this->cp->getRow()).end()) {
        this->cp->setCol((*this->cp->getRow()).erase(this->cp->getCol()));
    } else {
        return 1;
    }

    return 0;
}
/**
* 현재 커서에서 개행하는 함수
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::insertLine() {
    list<char> temp;

    while ((this->cp->getCol()) != (*this->cp->getRow()).end()) {
        temp.push_back(*(this->cp->getCol()));
        this->cp->setCol((*this->cp->getRow()).erase(this->cp->getCol()));
    }
    cp->scrollDown();

    this->cp->incRow();
    this->cp->setRow(text_list.insert(this->cp->getRow(), temp));
    this->cp->setCol((*this->cp->getRow()).begin());
    this->cp->incRowIndex();
    this->cp->setColIndex(0);

    return 0;
}
/**
* 현재 커서에서 라인을 삭제하는 함수
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::deleteLine() {
    list<char> temp;

    if ((this->cp->getRow()) != (this->text_list).end()) {
        cp->scrollUp();

        (*this->cp->getRow()).clear();
        this->cp->setRow((this->text_list).erase(this->cp->getRow()));

        if (cp->getRow() == (this->text_list).begin()) cp->setScrollPosition((this->text_list).begin());

        if ((this->cp->getRow()) == (this->text_list).end()) {
            if ((this->text_list).size() == 0) {
                text_list.push_back(temp);
                this->cp->setRow(text_list.begin());
                this->cp->setCol((*text_list.begin()).begin());
            }
            else {
                this->cp->decRow();
                this->cp->setCol((*this->cp->getRow()).begin());
                this->cp->decRowIndex();
            }
        }
        else
            this->cp->setCol((*this->cp->getRow()).begin());
        this->cp->setColIndex(0);
    }

    return 0;
}


const char* Editor::getFilepath() {
	return this->filepath;
}