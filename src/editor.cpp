#include "editor.h"

#include <stdio.h>

#include "cursor.h"
#include "file_manager.h"
#include "utils.h"


Editor::Editor(const char* filepath) {
    cursor_ = NULL;
    mode_ = MODE_COMMAND;
    show_line_number_ = false;
    file_manager_ = new FileManager(filepath, &text_list_);

    editFile();
}

Editor::~Editor() {
    delete file_manager_;
    delete cursor_;
}

int Editor::editFile() {
    int ret = file_manager_->edit();
    cursor_ = new Cursor(text_list_.begin(), (text_list_.begin())->begin());

    return ret;
}


int Editor::writeFile() {
    return file_manager_->write();
}

int Editor::writeFile(const char* filepath) {
    if (file_manager_->changeFile(filepath) != 0) return 1;

    return file_manager_->write();
}


int Editor::printEditor() {
    clearScreen();

    auto screen_size = getScreenSize();
    const int screen_height = screen_size.first;
    const int screen_width = screen_size.second;

    std::list<std::list<char> >::iterator row = cursor_->getScrollPosition().first;
    int row_idx = cursor_->getScrollPosition().second;

    for (; screen_height - getConsoleCursor() - 1 > 0 && row != text_list_.end(); ++row_idx, ++row) {
        int digit_number = 0;
        if (show_line_number_) {
            digit_number = log10(text_list_.size()) + 2;
            printf("%*d ", digit_number, row_idx + 1);
        }

        int col_idx = 0;
        int number_of_wordwrap = 1;
        std::list<char>::iterator col = row->begin();
        for (; col != row->end(); ++col_idx, ++col) {
            if ((col_idx + (digit_number + 1) * number_of_wordwrap) % screen_width == 0) {
                printf("\n");
                if (show_line_number_) printf("%*c ", digit_number, ' ');
                ++number_of_wordwrap;
            }
            if (row_idx == cursor_->getRowIndex() && col_idx == cursor_->getColIndex()) {
                printf("|");
            } else {
                printf("%c", *(col));
            }
        }
        if (row_idx == cursor_->getRowIndex() && col_idx == cursor_->getColIndex()) printf("|");

        printf("\n");
    }

    int padding = screen_height - getConsoleCursor() - 1;
    for (int i = 0; i < padding; ++i)
        printf("\n");

    if (mode_ == MODE_LAST_LINE) {
        printf(":");
    }
    return 0;
}

int Editor::moveCursorToLeft() {
    return cursor_->moveToLeft();
}

int Editor::moveCursorToRight() {
    return cursor_->moveToRight(mode_ == MODE_INSERT);
}

int Editor::moveCursorToUp() {
    return cursor_->moveToUp();
}

int Editor::moveCursorToDown() {
    return cursor_->moveToDown(text_list_.size());
}

int Editor::moveCursorToBeginning() {
    return cursor_->moveToBeginning();
}

int Editor::moveCursorToEnd() {
    return cursor_->moveToEnd();
}

int Editor::moveCursorToSpecifiedLine(int n) {
    if (text_list_.size() <= n) {
        cursor_->setRow(std::prev(text_list_.end()));
        cursor_->setRowIndex(text_list_.size() - 1);
    } else {
        auto temp = text_list_.begin();
        std::advance(temp, n);
        cursor_->setRow(temp);
        cursor_->setRowIndex(n);
    }
    cursor_->scrollTo();

    cursor_->setCol(cursor_->getRow()->begin());
    cursor_->setColIndex(0);

    return 0;
}

int Editor::insertChar(const char c) {
    cursor_->setCol(++cursor_->getRow()->insert(cursor_->getCol(), c));
    cursor_->incColIndex();

    return 0;
}

int Editor::deleteChar() {
    if ((cursor_->getCol()) == cursor_->getRow()->end()) return 1;

    cursor_->setCol(cursor_->getRow()->erase(cursor_->getCol()));

    return 0;
}

int Editor::deleteCharBefore() {
    if (cursor_->getCol() != cursor_->getRow()->begin()) {
        moveCursorToLeft();
        cursor_->setCol(cursor_->getRow()->erase(cursor_->getCol()));
        return 0;
    }

    if (cursor_->getRow() == text_list_.begin()) return 1;

    Cursor paste_cursor(cursor_->getRow(), cursor_->getCol());

    moveCursorToUp();
    moveCursorToEnd();

    if (!paste_cursor.getRow()->empty()) {
        cursor_->setCol(cursor_->getRow()->insert(cursor_->getRow()->end(), paste_cursor.getCol(), paste_cursor.getRow()->end()));
        cursor_->incColIndex();
        paste_cursor.getRow()->clear();
    }

    text_list_.erase(paste_cursor.getRow());

    return 0;
}

int Editor::insertLine() {
    std::list<char> temp;

    cursor_->scrollDown();

    cursor_->incRow();
    cursor_->setRow(text_list_.insert(cursor_->getRow(), temp));
    cursor_->incRowIndex();

    cursor_->moveToBeginning();

    mode_ = MODE_INSERT;

    return 0;
}

int Editor::insertLineBefore() {
    std::list<char> temp;

    text_list_.insert(cursor_->getRow(), temp);
    cursor_->incScrollPositionIndex();
    cursor_->incRowIndex();

    cursor_->moveToBeginning();
    cursor_->moveToUp();

    mode_ = MODE_INSERT;

    return 0;
}

int Editor::deleteLine() {
    if (cursor_->getRow() == text_list_.end()) return 1;

    if (text_list_.size() == 1) {
        cursor_->getRow()->clear();
        cursor_->setCol(cursor_->getRow()->begin());
        cursor_->setColIndex(0);

        return 0;
    }

    cursor_->scrollUp();
    cursor_->getRow()->clear();
    cursor_->setRow(text_list_.erase(cursor_->getRow()));

    if (cursor_->getRow() == text_list_.begin()) cursor_->setScrollPosition(text_list_.begin());

    if (cursor_->getRow() == text_list_.end()) {
        cursor_->decRow();
        cursor_->decRowIndex();
    }

    cursor_->setCol(cursor_->getRow()->begin());
    cursor_->setColIndex(0);

    return 0;
}

int Editor::deleteToBeginningOfLine() {
    if ((cursor_->getCol()) == cursor_->getRow()->begin()) return 1;

    cursor_->getRow()->erase(cursor_->getRow()->begin(), cursor_->getCol());
    cursor_->setColIndex(0);

    return 0;
}

int Editor::deleteToEndOfLine() {
    if ((cursor_->getCol()) == cursor_->getRow()->end()) return 1;

    cursor_->setCol(cursor_->getRow()->erase(cursor_->getCol(), cursor_->getRow()->end()));
    cursor_->setColIndex(cursor_->getRow()->size() - 1);

    return 0;
}
