#ifndef __CURSOR__
#define __CURSOR__

#include <list>

using namespace std;

class Cursor {
private:
	list<list<char> >::iterator row;
	list<char>::iterator col;

	int row_idx;
	int col_idx;

    list<list<char> >::iterator scroll_position;
    int scroll_position_idx;

public:
	Cursor(list<list<char> >::iterator row, list<char>::iterator col);
	~Cursor();

	int move(const char command, const int row_size, const bool is_insert_mode = false);

    void scrollUp();
    void scrollDown();


	//getter
	list<list<char> >::iterator getRow();
	list<char>::iterator getCol();
	int getRowIndex();
	int getColIndex();

    pair<list<list<char> >::iterator, int> getScrollPosition() {
        return make_pair(scroll_position, scroll_position_idx);
    }

	//setter
	void setRow(const list<list<char> >::iterator r);
	void setCol(const list<char>::iterator c);
	void setRowIndex(const int idx);
	void setColIndex(const int idx);

    void setScrollPosition(list<list<char> >::iterator visible_top_row) {
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
