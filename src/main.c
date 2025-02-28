#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdio.h>
#include <resource.h>

wchar_t winname[] = L"text editor (no longer asm)";
wchar_t *content;
LONG width = 800, height = 600;
HBRUSH bgbrush;
HWND txtbox;
HGDIOBJ font;

LRESULT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI HandleMenubar(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main(int argc, wchar_t* argv[]) {
    return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_HREDRAW, WndProc, 0, 0, hInstance, LoadImage(hInstance, L"ICON1", IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR), LoadCursor(NULL, IDC_ARROW), (HBRUSH)COLOR_WINDOW, NULL, winname, LoadImage(hInstance, L"ICON1", IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)};
    bgbrush = CreateSolidBrush(RGB(9, 1, 0xc));

    RegisterClassEx(&wc);
    hwnd = CreateWindowEx(0,
        winname, winname, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, width+16, height+59,
        NULL, LoadMenu(hInstance, MAKEINTRESOURCE(100)), hInstance, hInstance);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, hwnd, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            printf("%ls\n", winname);
            txtbox = CreateWindowEx(WS_EX_WINDOWEDGE,
            L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | DS_SETFONT | WS_BORDER,
                0, 0, width, height,
                hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            font = CreateFontW(0, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, EASTEUROPE_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_MODERN, L"Courier New\0\0");
            SendMessage(hwnd, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
            SendMessage(txtbox, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
        break;
        case WM_DESTROY:
            DeleteObject(hwnd);
            PostQuitMessage(0);
            ExitProcess(0);
        case WM_SIZE:
            width = LOWORD(lParam);
            height = HIWORD(lParam);
            wprintf(L"%d %d\n", width, height);

            SetWindowPos(txtbox, hwnd, 0, 0, width, height, SWP_NOZORDER);
        break;
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc;

            hdc = BeginPaint(hwnd, &ps);
            SelectObject(hdc, font);

            FillRect(hdc, &ps.rcPaint, bgbrush);
            EndPaint(hwnd, &ps);
        break;
        case WM_CTLCOLOREDIT:
            SetTextColor((HDC)wParam, RGB(255, 255, 255));
            SetBkColor((HDC)wParam, RGB(0x9, 0x1, 0xC));
        return (INT_PTR)CreateSolidBrush(RGB(0x9, 0x1, 0xC));
        case WM_COMMAND:
        return HandleMenubar(hwnd, uMsg, wParam, lParam);

        default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

LRESULT WINAPI HandleMenubar(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    WORD wmId = LOWORD(wParam);
    WORD wmEvent = HIWORD(wParam);

    switch (wmId) {
        case IDR_ABOUT:
            int len = GetWindowTextLength(txtbox);
            content = malloc(len+1);
            GetWindowText(txtbox, content, len+1);
            wprintf(L"%ls\n", content);
        break;
        case IDR_QUIT:
            DestroyWindow(hwnd);
        break;

        default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}