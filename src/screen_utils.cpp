#include "screen_utils.h"

#include <stdlib.h>

#ifdef WINDOWS
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

#include <regex>

void clearScreen() {
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

std::pair<int, int> getScreenSize() {
#ifdef WINDOWS
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    const int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    const int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return { rows, cols };
#else
    struct winsize max;
    ioctl(0, TIOCGWINSZ, &max);
    return { max.ws_row, max.ws_col };
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
