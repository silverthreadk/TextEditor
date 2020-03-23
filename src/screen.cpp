#include "screen.h"

#include <stdio.h>

#include "cursor.h"
#include "screen_utils.h"

Screen::Screen(std::list<std::list<char> >* text_list, Cursor** cursor, EDITOR_MODE* mode)
    : text_list_(text_list),
      cursor_(cursor),
      mode_(mode),
      show_line_number_(false) {
}

Screen::~Screen() {
}

int Screen::print() {
    clearScreen();

    auto screen_size = getScreenSize();
    const int screen_height = screen_size.first;
    const int screen_width = screen_size.second;

    std::list<std::list<char> >::iterator row = (*cursor_)->getScrollPosition().first;
    int row_idx = (*cursor_)->getScrollPosition().second;

    for (; screen_height - getConsoleCursor() - 1 > 0 && row != text_list_->end(); ++row_idx, ++row) {
        int digit_number = 0;
        if (show_line_number_) {
            digit_number = log10(text_list_->size()) + 2;
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
            if (row_idx == (*cursor_)->getRowIndex() && col_idx == (*cursor_)->getColIndex()) {
                printf("|");
            } else {
                printf("%c", *(col));
            }
        }
        if (row_idx == (*cursor_)->getRowIndex() && col_idx == (*cursor_)->getColIndex()) printf("|");

        printf("\n");
    }

    printPadding();

    if (*mode_ == EDITOR_MODE::LAST_LINE) {
        printf(":");
    }

    return 0;
}

void Screen::printPadding() {
    const int padding = getScreenSize().first - getConsoleCursor() - 1;
    for (int i = 0; i < padding; ++i)
        printf("~\n");
}

