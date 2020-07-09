#ifndef SCREEN_H_
#define SCREEN_H_

#include <list>

#include "editor_enum.h"

class Cursor;

class Screen {
 public:
    Screen(std::list<std::list<char> >* text_list, EDITOR_MODE* mode);
    ~Screen();

    int print(Cursor* cursor);
    void setShowLineNumber(bool show_line_number) { this->show_line_number_ = show_line_number; }

 private:
    void printLineNumber(int line_number_width, int row_idx);
    void printLineNumberPadding(int line_number_width);
    void printPadding();

    std::list<std::list<char> >* text_list_;
    EDITOR_MODE* mode_;
    bool show_line_number_;
};

#endif  // SCREEN_H_
