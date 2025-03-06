#ifndef MAIN_H
#define MAIN_H

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdio.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <resource.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <menubar.h>

LRESULT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
