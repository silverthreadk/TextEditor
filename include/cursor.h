#ifndef CURSOR_H_
#define CURSOR_H_

#include <list>

class Cursor {
 public:
    Cursor(std::list<std::list<char> >::iterator row, std::list<char>::iterator col);

    ~Cursor();

    /**
     * Move the cursor one character to the left
     * @return          0 if the operation succeeded
     */
    int moveToLeft();

    /**
     * Move the cursor one character to the right
     * @return          0 if the operation succeeded
     */
    int moveToRight(const bool is_insert_mode = false);

    /**
     * Move the cursor up one line
     * @return          0 if the operation succeeded
     */
    int moveToUp();

    /**
     * Move the cursor up one line
     * @return          0 if the operation succeeded
     */
    int moveToDown(const int row_size);

    /**
     * Move cursor to the beginning of current line.
     * @return          0 if the operation succeeded
     */
    int moveToBeginning();

    /**
     * Move cursor to the end of current line
     * @return          0 if the operation succeeded
     */
    int moveToEnd();

    void scrollUp();

    void scrollDown();

    void scrollTo();

    std::list<std::list<char> >::iterator getRow() const;

    std::list<char>::iterator getCol() const;

    int getRowIndex() const;

    int getColIndex() const;

    std::pair<std::list<std::list<char> >::iterator, int> getScrollPosition() const {
        return make_pair(scroll_position_, scroll_position_idx_);
    }

    void setRow(const std::list<std::list<char> >::iterator r);

    void setCol(const std::list<char>::iterator c);

    void setRowIndex(const int idx);

    void setColIndex(const int idx);

    void setScrollPosition(std::list<std::list<char> >::iterator visible_top_row) {
        this->scroll_position_ = visible_top_row;
    }

    void incRow();

    void incCol();

    void incRowIndex();

    void incColIndex();

    void incScrollPositionIndex() {
        if (row_idx_ == scroll_position_idx_) ++scroll_position_idx_;
    }

    void decRow();

    void decCol();

    void decRowIndex();

    void decColIndex();

 private:
    void moveCol(const int n);

    std::list<std::list<char> >::iterator row_;
    std::list<char>::iterator col_;

    int row_idx_;
    int col_idx_;

    std::list<std::list<char> >::iterator scroll_position_;
    int scroll_position_idx_;
};


#endif  // CURSOR_H_
