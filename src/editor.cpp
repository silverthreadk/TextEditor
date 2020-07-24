#include "editor.h"

#include <stdio.h>

#include "cursor.h"
#include "file_manager.h"
#include "screen.h"


Editor::Editor(const char* filepath)
    : file_manager_(std::unique_ptr<FileManager>(new FileManager(filepath, &text_list_))),
    cursor_(nullptr),
    screen_(std::unique_ptr<Screen>(new Screen(&text_list_, &mode_))),
    mode_(EDITOR_MODE::COMMAND) {
    editFile();
}

Editor::~Editor() {
}

int Editor::editFile() {
    int ret = file_manager_->edit();
    cursor_ = std::unique_ptr<Cursor>(new Cursor(text_list_.begin(), (text_list_.begin())->begin()));

    return ret;
}

int Editor::editFile(const char* filepath) {
    if (file_manager_->changeFile(filepath) != 0) return 1;

    int ret = file_manager_->edit();
    cursor_ = std::unique_ptr<Cursor>(new Cursor(text_list_.begin(), (text_list_.begin())->begin()));

    return ret;
}


int Editor::writeFile() {
    return file_manager_->write();
}

int Editor::writeFile(const char* filepath) {
    if (file_manager_->changeFile(filepath) != 0) return 1;

    return file_manager_->write();
}


int Editor::printEditor() const {
    return screen_->print(cursor_.get());
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

int Editor::moveCursorToSpecifiedLine(const int n) {
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

int Editor::moveCursorToRightOneWord() {
    if (cursor_->getRow()->empty() || cursor_->getCol() == std::prev(cursor_->getRow()->end())) {
        moveCursorToDown();
        moveCursorToBeginning();
        return 0;
    }

    bool previous_space = *cursor_->getCol() == ' ';
    int ret = 0;
    while ((ret = cursor_->moveToRight(false)) == 0) {
        if (previous_space && *cursor_->getCol() != ' ') break;
        previous_space = *cursor_->getCol() == ' ';
    }

    return 0;
}

int Editor::moveCursorToLeftOneWord() {
    if (cursor_->getRow()->empty() || cursor_->getCol() == cursor_->getRow()->begin()) {
        if (moveCursorToUp() == 0) moveCursorToEnd();
    }
    if (cursor_->getRow()->empty()) return 0;

    bool previous_text = *cursor_->getCol() != ' ';
    int ret = 0;
    while ((ret = cursor_->moveToLeft()) == 0) {
        if (previous_text && *cursor_->getCol() == ' ') break;
        previous_text = *cursor_->getCol() != ' ';
    }

    return 0;
}

int Editor::insertChar(const char c) {
    cursor_->setCol(++cursor_->getRow()->insert(cursor_->getCol(), c));
    cursor_->incColIndex();

    if (mode_ != EDITOR_MODE::UNDO_REDO) {
        undo_.push_back(UndoRedoInfo('i', c, cursor_->getRowIndex(), cursor_->getColIndex()));
        redo_.clear();
    }

    return 0;
}

int Editor::deleteChar() {
    if (cursor_->getCol() == cursor_->getRow()->end()) return 1;

    const char ch = *cursor_->getCol();
    cursor_->setCol(cursor_->getRow()->erase(cursor_->getCol()));

    if (mode_ != EDITOR_MODE::UNDO_REDO) {
        undo_.push_back(UndoRedoInfo('d', ch, cursor_->getRowIndex(), cursor_->getColIndex()));
        redo_.clear();
    }

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
        if (!cursor_->getRow()->empty()) cursor_->incColIndex();
        cursor_->setCol(cursor_->getRow()->insert(cursor_->getRow()->end(), paste_cursor.getCol(), paste_cursor.getRow()->end()));
        paste_cursor.getRow()->clear();
    }

    text_list_.erase(paste_cursor.getRow());

    return 0;
}

int Editor::deleteWord() {
    if (cursor_->getRow()->empty()) {
        deleteLine();
        return 0;
    }
    if (cursor_->getCol() == cursor_->getRow()->end()) {
        return 1;
    }
    if (cursor_->getCol() == std::prev(cursor_->getRow()->end())) {
        deleteChar();
        return 0;
    }

    bool previous_space = *cursor_->getCol() == ' ';
    while (deleteChar() == 0) {
        if (cursor_->getCol() == cursor_->getRow()->end()) break;
        if (previous_space ^ *cursor_->getCol() == ' ') break;
    }

    return 0;
}

int Editor::deleteWordBefore() {
    if (cursor_->getCol() == cursor_->getRow()->begin()) {
        if (cursor_->getRow() == text_list_.begin()) return 1;
        moveCursorToUp();
        moveCursorToEnd();
    }

    if (cursor_->getCol() == cursor_->getRow()->begin()) {
        deleteLine();
        return 0;
    }

    bool previous_text = *cursor_->getCol() != ' ';
    cursor_->moveToRight(true);
    while (deleteCharBefore() == 0) {
        if (cursor_->getCol() == cursor_->getRow()->begin()) break;
        if (!previous_text && *std::prev(cursor_->getCol()) != ' ') break;
        previous_text = *std::prev(cursor_->getCol()) != ' ';
    }
    cursor_->moveToLeft();

    return 0;
}

int Editor::insertLine() {
    cursor_->scrollDown();

    cursor_->incRow();
    cursor_->setRow(text_list_.insert(cursor_->getRow(), std::list<char>()));
    cursor_->incRowIndex();

    cursor_->setCol(cursor_->getRow()->begin());
    cursor_->setColIndex(0);

    mode_ = EDITOR_MODE::INSERT;

    return 0;
}

int Editor::insertLineBefore() {
    text_list_.insert(cursor_->getRow(), std::list<char>());
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
    if (cursor_->getCol() == cursor_->getRow()->begin()) return 1;

    cursor_->getRow()->erase(cursor_->getRow()->begin(), cursor_->getCol());
    cursor_->setColIndex(0);

    return 0;
}

int Editor::deleteToEndOfLine() {
    if (cursor_->getCol() == cursor_->getRow()->end()) return 1;

    cursor_->setCol(cursor_->getRow()->erase(cursor_->getCol(), cursor_->getRow()->end()));
    cursor_->setColIndex(cursor_->getRow()->size() - 1);

    return 0;
}

int Editor::undo() {
    if (undo_.empty()) return 1;
    mode_ = EDITOR_MODE::UNDO_REDO;
    auto undo_info = undo_.back();

    if (undo_info.command == 'i') {
        moveCursorToSpecifiedLine(undo_info.row);
        for (int i = cursor_->getColIndex(); i < undo_info.col; ++i) {
            moveCursorToRight();
        }
        deleteCharBefore();
    } else if (undo_info.command == 'd') {
        moveCursorToSpecifiedLine(undo_info.row);
        for (int i = cursor_->getColIndex(); i < undo_info.col; ++i) {
            moveCursorToRight();
        }
        insertChar(undo_info.ch);
    }

    redo_.push_back(undo_info);
    undo_.pop_back();

    mode_ = EDITOR_MODE::COMMAND;
    return 0;
}

int Editor::redo() {
    if (redo_.empty()) return 1;
    mode_ = EDITOR_MODE::UNDO_REDO;
    auto redo_info = redo_.back();

    if (redo_info.command == 'i') {
        moveCursorToSpecifiedLine(redo_info.row);
        for (int i = cursor_->getColIndex(); i < redo_info.col; ++i) {
            moveCursorToRight();
        }
        insertChar(redo_info.ch);
    } else if (redo_info.command == 'd') {
        moveCursorToSpecifiedLine(redo_info.row);
        for (int i = cursor_->getColIndex(); i < redo_info.col; ++i) {
            moveCursorToRight();
        }
        deleteCharBefore();
    }

    undo_.push_back(redo_info);
    redo_.pop_back();

    mode_ = EDITOR_MODE::COMMAND;
    return 0;
}

void Editor::setShowLineNumber(const bool show_line_number) {
    screen_->setShowLineNumber(show_line_number);
}
