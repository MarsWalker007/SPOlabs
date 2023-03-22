#include <Windows.h>
#include <stdio.h>
#include <time.h>

#define ID_GENERATE_BUTTON 1
#define ID_PASSWORD_EDIT 2

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void GeneratePassword(HWND hwnd)
{
    const int PASSWORD_LENGTH = 12;
    char password[13] = "";

    const char SYMBOLS[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-={}[]\\|;:'\",.<>/?";
    const int SYMBOLS_COUNT = strlen(SYMBOLS);

    srand(time(NULL));

    for (int i = 0; i < PASSWORD_LENGTH; i++)
    {
        password[i] = SYMBOLS[rand() % SYMBOLS_COUNT];
    }

    SetWindowText(GetDlgItem(hwnd, ID_PASSWORD_EDIT), password);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char *CLASS_NAME = "PasswordGeneratorClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(CLASS_NAME, "Password Generator", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);

    HWND generateButton = CreateWindow("BUTTON", "Generate", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                       140, 100, 100, 24, hwnd, (HMENU)ID_GENERATE_BUTTON, hInstance, NULL);

    HWND passwordEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_AUTOHSCROLL,
                                     20, 50, 350, 24, hwnd, (HMENU)ID_PASSWORD_EDIT, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_GENERATE_BUTTON:
            GeneratePassword(hwnd);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
