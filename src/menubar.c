#include <menubar.h>

extern WCHAR filename[32767];
extern LPWSTR content;
extern HWND txtbox;
extern BOOL saved;
extern HANDLE hout;
extern LPWSTR txt;

LRESULT WINAPI HandleMenubar(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    WORD wmId = LOWORD(wParam);
    WORD wmEvent = HIWORD(wParam);

    switch (wmId) {
        case IDR_NEW:
            return New(hwnd, uMsg, wParam, lParam);
        case IDR_OPEN:
            return Open(hwnd, uMsg, wParam, lParam);
        case IDR_SAVE:
            return Save(hwnd, uMsg, wParam, lParam);
        case IDR_SAVEAS:
            return SaveAs(hwnd, uMsg, wParam, lParam);
        case IDR_QUIT:
            CloseWindow(hwnd);

        case IDR_ABOUT:
            MessageBox(hwnd, L"notedark is basically windows notepad but worse (and in dark mode) so it's written in C using windows API (and a bit of stdio)", L"About", MB_OK | MB_ICONINFORMATION);
        break;

        default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT WINAPI New(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    int len = GetWindowTextLength(txtbox);
    free(txt);
    txt = malloc(len+1);

    GetWindowText(txtbox, txt, len+1);
    BOOL doireallyquit = TRUE;
    if (lstrcmpW(content, txt)) {
        int resp = MessageBox(hwnd, L"You have unsaved changes. Do you want to save now?", L"Unsaved changes", MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON3);
        if (resp == IDYES) Save(hwnd, msg, wParam, lParam);
        else if (resp == IDCANCEL) doireallyquit = FALSE;
    }
    free(txt);

    if (!doireallyquit) return 1;
    SetWindowText(txtbox, NULL);
    len = GetWindowTextLength(txtbox);
    free(content);
    content = malloc(len+1);

    GetWindowText(txtbox, content, len+1);
    saved = FALSE;
    
    memset(filename, 0, 32767);

    printf("created new file\n");

    return 0;
}

LRESULT WINAPI Save(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (saved) {
        int len = GetWindowTextLength(txtbox);
        content = malloc(len+1);
        
        char* s = malloc(2*(len+1));
        GetWindowText(txtbox, content, len+1);

        DWORD l = WideCharToMultiByte(CP_UTF8, 0, content, len+1, s, 2*(len+1), NULL, NULL);

        FILE *f = _wfopen(filename, L"w");
        if (!f) {
            MessageBox(hwnd, L"Could not open file", L"Error", MB_OK | MB_ICONERROR);
            return 1;
        }
        fwrite(s, l-1, 1, f);
        fclose(f);

        free(s);
        printf("saved file: ");
        WriteConsoleW(hout, filename, lstrlenW(filename), NULL, NULL);
        printf("\n");
    } else return SaveAs(hwnd, msg, wParam, lParam);

    return 0;
}

LRESULT WINAPI SaveAs(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    OPENFILENAMEW ofn = {0};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = L"All Files\0*.*\0\0";
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = 32767;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 8;
    ofn.lpstrDefExt = NULL;
    ofn.lCustData = 0;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;

    if (GetSaveFileName(&ofn)) {
        wprintf(L"saved as: ");
        WriteConsoleW(hout, ofn.lpstrFile, lstrlenW(ofn.lpstrFile), NULL, NULL);
        wprintf(L"\n");
        saved = TRUE;
        Save(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT WINAPI Open(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    int len = GetWindowTextLength(txtbox);
    free(txt);
    txt = malloc(len+1);

    GetWindowText(txtbox, txt, len+1);
    BOOL doireallyquit = TRUE;
    if (len) {
        int resp = MessageBox(hwnd, L"You have unsaved changes. Do you want to save now?", L"Unsaved changes", MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON3);
        if (resp == IDYES) Save(hwnd, msg, wParam, lParam);
        else if (resp == IDCANCEL) doireallyquit = FALSE;
    }
    free(txt);
    if (!doireallyquit) return 1;

    OPENFILENAMEW ofn = {0};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = L"All Files\0*.*\0\0";
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = 32767;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 8;
    ofn.lpstrDefExt = NULL;
    ofn.lCustData = 0;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;

    if (GetOpenFileName(&ofn)) {
        wprintf(L"opened: ");
        WriteConsoleW(hout, ofn.lpstrFile, lstrlenW(ofn.lpstrFile), NULL, NULL);
        wprintf(L"\n");
        saved = TRUE;

        FILE *f = _wfopen(filename, L"r");
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        fseek(f, 0, SEEK_SET);

        free(content);
        char *s = malloc(len+1);
        fread(s, len, 1, f);

        content = malloc(len+1);
        
        MultiByteToWideChar(CP_UTF8, 0, s, len+1, content, len+1);

        free(s);

        fclose(f);
        lstrcatW(content, L"\0");
        SetWindowText(txtbox, content);
        return 0;
    }

    return 1;
}

BOOL WINAPI Quit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    int len = GetWindowTextLength(txtbox);
    free(txt);
    txt = malloc(len+1);

    GetWindowText(txtbox, txt, len+1);
    BOOL doireallyquit = TRUE;
    if (lstrcmpW(content, txt)) {
        int resp = MessageBox(hwnd, L"You have unsaved changes. Do you want to save now?", L"Unsaved changes", MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON3);
        if (resp == IDYES) Save(hwnd, msg, wParam, lParam);
        else if (resp == IDCANCEL) doireallyquit = FALSE;
    }
    free(txt);

    if (doireallyquit) DeleteObject(hwnd);

    return doireallyquit;
}