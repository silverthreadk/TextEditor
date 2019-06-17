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
    bool line_number;

	const char* filepath;

public:
    Editor(const char* filepath);
	~Editor();

    /**
     * Edit the file in the file path.
     * @return          0 if the operation succeeded
     */
	int editFile();

    /**
     * Write the file in the file path.
     * @return          0 if the operation succeeded
     */
	int writeFile();

    /**
     * Write the file in a specific file path.
     * @param filepath  specific file path to write
     * @return          0 if the operation succeeded
     */
	int writeFile(const char* filepath);

    /**
     * Prints the current file.
     * @return          0 if the operation succeeded
     */
	int printEditor();

    /**
     * Move around using cursor key.
     * Use the cursor keys, "h" to go left, "j" to go down, "k" to go up, "l" to go right.
     * @param c         direction to move cursor
     * @return          0 if the operation succeeded
     */
    int moveCursor(const char c);

    /**
     * Move cursor to the beginning of current line.
     * @return          0 if the operation succeeded
     */
    int moveCursorToBeginning();

    /**
     * Move cursor to the end of current line
     * @return          0 if the operation succeeded
     */
    int moveCursorToEnd();

    /**
     * Insert character before the current cursor position.
     * @param c         character to append
     * @return          0 if the operation succeeded
     */
    int insertChar(const char c);

    /**
     * Delete character following current cursor position.
     * @return          0 if the operation succeeded
     */
	int deleteChar();

    /**
     * Insert new line following the current line and add character there.
     * @return          0 if the operation succeeded
     */
	int insertLine();

    /**
     * Insert new line in front of the current line and add text there
     * @return          0 if the operation succeeded
     */
    int insertLineBefore();

    /**
     * Delete the current line.
     * @return          0 if the operation succeeded
     */
	int deleteLine();
	
    //setter
    void setMode(MODE mode) { this->mode = mode; }
    void setLineNumber(bool line_number) { this->line_number = line_number; }

	//getter
    MODE getMode() { return mode; }

};


#endif 
