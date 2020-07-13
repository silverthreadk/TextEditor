#ifndef SCREEN_H_
#define SCREEN_H_

#include <list>

#include "editor_enum.h"

class Cursor;

class Screen {
 public:
    Screen(std::list<std::list<char> >* text_list, EDITOR_MODE* mode);
    ~Screen();

    int print(Cursor* cursor) const;
    void setShowLineNumber(const bool show_line_number) { this->show_line_number_ = show_line_number; }

 private:
    void printLineNumber(const int line_number_width, const int row_idx) const;
    void printLineNumberPadding(const int line_number_width) const;
    void printPadding() const;

    std::list<std::list<char> >* text_list_;
    EDITOR_MODE* mode_;
    bool show_line_number_;
};

#endif  // SCREEN_H_
