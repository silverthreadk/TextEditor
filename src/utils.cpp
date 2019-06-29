#include "utils.h"

#include <stdlib.h>

#ifdef WINDOWS
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

void clearScreen() {
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

int getScreenSize() {
#ifdef WINDOWS
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return rows;
#else
    struct winsize max;
    ioctl(0, TIOCGWINSZ, &max);
    return max;
#endif
}

int getConsoleCursor() {
#ifdef WINDOWS
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y;
#else
    return 20;
#endif
}

bool isDigit(std::string str) {
    return atoi(str.c_str()) != 0 || str.compare("0") == 0;
}
