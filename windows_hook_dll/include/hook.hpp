#pragma once

#include <Windows.h>

#include <functional>

#ifdef windows_hook_EXPORTS
#define HOOK_API __declspec(dllexport)
#else
#define HOOK_API __declspec(dllimport)
#endif

using callback = std::function<void(WPARAM wParam, LPARAM lParam)>;

extern "C" HOOK_API void register_callback(HWND windowHandle);
extern "C" HOOK_API LRESULT CALLBACK KeyboardProc(_In_ int code, _In_ WPARAM wParam, _In_ LPARAM lParam);
