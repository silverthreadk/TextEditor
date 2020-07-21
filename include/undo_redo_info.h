#ifndef UNDO_REDO_INFO_H_
#define UNDO_REDO_INFO_H_

struct UndoRedoInfo {
    char command;
    char ch;
    int row;
    int col;

    UndoRedoInfo(char command, char ch, int row, int col) : command(command), ch(ch), row(row), col(col) {
    }
};

#endif