#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>
#include <vector>

using namespace std;

DWORD FindAdress(HANDLE procHandle, DWORD Offsets[], DWORD BaseAddress);

void WriteToMemory(HANDLE procHandle);

string string_format(const string fmt, ...);


string windowName = "AssaultCube";
LPCSTR lWindowName = "AssaultCube";
string gameStatus = "";

bool gameAvailable = false;
bool updateOnNextCycle = true;

struct HackableValue {
    bool status;
    vector<BYTE> value;
    DWORD baseAddress;
    vector<DWORD> offsets;

    string toString() {
        return string_format("status: %s\nvalue: %s\nbaseAddress: %s\noffsets: %s\n",
                             (status ? "true" : "false"),
                             value,
                             baseAddress,
                             offsets);
    }
};

HackableValue ammo = {
        false,
        {0xA3, 0x1C, 0x0, 0x0},
        0x004DF73C,
        {0x378, 0x14, 0x0}
};

HackableValue health = {
        false,
        {0x39, 0x5, 0x0, 0x0},
        0x004DF73C,
        {0xF4}
};


int main() {
    cout << "Hello, World!" << endl;
    cout << "ammo:" << endl << ammo.toString() << endl;
    cout << "health:" << endl << health.toString() << endl;

    HWND gameWindow = NULL;
    int timeSinceLAstUpdate = clock();
    int gameAvailableTimer = clock();
    int pressTimer = clock();

    DWORD procId = NULL;

    return 0;
}


string string_format(const std::string fmt, ...) {
    int size = ((int) fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *) str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}
