#ifndef SCREEN_H_
#define SCREEN_H_

#include <list>

#include "editor_enum.h"

class Cursor;

class Screen {
 public:
    Screen(std::list<std::list<char> >* text_list, Cursor** cursor, EDITOR_MODE* mode);
    ~Screen();

    int print();
    void setShowLineNumber(bool show_line_number) { this->show_line_number_ = show_line_number; }

 private:
    std::list<std::list<char> >* text_list_;
    Cursor** cursor_;
    EDITOR_MODE* mode_;
    bool show_line_number_;
};

#endif  // SCREEN_H_
