#ifndef __EDITOR__
#define __EDITOR__

#include <list>

class Cursor;

enum MODE {
    MODE_COMMAND = 0,
    MODE_INSERT,
    MODE_LAST_LINE,
    MODE_END,
    MODE_SAVE
};

class Editor {
private:
	Cursor* cp;	//���� Ŀ�� ����
	std::list<std::list<char> > text_list;	//�ۼ��� ���� ����Ʈ
    MODE mode;

	const char* filepath;	//���� ���� ���

public:
	Editor();
	~Editor();

	//���� ���� �Լ�
	int editNewFile();
	int editFile(const char* filepath);

	//file �б�/���� ���� �Լ�
	int readFile(const char* filepath);
	int writeFile();
	int writeFile(const char* filepath);

	//���� ȭ�� ��� �Լ�
	int printEditor();

	//���� ���� �Լ�
	int insertChar(const char c);
	int deleteChar();
	int insertLine();
	int deleteLine();
	int backspace();
	
    //setter
    void setMode(MODE mode) { this->mode = mode; }

	//getter
    MODE getMode() { return mode; }
	Cursor* getCursor();
	int getRowSize();
	const char* getFilepath();

};


#endif 
