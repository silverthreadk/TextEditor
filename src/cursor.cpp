#include "cursor.h"

#include <algorithm>

#include "screen_utils.h"

Cursor::Cursor(std::list<std::list<char> >::iterator row, std::list<char>::iterator col) {
    row_idx_ = 0;
    col_idx_ = 0;

    row_ = row;
    col_ = col;

    scroll_position_ = row;
    scroll_position_idx_ = 0;
}
Cursor::~Cursor() {
}

int Cursor::moveToLeft() {
    if (col_idx_ <= 0) return 1;

    --col_;
    --col_idx_;
    return 0;
}

int Cursor::moveToRight(const bool is_insert_mode) {
    const int end = static_cast<int>(row_->size()) - (is_insert_mode ? 0 : 1);
    if (col_idx_ >= end) return 1;

    ++col_;
    ++col_idx_;
    return 0;
}

int Cursor::moveToUp() {
    if (row_idx_ <= 0) return 1;

    if (row_idx_ == scroll_position_idx_) scrollUp();
    --row_;
    --row_idx_;

    moveCol(col_idx_);
    return 0;
}

int Cursor::moveToDown(const int row_size) {
    if (row_idx_ >= row_size - 1) return 1;

    if (row_idx_ >= scroll_position_idx_ + getScreenSize().first - 2) scrollDown();
    ++row_;
    ++row_idx_;

    moveCol(col_idx_);
    return 0;
}

int Cursor::moveToBeginning() {
    col_ = row_->begin();
    col_idx_ = 0;

    return 0;
}

int Cursor::moveToEnd() {
    if (col_ == row_->end()) return 1;

    col_ = std::prev(row_->end());
    col_idx_ = row_->size() - 1;

    return 0;
}

void Cursor::moveCol(const int n) {
    if (row_->empty()) {
        col_idx_ = 0;
        col_ = row_->begin();
    } else if (n >= row_->size()) {
        col_idx_ = row_->size() - 1;
        col_ = std::prev(row_->end());
    } else {
        col_idx_ = std::min(n, static_cast<int>(row_->size()) - 1);
        col_ = row_->begin();
        std::advance(col_, col_idx_);
    }
}

void Cursor::scrollUp() {
    if (row_idx_ <= 0 || scroll_position_idx_ <= 0) return;
    --scroll_position_idx_;
    --scroll_position_;
}

void Cursor::scrollDown() {
    if (row_idx_ != scroll_position_idx_ + getScreenSize().first - 2) return;
    ++scroll_position_idx_;
    ++scroll_position_;
}

void Cursor::scrollTo() {
    scroll_position_ = row_;
    scroll_position_idx_ = row_idx_;

    int offset = row_idx_ - getScreenSize().first - 2;
    for (int i = 0; i < offset; ++i) scrollUp();
}

std::list<std::list<char> >::iterator Cursor::getRow() {
    return row_;
}
std::list<char>::iterator Cursor::getCol() {
    return col_;
}
int Cursor::getRowIndex() {
    return row_idx_;
}
int Cursor::getColIndex() {
    return col_idx_;
}

void Cursor::setRow(const std::list<std::list<char> >::iterator r) {
    row_ = r;
}
void Cursor::setCol(const std::list<char>::iterator c) {
    col_ = c;
}
void Cursor::setRowIndex(const int idx) {
    row_idx_ = idx;
}
void Cursor::setColIndex(const int idx) {
    col_idx_ = idx;
}

void Cursor::incRow() {
    ++row_;
}
void Cursor::incCol() {
    ++col_;
}
void Cursor::incRowIndex() {
    ++row_idx_;
}
void Cursor::incColIndex() {
    ++col_idx_;
}

void Cursor::decRow() {
    --row_;
}
void Cursor::decCol() {
    --col_;
}
void Cursor::decRowIndex() {
    --row_idx_;
}
void Cursor::decColIndex() {
    --col_idx_;
}
