#include "cursor.h"

Cursor::Cursor() {
	row_idx=0;
	col_idx=0;
	
	scroll_up = SCROLL_UP;
	scroll_down = SCROLL_DOWN;
	scroll_left= SCROLL_LEFT;
	scroll_right= SCROLL_RIGHT;
}
Cursor::~Cursor() {
}
/**
* Cursor를 한칸 이동하는 함수
* @param const char command 커서의 방향
* @param const int row_size 전체 행 크기
* @return int 성공 0 실패 1
*/
int Cursor::moveCursor(const char command, const int row_size) {
    switch (command) {
    case 'k':	//커서 위로 이동
        if (this->row_idx > 0) {
            if (this->row_idx == this->scroll_up)
                --scroll_up, --scroll_down;
            --(this->row);
            --(this->row_idx);

            //열 조정
            if (col_idx >= (*this->row).size()) {
                this->col_idx = scroll_right = (*this->row).size();
                this->col = (*this->row).end();
                if ((*this->row).size() > SCROLL_RIGHT) {
                    scroll_right = (*this->row).size();
                    scroll_left = scroll_right - SCROLL_RIGHT;
                } else
                    scroll_left = SCROLL_LEFT, scroll_right = SCROLL_RIGHT;
            } else {
                this->col = (*this->row).begin();
                for (int i = 0; i < col_idx && (*this->row).end() != this->col; ++i, ++this->col);
            }
        }
        break;

    case 'h':	//커서 왼쪽으로 이동
        if (this->col_idx != 0 && this->col_idx == this->scroll_left)
            --scroll_left, --scroll_right;
        if (this->col_idx > 0) {
            --(this->col);
            --(this->col_idx);
        }
        break;

    case 'l':	//커서 오른쪽으로 이동
        if (this->col_idx < (*this->row).size()) {
            if (this->col_idx == this->scroll_right)
                ++scroll_left, ++scroll_right;
            ++(this->col);
            ++(this->col_idx);
        }
        break;

    case 'j':	//커서 아래로 이동
        if (this->row_idx < row_size - 1) {
            if (this->row_idx == this->scroll_down)
                ++scroll_up, ++scroll_down;
            ++(this->row);
            ++(this->row_idx);

            //열 조정
            if (col_idx >= (*this->row).size()) {
                this->col_idx = scroll_right = (*this->row).size();
                this->col = (*this->row).end();
                if ((*this->row).size() > SCROLL_RIGHT) {
                    scroll_right = (*this->row).size();
                    scroll_left = scroll_right - SCROLL_RIGHT;
                } else
                    scroll_left = SCROLL_LEFT, scroll_right = SCROLL_RIGHT;
            } else {
                this->col = (*this->row).begin();
                for (int i = 0; i < col_idx && (*this->row).end() != this->col; ++i, ++this->col);
            }
        }
        break;

    }
    return 0;
}
/**
* Cursor 행을 이동하는 함수
* @param int n 이동할 행
* @param const int row_size 전체 행 크기
* @return int 성공 0 실패 1
*/
int Cursor::moveRow(int n, const int row_size){
    if (n > row_size)
        n = row_size;
    else if (n < 0)
        n = 0;
    if (n > this->row_idx) {
        for (; this->row_idx < n && this->row_idx<row_size - 1; ++(this->row_idx), ++(this->row));

        if (row_idx > scroll_down)
            scroll_down = row_idx, scroll_up = row_idx - SCROLL_DOWN;
    } else if (n < this->row_idx) {
        for (; this->row_idx > n; --(this->row_idx), --(this->row));

        if (row_idx < scroll_up)
            scroll_down = row_idx + SCROLL_DOWN, scroll_up = row_idx;
    }

    scroll_left = SCROLL_LEFT, scroll_right = SCROLL_RIGHT;
    this->col_idx = SCROLL_LEFT;
    this->col = (*this->row).begin();

    return 0;
}
/**
* Cursor 열을 이동하는 함수
* @param int n 이동할 열
* @return int 성공 0 실패 1
*/
int Cursor::moveCol(int n) {
    if (n > (*row).size())
        n = (*row).size();
    else if (n < 0)
        n = 0;
    if (n > this->col_idx) {
        for (; this->col_idx < n; ++(this->col_idx), ++(this->col));

        if (col_idx > scroll_right)
            scroll_right = col_idx, scroll_left = col_idx - SCROLL_RIGHT;
    } else if (n < this->col_idx) {
        for (; this->col_idx > n; --(this->col_idx), --(this->col));

        if (col_idx < scroll_left)
            scroll_right = col_idx + SCROLL_RIGHT, scroll_left = col_idx;
    }

    return 0;
}
//getter
list<list<char> >::iterator Cursor::getRow(){
	return this->row;
}
list<char>::iterator Cursor::getCol(){
	return this->col;
}
int Cursor::getRowIndex(){
	return this->row_idx;
}
int Cursor::getColIndex() {
	return this->col_idx;
}
int Cursor::getScrollUp() {
	return this->scroll_up;
}
int Cursor::getScrollDown() {
	return this->scroll_down;
}
int Cursor::getScrollLeft() {
	return this->scroll_left;
}
int Cursor::getScrollRight() {
	return this->scroll_right;
}

//setter
void Cursor::setRow(const list<list<char> >::iterator r) {
	this->row = r;
}
void Cursor::setCol(const list<char>::iterator c) {
	this->col = c;
}
void Cursor::setRowIndex(const int idx) {
	this->row_idx = idx;
}
void Cursor::setColIndex(const int idx) {
	this->col_idx = idx;
}
void Cursor::setScrollUp(const int value) {
	this->scroll_up = value;
}
void Cursor::setScrollDown(const int value) {
	this->scroll_down = value;
}
void Cursor::setScrollLeft(const int value) {
	this->scroll_left = value;
}
void Cursor::setScrollRight(const int value) {
	this->scroll_right = value;
}

//increase
void Cursor::incRow() {
	++(this->row);
}
void Cursor::incCol() {
	++(this->col);
}
void Cursor::incRowIndex() {
	++(this->row_idx);
}
void Cursor::incColIndex() {
	++(this->col_idx);
}
void Cursor::incScrollUp() {
	++(this->scroll_up);
}
void Cursor::incScrollDown() {
	++(this->scroll_down);
}
void Cursor::incScrollLeft() {
	++(this->scroll_left);
}
void Cursor::incScrollRight() {
	++(this->scroll_right);
}

//decrease
void Cursor::decRow() {
	--(this->row);
}
void Cursor::decCol() {
	--(this->col);
}
void Cursor::decRowIndex() {
	--(this->row_idx);
}
void Cursor::decColIndex() {
	--(this->col_idx);
}
void Cursor::decScrollUp() {
	--(this->scroll_up);
}
void Cursor::decScrollDown() {
	--(this->scroll_down);
}
void Cursor::decScrollLeft() {
	--(this->scroll_left);
}
void Cursor::decScrollRight() {
	--(this->scroll_right);
}