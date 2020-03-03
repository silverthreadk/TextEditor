#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <list>

class FileManager {
 public:
    FileManager(const char* filepath, std::list<std::list<char> >* text_list);
    ~FileManager();

    int edit();
    int write();
    int changeFile(const char* filepath);

 private:
    const char* filepath_;
    std::list<std::list<char> >* text_list_;
};

#endif  // FILE_MANAGER_H_
