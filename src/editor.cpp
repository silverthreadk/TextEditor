#include "editor.h"

#include <stdio.h>
#include <list>

#include "cursor.h"
#include "utils.h"

using namespace std;

Editor::Editor(const char* filepath) {
    cp = NULL;
    mode = MODE_COMMAND;
    line_number = false;
    this->filepath = filepath;

    editFile();
}

Editor::~Editor() {
    delete cp;
}

int Editor::editFile() {
    FILE* fp;
    char c;
    list<char> temp;

    if ((fp = fopen(filepath, "r")) == NULL) {
        text_list.push_back(temp);

        cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());
    } else {
        while (fscanf(fp, "%c", &c) != EOF) {
            if (c == '\n' || c == '\r') {
                text_list.push_back(temp);
                temp.clear();
            } else {
                temp.push_back(c);
            }
        }
        text_list.push_back(temp);

        cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());

        fclose(fp);
    }
    return 0;
}


int Editor::writeFile() {
    FILE *fp;
    Cursor* cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());

    if ((fp = fopen(filepath, "w")) == NULL)
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

int Editor::writeFile(const char* filepath) {
    FILE *fp;
    this->filepath = filepath;
    Cursor* cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());

    if ((fp = fopen(filepath, "w")) == NULL) return 1;
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


int Editor::printEditor() {
    clearScreen();

    int screen_size = getScreenSize();
    list<list<char> >::iterator row = cp->getScrollPosition().first;
    int row_idx = cp->getScrollPosition().second;

    for (; screen_size - getConsoleCursor() - 1 > 0 && row != text_list.end(); ++row_idx, ++row) {
        if (line_number) {
            const int digit_number = log10(text_list.size()) + 1;
            printf("%*d ", digit_number, row_idx + 1);
        }

        int col_idx = 0;
        list<char>::iterator col = (*row).begin();
        for (; col != (*row).end(); ++col_idx, ++col) {
            if (row_idx == cp->getRowIndex() && col_idx == cp->getColIndex()) printf("|");
            else printf("%c", *(col));
        }
        if (row_idx == cp->getRowIndex() && col_idx == cp->getColIndex()) printf("|");

        printf("\n");
    }

    int padding = screen_size - getConsoleCursor() - 1;
    for (int i = 0; i < padding; ++i)
        printf("\n");

    if (mode == MODE_LAST_LINE) {
        printf(":");
    }
    return 0;
}

int Editor::moveCursorToLeft() {
    return cp->moveToLeft();
}

int Editor::moveCursorToRight() {
    return cp->moveToRight(mode == MODE_INSERT);
}

int Editor::moveCursorToUp() {
    return cp->moveToUp();
}

int Editor::moveCursorToDown() {
    return cp->moveToDown(text_list.size());
}

int Editor::moveCursorToBeginning() {
    return cp->moveToBeginning();
}

int Editor::moveCursorToEnd() {
    return cp->moveToEnd();
}

int Editor::moveCursorToSpecifiedLine(int n) {
    if (text_list.size() <= n) {
        cp->setRow(std::prev(text_list.end()));
        cp->setRowIndex(text_list.size() - 1);
    } else {
        auto temp = text_list.begin();
        std::advance(temp, n);
        cp->setRow(temp);
        cp->setRowIndex(n);
    }
    cp->scrollTo();

    cp->setCol((*cp->getRow()).begin());
    cp->setColIndex(0);

    return 0;
}

int Editor::insertChar(const char c) {
    cp->setCol(++(*cp->getRow()).insert(cp->getCol(), c));
    cp->incColIndex();

    return 0;
}

int Editor::deleteChar() {
    if ((cp->getCol()) != (*cp->getRow()).end()) {
        cp->setCol((*cp->getRow()).erase(cp->getCol()));
    } else {
        return 1;
    }

    return 0;
}

int Editor::insertLine() {
    list<char> temp;

    cp->scrollDown();

    cp->incRow();
    cp->setRow(text_list.insert(cp->getRow(), temp));
    cp->incRowIndex();

    cp->moveToBeginning();

    mode = MODE_INSERT;

    return 0;
}

int Editor::insertLineBefore() {
    list<char> temp;

    text_list.insert(cp->getRow(), temp);
    cp->incScrollPositionIndex();
    cp->incRowIndex();

    cp->moveToBeginning();
    cp->moveToUp();

    mode = MODE_INSERT;

    return 0;
}

int Editor::deleteLine() {
    if (cp->getRow() == text_list.end()) return 1;

    if (text_list.size() == 1) {
        (*cp->getRow()).clear();
        cp->setCol((*cp->getRow()).begin());
        cp->setColIndex(0);

        return 0;
    }

    cp->scrollUp();
    (*cp->getRow()).clear();
    cp->setRow(text_list.erase(cp->getRow()));

    if (cp->getRow() == text_list.begin()) cp->setScrollPosition(text_list.begin());

    if (cp->getRow() == text_list.end()) {
        cp->decRow();
        cp->decRowIndex();
    }

    cp->setCol((*cp->getRow()).begin());
    cp->setColIndex(0);

    return 0;
}

int Editor::deleteToBeginningOfLine()
{
    if ((cp->getCol()) != (*cp->getRow()).begin()) {
        (*cp->getRow()).erase(cp->getRow()->begin(), cp->getCol());
        cp->setColIndex(0);
    } else {
        return 1;
    }
}

int Editor::deleteToEndOfLine()
{
    if ((cp->getCol()) != (*cp->getRow()).end()) {
        (*cp->getRow()).erase(cp->getCol(), cp->getRow()->end());
    } else {
        return 1;
    }
}