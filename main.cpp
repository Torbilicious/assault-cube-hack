#include <iostream>
#include <Windows.h>
#include <ctime>
#include <vector>

using namespace std;

DWORD FindAdress(HANDLE procHandle, vector<DWORD> offsets, DWORD baseAddress);

void WriteToMemory(HANDLE procHandle);

string getString(bool in);

void printStatus();

string windowName = "AssaultCube";
LPCSTR lWindowName = "AssaultCube";
string gameStatus = "";

bool gameAvailable = false;
bool updateOnNextCycle = true;

struct HackableValue {
    string name;
    bool status;
    pair<int, string> key;
    vector<BYTE> value;
    DWORD baseAddress;
    vector<DWORD> offsets;
};

vector<HackableValue> hacks;

HackableValue ammo = {
        "Unlimited Ammo",
        false,
        {VK_F1, "F1"},
        {0xA3, 0x1C, 0x0, 0x0},
        0x004DF73C,
        {0x378, 0x14, 0x0}
};

HackableValue health = {
        "Unlimited Health",
        false,
        {VK_F2, "F2"},
        {0x39, 0x5, 0x0, 0x0},
        0x004DF73C,
        {0xF4}
};


int main() {

    hacks.push_back(ammo);
    hacks.push_back(health);

    HWND gameWindow = NULL;
    int timeSinceLastUpdate = clock();
    int gameAvailableTimer = clock();
    int pressTimer = clock();

    DWORD procId = NULL;
    HANDLE procHandle = NULL;

    while (!GetAsyncKeyState(VK_INSERT)) {
        if (clock() - gameAvailableTimer > 100) {
            gameAvailableTimer = clock();
            gameAvailable = false;

            gameWindow = FindWindowA(NULL, lWindowName);
            if (gameWindow) {
                GetWindowThreadProcessId(gameWindow, &procId);
                if (procId != 0) {
                    procHandle = OpenProcess(PROCESS_ALL_ACCESS, false, procId);
                    if (procHandle == INVALID_HANDLE_VALUE || procHandle == NULL) {
                        gameStatus = "Failed to open Process for valid handle";
                    } else {
                        gameStatus = windowName + " is ready to be hacked";
                        gameAvailable = true;
                    }
                } else {
                    gameStatus = "Failed to get process Id";
                }
            } else {
                gameStatus = windowName + ": NOT FOUND";
            }

            if (updateOnNextCycle || clock() - timeSinceLastUpdate > 5000) {
                system("cls");
                printStatus();
                updateOnNextCycle = false;
                timeSinceLastUpdate = clock();
            }

            if (gameAvailable) {
                WriteToMemory(procHandle);
            }
        }

        if (clock() - pressTimer > 400) {
            if (gameAvailable) {
                for (HackableValue &hack : hacks) {
                    if (GetAsyncKeyState(hack.key.first)) {
                        pressTimer = clock();
                        hack.status = !hack.status;
                        updateOnNextCycle = true;
                    }
                }
            }
        }
    }

    return 0;
}

DWORD FindAdress(HANDLE procHandle, vector<DWORD> offsets, DWORD baseAddress) {
    DWORD pointer = baseAddress;
    DWORD temp = 0;
    DWORD pointerAddress = 0;

    for (int i = 0; i < offsets.size(); i++) {
        if (i == 0) {
            ReadProcessMemory(procHandle, (LPCVOID) pointer, &temp, sizeof(temp), NULL);
        }

        pointerAddress = temp + offsets[i];
        ReadProcessMemory(procHandle, (LPCVOID) pointerAddress, &temp, sizeof(temp), NULL);
    }

    return pointerAddress;
}


void WriteToMemory(HANDLE procHandle) {
    DWORD addressToWrite;

    for (HackableValue &hack : hacks) {
        addressToWrite = FindAdress(procHandle, hack.offsets, hack.baseAddress);
        WriteProcessMemory(procHandle, (BYTE *) addressToWrite, &hack.value, sizeof(hack.value), NULL);
    }
}

void printStatus() {
    cout << "------------------------------------------------------------" << endl;
    cout << "                 AssaultCube memory hacker                  " << endl;
    cout << "------------------------------------------------------------" << endl << endl;
    cout << "Game status: " << gameStatus << endl << endl;

    for (HackableValue &hack : hacks) {
        cout << "[" << hack.key.second << "] " << hack.name << " -> " << getString(hack.status) << endl << endl;
    }

    cout << "[INSERT] Exit" << endl << endl;
}

string getString(bool in) {
    return in ? "ON" : "OFF";
}
