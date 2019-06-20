#include "cursor.h"
#include "utils.h"

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
    const int end = static_cast<int>((*row).size()) - (is_insert_mode ? 0 : 1);

    switch (command) {
    case 'k':   //top
        if (this->row_idx > 0) {
            if (this->row_idx == this->scroll_position_idx)
                scrollUp();
            --(this->row);
            --(this->row_idx);
            if (col_idx >= (*this->row).size()) {
                this->col_idx = (*this->row).size();
                this->col = (*this->row).end();
            } else {
                this->col = (*this->row).begin();
                for (int i = 0; i < col_idx && (*this->row).end() != this->col; ++i, ++this->col);
            }
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

            if (col_idx >= (*this->row).size()) {
                this->col_idx = (*this->row).size();
                this->col = (*this->row).end();
            } else {
                this->col = (*this->row).begin();
                for (int i = 0; i < col_idx && (*this->row).end() != this->col; ++i, ++this->col);
            }
        }
        break;

    }
    return 0;
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