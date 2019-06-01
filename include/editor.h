#ifndef __EDITOR__
#define __EDITOR__

#include <list>

class Cursor;

enum MODE {
    MODE_COMMAND = 0,
    MODE_INSERT,
    MODE_LAST_LINE,
    MODE_END
};

class Editor {
private:
	Cursor* cp;
	std::list<std::list<char> > text_list;
    MODE mode;

	const char* filepath;

public:
	Editor();
    Editor(const char* filepath);
	~Editor();

	int editNewFile();
	int editFile(const char* filepath);

	int writeFile();
	int writeFile(const char* filepath);

	int printEditor();

    int moveCursor(const char c);

	int insertChar(const char c);
	int deleteChar();
	int insertLine();
	int deleteLine();
	
    //setter
    void setMode(MODE mode) { this->mode = mode; }

	//getter
    MODE getMode() { return mode; }
	int getRowSize();
	const char* getFilepath();

};


#endif 
