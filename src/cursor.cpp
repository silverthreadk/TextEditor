#include "cursor.h"

#include <algorithm>

#include "utils.h"

using namespace std;

Cursor::Cursor(std::list<std::list<char> >::iterator row, std::list<char>::iterator col) {
    row_idx=0;
    col_idx=0;

    this->row = row;
    this->col = col;

    scroll_position = row;
    scroll_position_idx = 0;
}
Cursor::~Cursor() {
}

int Cursor::moveToLeft() {
    if (col_idx <= 0) return 1;

    --col;
    --col_idx;
    return 0;
}

int Cursor::moveToRight(const bool is_insert_mode) {
    const int end = static_cast<int>(row->size()) - (is_insert_mode ? 0 : 1);
    if (col_idx >= end) return 1;

    ++col;
    ++col_idx;
    return 0;
}

int Cursor::moveToUp() {
    if (row_idx <= 0) return 1;

    if (row_idx == scroll_position_idx) scrollUp();
    --row;
    --row_idx;

    moveCol(col_idx);
    return 0;
}

int Cursor::moveToDown(const int row_size) {
    if (row_idx >= row_size - 1) return 1;

    if (row_idx >= scroll_position_idx + getScreenSize() - 2) scrollDown();
    ++row;
    ++row_idx;

    moveCol(col_idx);
    return 0;
}

int Cursor::moveToBeginning() {
    col = row->begin();
    col_idx = 0;

    return 0;
}

int Cursor::moveToEnd() {
    col = std::prev(row->end());
    col_idx = row->size() - 1;

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

void Cursor::scrollTo() {
    scroll_position = row;
    scroll_position_idx = row_idx;

    int offset = row_idx - getScreenSize() - 2;
    for (int i = 0; i < offset; ++i) scrollUp();
}

//getter
std::list<std::list<char> >::iterator Cursor::getRow(){
    return row;
}
std::list<char>::iterator Cursor::getCol(){
    return col;
}
int Cursor::getRowIndex(){
    return row_idx;
}
int Cursor::getColIndex() {
    return col_idx;
}

//setter
void Cursor::setRow(const std::list<std::list<char> >::iterator r) {
    row = r;
}
void Cursor::setCol(const std::list<char>::iterator c) {
    col = c;
}
void Cursor::setRowIndex(const int idx) {
    row_idx = idx;
}
void Cursor::setColIndex(const int idx) {
    col_idx = idx;
}

//increase
void Cursor::incRow() {
    ++row;
}
void Cursor::incCol() {
    ++col;
}
void Cursor::incRowIndex() {
    ++row_idx;
}
void Cursor::incColIndex() {
    ++col_idx;
}

//decrease
void Cursor::decRow() {
    --row;
}
void Cursor::decCol() {
    --col;
}
void Cursor::decRowIndex() {
    --row_idx;
}
void Cursor::decColIndex() {
    --col_idx;
}