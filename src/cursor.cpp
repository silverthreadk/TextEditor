#include "cursor.h"
#include "utils.h"

#include <algorithm>

Cursor::Cursor(list<list<char> >::iterator row, list<char>::iterator col) {
	row_idx=0;
	col_idx=0;

    this->row = row;
    this->col = col;

    scroll_position = row;
    scroll_position_idx = 0;
}
Cursor::~Cursor() {
}

int Cursor::move(const char command, const int row_size, const bool is_insert_mode) {
    const int end = static_cast<int>(row->size()) - (is_insert_mode ? 0 : 1);

    switch (command) {
    case 'k':   //top
        if (this->row_idx > 0) {
            if (this->row_idx == this->scroll_position_idx)
                scrollUp();
            --(this->row);
            --(this->row_idx);

            moveCol(col_idx);
        }
        break;

    case 'h':   //left
        if (this->col_idx > 0) {
            --(this->col);
            --(this->col_idx);
        }
        break;

    case 'l':   //right
        if (col_idx < end) {
            ++(this->col);
            ++(this->col_idx);
        }
        break;

    case 'j':   //down
        if (this->row_idx < row_size - 1) {
            if (this->row_idx >= this->scroll_position_idx + getScreenSize() - 2)
                scrollDown();
            ++(this->row);
            ++(this->row_idx);

            moveCol(col_idx);
        }
        break;

    }
    return 0;
}

void Cursor::moveCol(const int n) {
    if (row->empty()) {
        col_idx = 0;
        col = row->begin();
    } else if (n >= row->size()) {
        col_idx = row->size() - 1;
        col = std::prev(row->end());
    } else {
        col_idx = std::min(n, static_cast<int>(row->size()) - 1);
        col = row->begin();
        std::advance(col, col_idx);
    }
}

void Cursor::scrollUp() {
    if (row_idx <= 0 || scroll_position_idx <= 0) return;
    --scroll_position_idx;
    --scroll_position;
}

void Cursor::scrollDown() {
    if (row_idx != scroll_position_idx + getScreenSize() - 2) return;
    ++scroll_position_idx;
    ++scroll_position;
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

//increase
void Cursor::incRow() {
	++(this->row);
}
void Cursor::incCol() {
	++(this->col);
}
void Cursor::incRowIndex() {
    if (row_idx == scroll_position_idx) ++scroll_position_idx;
	++(this->row_idx);
}
void Cursor::incColIndex() {
	++(this->col_idx);
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