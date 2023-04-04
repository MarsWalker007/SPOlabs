#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#define PASSWORD_LENGTH 16
#define TIMER_ID 1
#define ID_BUTTON 2
#define CHARACTERS "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;':\",./<>?"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void GeneratePassword(int *pressedKeys, int *mousePosX, int *mousePosY, char *password);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc = {0};
    HWND hwnd;
    MSG msg;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "Password Generator";
    wc.lpfnWndProc = WndProc;

    RegisterClass(&wc);
    hwnd = CreateWindow(wc.lpszClassName, "Password Generator", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static char password[PASSWORD_LENGTH + 1] = "";
    static int timerCount = 0;
    static int pressedKeys[PASSWORD_LENGTH];
    static int mousePosX[PASSWORD_LENGTH];
    static int mousePosY[PASSWORD_LENGTH];
    static int passwordGenerated = 0;
    static HWND hPasswordOutput;

    switch (msg)
    {
    case WM_CREATE:
    {
        hPasswordOutput = CreateWindow("STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
                                       10, 40, 200, 24, hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

        CreateWindow(TEXT("BUTTON"), TEXT("Generate Password"), WS_VISIBLE | WS_CHILD,
                     10, 10, 200, 24, hwnd, (HMENU)(INT_PTR)ID_BUTTON, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        SendMessage(hPasswordOutput, EM_SETSEL, 0, -1);
        break;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == ID_BUTTON)
        {
            if (!passwordGenerated)
            {
                SetTimer(hwnd, TIMER_ID, 1000, NULL);
                passwordGenerated = 1;
            }
            else
            {
                passwordGenerated = 0;
                timerCount = 0;
                memset(pressedKeys, 0, sizeof(pressedKeys));
                memset(mousePosX, 0, sizeof(mousePosX));
                memset(mousePosY, 0, sizeof(mousePosY));
                memset(password, 0, sizeof(password));
            }
        }
        break;
    }
    case WM_TIMER:
    {
        if (wParam == TIMER_ID)
        {
            timerCount++;
            if (timerCount == 5)
            {
                KillTimer(hwnd, TIMER_ID);
                passwordGenerated = 0;
                timerCount = 0;
                GeneratePassword(pressedKeys, mousePosX, mousePosY, password);
                SetWindowText(hPasswordOutput, password);
                MessageBox(hwnd, "Password generated!", "Success", MB_OK | MB_ICONINFORMATION);
                if (OpenClipboard(hwnd))
                {
                    EmptyClipboard();
                    HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, strlen(password) + 1);
                    char *pszText = (char *)GlobalLock(hClipboardData);
                    strcpy(pszText, password);
                    GlobalUnlock(hClipboardData);
                    SetClipboardData(CF_TEXT, hClipboardData);
                    CloseClipboard();
                }
            }
        }
        break;
    }
    case WM_KEYDOWN:
    {
        if (passwordGenerated)
        {
            for (int i = 0; i < PASSWORD_LENGTH; i++)
            {
                if (pressedKeys[i] == 0)
                {
                    pressedKeys[i] = wParam;
                    break;
                }
            }
        }
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (passwordGenerated)
        {
            for (int i = 0; i < PASSWORD_LENGTH; i++)
            {
                if (mousePosX[i] == 0 && mousePosY[i] == 0)
                {
                    mousePosX[i] = LOWORD(lParam);
                    mousePosY[i] = HIWORD(lParam);
                    break;
                }
            }
        }
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    default:
    {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    }
    return 0;
}

void GeneratePassword(int *pressedKeys, int *mousePosX, int *mousePosY, char *password)
{
    srand(time(NULL));
    int index = 0;
    while (index < PASSWORD_LENGTH)
    {
        if (pressedKeys[index] != 0)
        {
            password[index] = CHARACTERS[pressedKeys[index] % strlen(CHARACTERS)];
            pressedKeys[index] = 0;
            index++;
        }
        else if (mousePosX[index] != 0 && mousePosY[index] != 0)
        {
            password[index] = CHARACTERS[mousePosX[index] % strlen(CHARACTERS)];
            index++;
            password[index] = CHARACTERS[mousePosY[index] % strlen(CHARACTERS)];
            index++;
            mousePosX[index - 2] = 0;
            mousePosY[index - 1] = 0;
        }
        else
        {
            password[index] = CHARACTERS[rand() % strlen(CHARACTERS)];
            index++;
        }
    }
    password[PASSWORD_LENGTH] = '\0';
}