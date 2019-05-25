#ifndef __CURSOR__
#define __CURSOR__

#include <list>

#define SCROLL_UP 0
#define SCROLL_DOWN 19
#define SCROLL_LEFT 0
#define SCROLL_RIGHT 69

using namespace std;

class Cursor {
private:
	list<list<char> >::iterator row;	//���� Ŀ���� ���� ��Ÿ���� list iterator
	list<char>::iterator col;	//���� Ŀ���� ���� ��Ÿ���� list iterator

	int row_idx;	//���� Ŀ���� ���� ��Ÿ���� �ε���
	int col_idx;	//���� Ŀ���� ���� ��Ÿ���� �ε���

	int scroll_up;	//���� ȭ���� ���� ��谪
	int scroll_down;	//���� ȭ���� �Ʒ��� ��谪
	int scroll_left;	//���� ȭ���� ���� ��谪
	int scroll_right;	//���� ȭ���� ������ ��谪


public:
	Cursor();
	~Cursor();

	int moveCursor(const char command, const int row_size);
	int moveRow(int n, const int row_size);
	int moveCol(int n);

	//getter
	list<list<char> >::iterator getRow();
	list<char>::iterator getCol();
	int getRowIndex();
	int getColIndex();
	int getScrollUp();
	int getScrollDown();
	int getScrollLeft();
	int getScrollRight();

	//setter
	void setRow(const list<list<char> >::iterator r);
	void setCol(const list<char>::iterator c);
	void setRowIndex(const int idx);
	void setColIndex(const int idx);
	void setScrollUp(const int value);
	void setScrollDown(const int value);
	void setScrollLeft(const int value);
	void setScrollRight(const int value);

	//increase
	void incRow();
	void incCol();
	void incRowIndex();
	void incColIndex();
	void incScrollUp();
	void incScrollDown();
	void incScrollLeft();
	void incScrollRight();

	//decrease
	void decRow();
	void decCol();
	void decRowIndex();
	void decColIndex();
	void decScrollUp();
	void decScrollDown();
	void decScrollLeft();
	void decScrollRight();
};


#endif 
