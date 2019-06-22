#include "editor.h"

#include <stdlib.h>
#include <stdio.h>
#include <list>

#ifdef WINDOWS
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

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

	if ((fp = fopen(this->filepath, "r")) == NULL) {
		text_list.push_back(temp);

        this->cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());
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

        this->cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());
		
		fclose(fp);
	}
	return 0;
}


int Editor::writeFile() {
	FILE *fp;
	Cursor* cp = new Cursor(text_list.begin(), (*text_list.begin()).begin());
	
	if ((fp = fopen(this->filepath, "w")) == NULL)
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

	if ((fp = fopen(this->filepath, "w")) == NULL) return 1;
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
            if (row_idx == cp->getRowIndex() && col_idx == this->cp->getColIndex()) printf("|");
            else printf("%c", *(col));
        }
        if (row_idx == cp->getRowIndex() && col_idx == this->cp->getColIndex()) printf("|");

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

int Editor::moveCursor(const char c) {
    return cp->move(c, this->text_list.size(), mode == MODE_INSERT);
}

int Editor::moveCursorToBeginning() {
    cp->setCol(cp->getRow()->begin());
    cp->setColIndex(0);

    return 0;
}

int Editor::moveCursorToEnd() {
    cp->setCol(std::prev(cp->getRow()->end()));
    cp->setColIndex(cp->getRow()->size()-1);

    return 0;
}

int Editor::insertChar(const char c) {
    this->cp->setCol(++(*this->cp->getRow()).insert(this->cp->getCol(), c));
    this->cp->incColIndex();

    return 0;
}

int Editor::deleteChar() {
    if ((this->cp->getCol()) != (*this->cp->getRow()).end()) {
        this->cp->setCol((*this->cp->getRow()).erase(this->cp->getCol()));
    } else {
        return 1;
    }

    return 0;
}

int Editor::insertLine() {
    list<char> temp;

    cp->scrollDown();

    cp->incRow();
    cp->setRow(text_list.insert(this->cp->getRow(), temp));
    cp->incRowIndex();
    cp->setCol((*cp->getRow()).begin());
    cp->setColIndex(0);

    mode = MODE_INSERT;

    return 0;
}

int Editor::insertLineBefore() {
    list<char> temp;

    text_list.insert(this->cp->getRow(), temp);
    cp->incRowIndex();
    cp->setCol((*this->cp->getRow()).begin());
    cp->setColIndex(0);

    moveCursor('k');

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
