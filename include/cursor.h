#ifndef __CURSOR__
#define __CURSOR__

#include <list>

class Cursor {
private:
    std::list<std::list<char> >::iterator row;
    std::list<char>::iterator col;

    int row_idx;
    int col_idx;

    std::list<std::list<char> >::iterator scroll_position;
    int scroll_position_idx;

public:
    Cursor(std::list<std::list<char> >::iterator row, std::list<char>::iterator col);
    ~Cursor();

    int move(const char command, const int row_size, const bool is_insert_mode = false);

    void scrollUp();
    void scrollDown();


    //getter
    std::list<std::list<char> >::iterator getRow();
    std::list<char>::iterator getCol();
    int getRowIndex();
    int getColIndex();

    std::pair<std::list<std::list<char> >::iterator, int> getScrollPosition() {
        return make_pair(scroll_position, scroll_position_idx);
    }

    //setter
    void setRow(const std::list<std::list<char> >::iterator r);
    void setCol(const std::list<char>::iterator c);
    void setRowIndex(const int idx);
    void setColIndex(const int idx);

    void setScrollPosition(std::list<std::list<char> >::iterator visible_top_row) {
        this->scroll_position = visible_top_row;
    }

    //increase
    void incRow();
    void incCol();
    void incRowIndex();
    void incColIndex();

    //decrease
    void decRow();
    void decCol();
    void decRowIndex();
    void decColIndex();

private:
    void moveCol(const int n);
};


#endif 
