#include "file_manager.h"

#include <stdio.h>

#include "cursor.h"

FileManager::FileManager(const char* filepath, std::list<std::list<char> >* text_list)
    : filepath_(filepath),
    text_list_(text_list) {
}

FileManager::~FileManager() {
}


int FileManager::edit() {
    FILE* fp;
    std::list<char> temp;

    text_list_->clear();
    if (filepath_ == nullptr || (fp = fopen(filepath_, "r")) == nullptr) {
        text_list_->push_back(temp);

        return 0;
    }

    char c;
    while (fscanf(fp, "%c", &c) != EOF) {
        if (c == '\n' || c == '\r') {
            text_list_->push_back(temp);
            temp.clear();
        } else {
            temp.push_back(c);
        }
    }
    text_list_->push_back(temp);

    fclose(fp);

    return 0;
}

int FileManager::write() {
    FILE *fp;
    if (filepath_ == nullptr || (fp = fopen(filepath_, "w")) == nullptr) return 1;

    Cursor cursor(text_list_->begin(), text_list_->begin()->begin());
    for (cursor.setRow(text_list_->begin()); cursor.getRow() != text_list_->end(); cursor.incRow()) {
        if (cursor.getRow() != text_list_->begin()) putc('\n', fp);
        for (cursor.setCol(cursor.getRow()->begin()); cursor.getCol() != cursor.getRow()->end(); cursor.incCol()) {
            fputc(*cursor.getCol(), fp);
        }
    }

    fclose(fp);

    return 0;
}

int FileManager::changeFile(const char* filepath) {
    if (filepath == nullptr) return 1;

    filepath_ = filepath;

    return 0;
}
