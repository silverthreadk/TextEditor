#ifndef __EDITOR__
#define __EDITOR__

#include <list>

class Cursor;


class Editor {
private:
	Cursor* cp;	//현재 커서 정보
	std::list<std::list<char> > text_list;	//작성된 문자 리스트

	const char* filepath;	//현재 파일 경로

public:
	Editor();
	~Editor();

	//실행 관련 함수
	int editNewFile();
	int editFile(const char* filepath);

	//file 읽기/저장 관련 함수
	int readFile(const char* filepath);
	int writeFile();
	int writeFile(const char* filepath);

	//편집 화면 출력 함수
	int printEditor();

	//편집 관련 함수
	int insertChar(const char c);
	int deleteChar();
	int insertLine();
	int deleteLine();
	int backspace();
	
	//getter
	Cursor* getCursor();
	int getRowSize();
	const char* getFilepath();

};


#endif 
