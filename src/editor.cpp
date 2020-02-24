#include "editor.h"

#include <stdio.h>
#include <list>

#include "cursor.h"
#include "utils.h"


Editor::Editor(const char* filepath) {
    cursor_ = NULL;
    mode_ = MODE_COMMAND;
    show_line_number_ = false;
    filepath_ = filepath;

    editFile();
}

Editor::~Editor() {
    delete cursor_;
}

int Editor::editFile() {
    FILE* fp;
    char c;
    std::list<char> temp;

    if (filepath_ == NULL || (fp = fopen(filepath_, "r")) == NULL) {
        text_list_.push_back(temp);

        cursor_ = new Cursor(text_list_.begin(), (*text_list_.begin()).begin());
    } else {
        while (fscanf(fp, "%c", &c) != EOF) {
            if (c == '\n' || c == '\r') {
                text_list_.push_back(temp);
                temp.clear();
            } else {
                temp.push_back(c);
            }
        }
        text_list_.push_back(temp);

        cursor_ = new Cursor(text_list_.begin(), (*text_list_.begin()).begin());

        fclose(fp);
    }
    return 0;
}


int Editor::writeFile() {
    if (filepath_ == NULL) return 1;

    FILE *fp;
    Cursor cursor(text_list_.begin(), (*text_list_.begin()).begin());

    if ((fp = fopen(filepath_, "w")) == NULL) {
        return 1;
    } else {
        for (cursor.setRow(text_list_.begin()); cursor.getRow() != text_list_.end(); cursor.incRow()) {
            for (cursor.setCol((*cursor.getRow()).begin()); cursor.getCol() != (*cursor.getRow()).end(); cursor.incCol()) {
                fputc(*(cursor.getCol()), fp);
            }
            fputc('\n', fp);
        }

        fclose(fp);
    }

    return 0;
}

int Editor::writeFile(const char* filepath) {
    FILE *fp;
    filepath_ = filepath;
    Cursor cursor(text_list_.begin(), (*text_list_.begin()).begin());

    if ((fp = fopen(filepath, "w")) == NULL) {
        return 1;
    } else {
        for (cursor.setRow(text_list_.begin()); cursor.getRow() != text_list_.end(); cursor.incRow()) {
            for (cursor.setCol((*cursor.getRow()).begin()); cursor.getCol() != (*cursor.getRow()).end(); cursor.incCol()) {
                fputc(*(cursor.getCol()), fp);
            }
            fputc('\n', fp);
        }

        fclose(fp);
    }

    return 0;
}


int Editor::printEditor() {
    clearScreen();

    int screen_size = getScreenSize();
    std::list<std::list<char> >::iterator row = cursor_->getScrollPosition().first;
    int row_idx = cursor_->getScrollPosition().second;

    for (; screen_size - getConsoleCursor() - 1 > 0 && row != text_list_.end(); ++row_idx, ++row) {
        if (show_line_number_) {
            const int digit_number = log10(text_list_.size()) + 1;
            printf("%*d ", digit_number, row_idx + 1);
        }

        int col_idx = 0;
        std::list<char>::iterator col = (*row).begin();
        for (; col != (*row).end(); ++col_idx, ++col) {
            if (row_idx == cursor_->getRowIndex() && col_idx == cursor_->getColIndex()) {
                printf("|");
            } else {
                printf("%c", *(col));
            }
        }
        if (row_idx == cursor_->getRowIndex() && col_idx == cursor_->getColIndex()) printf("|");

        printf("\n");
    }

    int padding = screen_size - getConsoleCursor() - 1;
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

    cursor_->setCol((*cursor_->getRow()).begin());
    cursor_->setColIndex(0);

    return 0;
}

int Editor::insertChar(const char c) {
    cursor_->setCol(++(*cursor_->getRow()).insert(cursor_->getCol(), c));
    cursor_->incColIndex();

    return 0;
}

int Editor::deleteChar() {
    if ((cursor_->getCol()) != (*cursor_->getRow()).end()) {
        cursor_->setCol((*cursor_->getRow()).erase(cursor_->getCol()));
    } else {
        return 1;
    }

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
        (*cursor_->getRow()).clear();
        cursor_->setCol((*cursor_->getRow()).begin());
        cursor_->setColIndex(0);

        return 0;
    }

    cursor_->scrollUp();
    (*cursor_->getRow()).clear();
    cursor_->setRow(text_list_.erase(cursor_->getRow()));

    if (cursor_->getRow() == text_list_.begin()) cursor_->setScrollPosition(text_list_.begin());

    if (cursor_->getRow() == text_list_.end()) {
        cursor_->decRow();
        cursor_->decRowIndex();
    }

    cursor_->setCol((*cursor_->getRow()).begin());
    cursor_->setColIndex(0);

    return 0;
}

int Editor::deleteToBeginningOfLine() {
    if ((cursor_->getCol()) != (*cursor_->getRow()).begin()) {
        (*cursor_->getRow()).erase(cursor_->getRow()->begin(), cursor_->getCol());
        cursor_->setColIndex(0);
    } else {
        return 1;
    }
}

int Editor::deleteToEndOfLine() {
    if ((cursor_->getCol()) == (*cursor_->getRow()).end()) return 1;

    cursor_->setCol((*cursor_->getRow()).erase(cursor_->getCol(), cursor_->getRow()->end()));
    cursor_->setColIndex((*cursor_->getRow()).size() - 1);

    return 0;
}
