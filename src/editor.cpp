#include "editor.h"

#include <stdlib.h>
#include <stdio.h>
#include <list>

#include "cursor.h"

using namespace std;

Editor::Editor() {
	this->cp = new Cursor();
	this->filepath = "";
    mode = MODE_COMMAND;
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

	this->cp->setRow(text_list.begin());
	this->cp->setCol((*text_list.begin()).begin());

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

		this->cp->setRow(text_list.begin());
		this->cp->setCol((*text_list.begin()).begin());
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

		this->cp->setRow(text_list.begin());
		this->cp->setCol((*text_list.begin()).begin());
		
		fclose(fp);
	}
	return 0;
}
/**
* 기존 파일을 읽어오는 함수
* @param const char* filepath 파일 경로
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::readFile(const char* filepath) {
	FILE* fp;
	char c;
	list<char> temp;
	this->filepath = filepath;

	if ((fp = fopen(this->filepath, "r")) == NULL)
		return 1;
	else {
		while (fscanf(fp, "%c", &c) != EOF) {
			if (c == '\n') {
				text_list.push_back(temp);
				temp.clear();
			}
			else {
				temp.push_back(c);
			}
		}
		text_list.push_back(temp);

		this->cp->setRow(text_list.begin());
		this->cp->setCol((*text_list.begin()).begin());

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
	Cursor* cp = new Cursor();
	
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
	Cursor* cp = new Cursor();

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
    system("cls");

    printf("------------------------------------------------------------------------\n");
    printf("filepath : %s\n", this->filepath);
    printf("------------------------------------------------------------------------\n");

    //행 인덱스 맞추기
    int row_idx = this->cp->getScrollUp();
    list<list<char> >::iterator row = text_list.begin();

    std::advance(row, row_idx);

    for (; row_idx <= this->cp->getScrollDown() && row != text_list.end(); ++row_idx, ++row) {
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


    //여백
    for (int i = 0; i <= WINDOW_BOTTOM - (int)text_list.size(); ++i)
        printf("\n");
    //printf("scroll : %d, %d, %d, %d, cursor: %d, %d\n", this->cp->getScrollUp(), this->cp->getScrollDown(), this->cp->getScrollLeft(), scroll_right, this->cp->getRowIndex(), this->cp->getColIndex());

    if (mode == MODE_LAST_LINE) {
        printf(":");
    }
    return 0;
}
/**
* 현재 커서에서 문자를 추가하는 함수
* @param const char c
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::insertChar(const char c) {
    this->cp->setCol(++(*this->cp->getRow()).insert(this->cp->getCol(), c));
    if (this->cp->getColIndex() == this->cp->getScrollRight())
        this->cp->incScrollLeft(), this->cp->incScrollRight();
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
    if (this->cp->getRowIndex() == this->cp->getScrollDown())
        this->cp->incScrollUp(), this->cp->incScrollDown();

    this->cp->incRow();
    this->cp->setRow(text_list.insert(this->cp->getRow(), temp));
    this->cp->setCol((*this->cp->getRow()).begin());
    this->cp->incRowIndex();
    this->cp->setColIndex(0);

    if (this->cp->getScrollLeft() > WINDOW_LEFT)
        this->cp->setScrollLeft(WINDOW_LEFT), this->cp->setScrollRight(WINDOW_RIGHT);

    return 0;
}
/**
* 현재 커서에서 라인을 삭제하는 함수
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::deleteLine() {
    list<char> temp;

    if ((this->cp->getRow()) != (this->text_list).end()) {
        if (this->cp->getScrollDown()>WINDOW_BOTTOM) {
            this->cp->decScrollDown();
            this->cp->decScrollUp();
        }

        (*this->cp->getRow()).clear();
        this->cp->setRow((this->text_list).erase(this->cp->getRow()));

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
/**
* Backspace 키를 구현한 함수, 커서 앞 문자, 행을 지운다.
* @return int 성공 0, 실패 0 이외의 값
*/
int Editor::backspace() {
    if ((*this->cp->getRow()).begin() == this->cp->getCol() && (this->text_list).begin() != this->cp->getRow()) {// 행 지울 때
        list<list<char> >::iterator temp_row;

        temp_row = this->cp->getRow();

        this->cp->decRowIndex();
        this->cp->decRow();
        this->cp->setCol((*this->cp->getRow()).end() != (*this->cp->getRow()).begin() ? (--(*this->cp->getRow()).end()) : (*this->cp->getRow()).begin());
        this->cp->setColIndex((*this->cp->getRow()).size());

        //화면 표시 조정
        if ((*this->cp->getRow()).size() > WINDOW_RIGHT) {
            this->cp->setScrollRight((*this->cp->getRow()).size() + 1);
            this->cp->setScrollLeft(this->cp->getScrollRight() - WINDOW_RIGHT);
        } else {
            this->cp->setScrollRight(WINDOW_RIGHT);
            this->cp->setScrollLeft(WINDOW_LEFT);
        }

        if (this->cp->getScrollDown()>WINDOW_BOTTOM) {
            this->cp->decScrollDown();
            this->cp->decScrollUp();
        }

        //커서 뒷 문자 윗행으로
        while ((*temp_row).begin() != (*temp_row).end()) {
            (*this->cp->getRow()).push_back(*(*temp_row).begin());
            (*temp_row).pop_front();
        }
        (this->text_list).erase(++this->cp->getRow());

        if ((*this->cp->getRow()).end() != this->cp->getCol())
            this->cp->incCol();
        else
            this->cp->setCol((*this->cp->getRow()).begin());
    } else if ((*this->cp->getRow()).begin() != this->cp->getCol()) {// 열 지울 때
        if (this->cp->getScrollLeft()>WINDOW_LEFT) {
            this->cp->decScrollLeft();
            this->cp->decScrollRight();
        }
        this->cp->setCol((*this->cp->getRow()).erase(--this->cp->getCol()));
        this->cp->decColIndex();
    } else {
        return 1;
    }

    return 0;
}

Cursor* Editor::getCursor() {
	return this->cp;
}

int Editor::getRowSize() {
	return this->text_list.size();
}

const char* Editor::getFilepath() {
	return this->filepath;
}