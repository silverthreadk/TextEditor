#include "screen.h"

#include <stdio.h>

#include "cursor.h"
#include "screen_utils.h"

static int getDigitNumber(std::list<std::list<char> >* text_list) {
    return log10(text_list->size());
}

Screen::Screen(std::list<std::list<char> >* text_list, EDITOR_MODE* mode)
    : text_list_(text_list),
      mode_(mode),
      show_line_number_(false) {
}

Screen::~Screen() {
}

int Screen::print(Cursor* cursor) const {
    clearScreen();

    const auto screen_size = getScreenSize();
    const int screen_height = screen_size.first;
    const int screen_width = screen_size.second;

    std::list<std::list<char> >::iterator row = cursor->getScrollPosition().first;
    int row_idx = cursor->getScrollPosition().second;

    for (; screen_height - getConsoleCursor() - 1 > 0 && row != text_list_->end(); ++row_idx, ++row) {
        const int line_number_width = show_line_number_ ? getDigitNumber(text_list_) + 2 : 0;
        printLineNumber(line_number_width, row_idx);

        int col_idx = 0;
        int number_of_wordwrap = 1;
        std::list<char>::iterator col = row->begin();
        for (; col != row->end(); ++col_idx, ++col) {
            if ((col_idx + (line_number_width + 1) * number_of_wordwrap) % screen_width == 0) {
                printf("\n");
                printLineNumberPadding(line_number_width);
                ++number_of_wordwrap;
            }
            if (row_idx == cursor->getRowIndex() && col_idx == cursor->getColIndex()) {
                printf("|");
            } else {
                printf("%c", *(col));
            }
        }
        if (row_idx == cursor->getRowIndex() && col_idx == cursor->getColIndex()) printf("|");

        printf("\n");
    }

    printPadding();

    if (*mode_ == EDITOR_MODE::LAST_LINE) {
        printf(":");
    }

    return 0;
}

void Screen::printLineNumber(const int line_number_width, const int row_idx) const {
    if (!show_line_number_) return;

    printf("%*d ", line_number_width, row_idx + 1);
}

void Screen::printLineNumberPadding(const int line_number_width) const {
    if (!show_line_number_) return;

    printf("%*c ", line_number_width, ' ');
}

void Screen::printPadding() const {
    const int padding = getScreenSize().first - getConsoleCursor() - 1;
    for (int i = 0; i < padding; ++i)
        printf("~\n");
}
