#pragma once


// // SDKDDKVer.h をインクルードすると、利用できる最も高いレベルの Windows プラットフォームが定義されます。
// 以前の Windows プラットフォーム用にアプリケーションをビルドする場合は、WinSDKVer.h をインクルードし、
// サポートしたいプラットフォームに _WIN32_WINNT マクロを設定してから SDKDDKVer.h をインクルードします。
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// Windows ヘッダー ファイル
#include <windows.h>
// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>
#include <chrono>

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring& wstr);

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string& str);

// syntax suger
std::wstring utf8_decode(const char* ptr);
