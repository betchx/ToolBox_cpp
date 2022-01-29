
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

#include "..\rename2today\string_convert.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // 引数より対象ファイル名の取得
    auto arg = std::wstring(lpCmdLine);

    // ダブルクォーテーションで括られていれば，取り外す．
    std::wsmatch m;
    auto re_dq = std::wregex(L"\"(.+)\"");
    if (std::regex_match(arg, m, re_dq)) {
        arg = m[1];
    }

    using std::endl;
    auto target = std::filesystem::path(arg).lexically_normal();

    // ファイルの存在確認
    // 複数のD&Dには対応しない
    if (!std::filesystem::exists(target))
        return 1;

    std::filesystem::path dest;
    auto parent = target.parent_path();
    auto re_rev = std::wregex(L"_[rR](\\d+)$");
    std::wstring stem = target.stem().c_str();
    auto ext = target.extension();
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

    // copyを実行．エラーが出る可能性があるので，例外をキャッチ．
    try {
        std::filesystem::copy_file(target, dest);
    }
    catch (std::filesystem::filesystem_error& e)
    {
        return 2;
    }

    return 0;
}
