#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winuser.h>

#define ID_EDIT 1
#define ID_FILE_NEW 2
#define ID_FILE_OPEN 3
#define ID_FILE_SAVE 4
#define ID_EDIT_COPY 5
#define ID_EDIT_PASTE 6
#define ID_EDIT_CUT 7
#define ID_HELP_ABOUT 8

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void OnFileNew(HWND hWnd);
void OnFileOpen(HWND hWnd);
void OnFileSave(HWND hWnd);
void OnEditCopy(HWND hWnd);
void OnEditPaste(HWND hWnd);
void OnEditCut(HWND hWnd);
void OnHelpAbout(HWND hWnd);

HWND hEdit;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = "MainWndClass";
    if (!RegisterClassEx(&wcex))
    {
        return -1;
    }

    hWnd = CreateWindow("MainWndClass", "Word no ne word",
                        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                        800, 600, NULL, NULL, hInstance, NULL);
    if (!hWnd)
    {
        return -1;
    }

    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hEditMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, "Edit");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, "Help");

    AppendMenu(hFileMenu, MF_STRING, ID_FILE_NEW, "New");
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_OPEN, "Open");
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_SAVE, "Save");

    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_COPY, "Copy");
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_PASTE, "Paste");
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_CUT, "Cut");

    AppendMenu(hHelpMenu, MF_STRING, ID_HELP_ABOUT, "About");

    SetMenu(hWnd, hMenu);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_NEW:
            OnFileNew(hWnd);
            break;
        case ID_FILE_OPEN:
            OnFileOpen(hWnd);
            break;
        case ID_FILE_SAVE:
            OnFileSave(hWnd);
            break;
        case ID_EDIT_COPY:
            OnEditCopy(hWnd);
            break;
        case ID_EDIT_PASTE:
            OnEditPaste(hWnd);
            break;
        case ID_EDIT_CUT:
            OnEditCut(hWnd);
            break;
        case ID_HELP_ABOUT:
            OnHelpAbout(hWnd);
            break;
        }
        break;
    case WM_CREATE:
    {
        HWND hwndEdit = CreateWindowEx(
            WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
            0, 0, 0, 0, hWnd, (HMENU)ID_EDIT, GetModuleHandle(NULL), NULL);
        if (hwndEdit == NULL)
        {
            MessageBox(hWnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
        }

        LOGFONT lf = {0};
        lf.lfCharSet = RUSSIAN_CHARSET;
        lf.lfHeight = 16;
        HFONT hFont = CreateFontIndirect(&lf);
        SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    }
        return 0;

    case WM_SIZE:
    {
        HWND hwndEdit = GetDlgItem(hWnd, ID_EDIT);
        if (hwndEdit != NULL)
        {
            SetWindowPos(hwndEdit, NULL, 0, 0, LOWORD(lParam), HIWORD(lParam), SWP_NOZORDER);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void OnFileNew(HWND hWnd)
{
    HWND hwndEdit = GetDlgItem(hWnd, ID_EDIT);

    SetWindowText(hwndEdit, "");
}

void OnFileOpen(HWND hWnd)
{
    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            HWND hwndEdit = GetDlgItem(hWnd, ID_EDIT);

            SetWindowText(hwndEdit, "");

            char *buffer = (char *)malloc(MAX_PATH);

            DWORD bytesRead;
            while (ReadFile(hFile, buffer, MAX_PATH, &bytesRead, NULL) == TRUE && bytesRead > 0)
            {
                buffer[bytesRead] = '\0';
                SendMessage(hwndEdit, EM_REPLACESEL, 0, (LPARAM)buffer);
            }

            free(buffer);

            CloseHandle(hFile);

            MessageBox(hWnd, "File created", "Open", MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            MessageBox(hWnd, "File not opened", "Error", MB_OK | MB_ICONERROR);
        }
    }
}

void OnFileSave(HWND hWnd)
{
    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "*.txt";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

    if (GetSaveFileName(&ofn) == TRUE)
    {
        HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            HWND hwndEdit = GetDlgItem(hWnd, ID_EDIT);

            int len = GetWindowTextLength(hwndEdit);

            char *buffer = (char *)malloc(len + 1);

            GetWindowText(hwndEdit, buffer, len + 1);

            DWORD bytesWritten;
            if (WriteFile(hFile, buffer, len, &bytesWritten, NULL) == TRUE)
            {
                MessageBox(hWnd, "File saved", "Save", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                MessageBox(hWnd, "Can't save", "Error", MB_OK | MB_ICONERROR);
            }

            free(buffer);

            CloseHandle(hFile);
        }
        else
        {
            MessageBox(hWnd, "Failed to create a file to save", "Error", MB_OK | MB_ICONERROR);
        }
    }
}

void OnEditCopy(HWND hWnd)
{
    HWND hwndEdit = GetDlgItem(hWnd, ID_EDIT);

    int start, end;
    SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
    int length = end - start;
    if (length > 0)
    {
        char *buffer = (char *)malloc(length + 1);

        SendMessage(hwndEdit, WM_GETTEXT, 0, (LPARAM)buffer);
        buffer[length] = '\0';

        if (OpenClipboard(hWnd))
        {
            EmptyClipboard();
            HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, length + 1);
            if (hglbCopy != NULL)
            {
                LPSTR lpstrCopy = (LPSTR)GlobalLock(hglbCopy);
                if (lpstrCopy != NULL)
                {
                    lstrcpy(lpstrCopy, buffer);
                    GlobalUnlock(hglbCopy);
                    SetClipboardData(CF_TEXT, hglbCopy);
                }
            }
            CloseClipboard();
        }

        free(buffer);
    }
}

void OnEditPaste(HWND hWnd)
{
    HWND hwndEdit = GetDlgItem(hWnd, ID_EDIT);

    if (OpenClipboard(hWnd))
    {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData != NULL)
        {
            char *pData = (char *)GlobalLock(hData);
            if (pData != NULL)
            {
                SendMessage(hwndEdit, EM_REPLACESEL, TRUE, (LPARAM)pData);
                GlobalUnlock(hData);
            }
        }
        CloseClipboard();
    }
}

void OnEditCut(HWND hWnd)
{
    HWND hwndEdit = GetDlgItem(hWnd, ID_EDIT);

    SendMessage(hwndEdit, EM_SETSEL, 0, -1);

    SendMessage(hwndEdit, WM_CUT, 0, 0);
}

void OnHelpAbout(HWND hWnd)
{
    MessageBox(hWnd, "Blyat na sho ya zhmav\n Uhadi", "About", MB_OK | MB_ICONINFORMATION);
}