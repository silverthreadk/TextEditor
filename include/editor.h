#ifndef EDITOR_H_
#define EDITOR_H_

#include <list>
#include <memory>
#include <stack>

#include "editor_enum.h"
#include "undo_redo_info.h"

class FileManager;
class Cursor;
class Screen;

class Editor {
 public:

    explicit Editor(const char* filepath);

    ~Editor();

    /**
     * Edit the file in the file path.
     * @return          0 if the operation succeeded
     */
    int editFile();

    /**
     * Edit the file in a specific file path.
     * @return          0 if the operation succeeded
     */
    int editFile(const char* filepath);

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
    int printEditor() const;

    /**
     * Move the cursor one character to the left
     * @return          0 if the operation succeeded
     */
    int moveCursorToLeft();

    /**
     * Move the cursor one character to the right
     * @return          0 if the operation succeeded
     */
    int moveCursorToRight();

    /**
     * Move the cursor up one line
     * @return          0 if the operation succeeded
     */
    int moveCursorToUp();

    /**
     * Move the cursor up one line
     * @return          0 if the operation succeeded
     */
    int moveCursorToDown();

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

    /*
     * Move cursor to the specified line.
     * @param n         n th line
     * @return          0 if the operation succeeded
     */
    int moveCursorToSpecifiedLine(const int n);

    /*
     * Move cursor to the right one word.
     * @return          0 if the operation succeeded
     */
    int moveCursorToRightOneWord();

    /*
     * Move cursor to the left one word.
     * @return          0 if the operation succeeded
     */
    int moveCursorToLeftOneWord();

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
     * Delete character before current cursor position.
     * @return          0 if the operation succeeded
     */
    int deleteCharBefore();

    /**
     * Delete word from current cursor position.
     * @return          0 if the operation succeeded
     */
    int deleteWord();

    /**
     * Delete word before current cursor position.
     * @return          0 if the operation succeeded
     */
    int deleteWordBefore();

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

    /*
     * Delete character from beginning of line to cursor.
     * @return          0 if the operation succeeded
     */
    int deleteToBeginningOfLine();

    /*
     * Delete character from cursor to end of line.
     * @return          0 if the operation succeeded
     */
    int deleteToEndOfLine();

    /*
     * Undo last change
     * @return          0 if the operation succeeded
     */
    int undo();

    void setMode(const EDITOR_MODE mode) { this->mode_ = mode; }

    EDITOR_MODE getMode() const { return mode_; }

    void setShowLineNumber(const bool show_line_number);

 private:
    std::unique_ptr<FileManager> file_manager_;
    std::unique_ptr<Cursor> cursor_;
    std::unique_ptr<Screen> screen_;
    std::list<std::list<char> > text_list_;
    std::stack<UndoRedoInfo> undo_;
    EDITOR_MODE mode_;
};


#endif  // EDITOR_H_
