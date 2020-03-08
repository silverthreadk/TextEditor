#include "editor.h"

#include <stdio.h>

#include "cursor.h"
#include "file_manager.h"
#include "screen.h"


Editor::Editor(const char* filepath)
    : file_manager_(new FileManager(filepath, &text_list_)),
    cursor_(nullptr),
    screen_(new Screen(&text_list_, &cursor_, &mode_)),
    mode_(EDITOR_MODE::COMMAND) {
    editFile();
}

Editor::~Editor() {
    delete file_manager_;
    delete cursor_;
    delete screen_;
}

int Editor::editFile() {
    int ret = file_manager_->edit();
    cursor_ = new Cursor(text_list_.begin(), (text_list_.begin())->begin());

    return ret;
}

int Editor::editFile(const char* filepath) {
    if (file_manager_->changeFile(filepath) != 0) return 1;

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
    return screen_->print();
}

int Editor::moveCursorToLeft() {
    return cursor_->moveToLeft();
}

int Editor::moveCursorToRight() {
    return cursor_->moveToRight(mode_ == EDITOR_MODE::INSERT);
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

    mode_ = EDITOR_MODE::INSERT;

    return 0;
}

int Editor::insertLineBefore() {
    std::list<char> temp;

    text_list_.insert(cursor_->getRow(), temp);
    cursor_->incScrollPositionIndex();
    cursor_->incRowIndex();

    cursor_->moveToBeginning();
    cursor_->moveToUp();

    mode_ = EDITOR_MODE::INSERT;

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

void Editor::setShowLineNumber(bool show_line_number) {
    screen_->setShowLineNumber(show_line_number);;
}