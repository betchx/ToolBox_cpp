
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

// Reference: https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte/3999597#3999597
// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string& str)
{
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::wstring utf8_decode(const char* ptr)
{
    std::string str(ptr);
    return utf8_decode(str);
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // 引数より対象ファイル名の取得
    auto arg = std::wstring(lpCmdLine);

    std::wsmatch m;
    auto re_dq = std::wregex(L"\"(.+)\"");
    if (std::regex_match(arg, m, re_dq)) {
        arg = m[1];
    }

    using std::endl;
    auto target = std::filesystem::path(arg).lexically_normal();
    std::wstring fname = target.filename().c_str();

    // ファイルの存在確認
    // 複数のD&Dには対応しない
    if (!std::filesystem::exists(target))
        return 1;

    // 今日の日付の文字列を作成 
    time_t tm = time(nullptr);
    struct tm lt;
    localtime_s(&lt, &tm);
    const unsigned buf_size = 15;
    char buf[buf_size];
    strftime(buf, buf_size, "%Y-%m-%d", &lt);

    // wstringに変換
    auto date_str = utf8_decode(buf);

    std::filesystem::path dest;
    auto parent = target.parent_path();

    // 対象ファイル名に日付が含まれているかを確認
    auto re = std::wregex(L"((?:20)?[012][0-9])-(0\\d|11|12)-(0[1-9]|[12]\\d|30|31)");
    if (std::regex_search(fname, m, re)) {
        auto rep = std::regex_replace(fname, re, date_str);
        dest = parent / rep;
    }
    else {
        // ファイル名の先頭に日付文字列とアンダーバーを追加したものにする
        dest = parent / date_str;
        dest += L"_";
        dest += fname;
    }
    // 名前変更先に同名のファイルがある場合はrevisionアップする
    // もともと日付が同じ場合も同様になるので，リビジョンのインクリメントに化ける．
    if (std::filesystem::exists(dest)) {
        auto re_rev = std::wregex(L"_[rR](\\d+)$");
        std::wstring stem = dest.stem().c_str();
        auto ext = dest.extension();
        int rev = 1;
        std::filesystem::path base;
        if (std::regex_search(stem, m, re_rev)) {
            // リビジョン番号を取得し1増やす
            std::string num = utf8_encode(m[1]);
            rev = std::stoi(num) + 1;
            base = std::regex_replace(stem, re_rev, L"");
        }
        else {
            base = stem;
        }
        base += L"_r";

        // 永久ループ防止のために，999までにする．
        for (; rev < 1000; rev++) {
            dest = parent / base;
            dest += std::to_string(rev);
            dest += ext;
            if (!std::filesystem::exists(dest))
                break;
        }
    }

    // renameを実行．エラーが出る可能性があるので，例外をキャッチ．
    try {
        std::filesystem::rename(target, dest);
    }
    catch (std::filesystem::filesystem_error& e)
    {
        return 2;
    }

    return 0;
}
