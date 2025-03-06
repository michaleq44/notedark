#ifndef MENUBAR_H
#define MENUBAR_H
#include <main.h>

LRESULT WINAPI HandleMenubar(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI New(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI Open(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI Save(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI SaveAs(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL WINAPI Quit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
