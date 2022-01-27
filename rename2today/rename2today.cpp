// rename2today.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "rename2today.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // 引数より対象ファイル名の取得
    auto arg = std::wstring(lpCmdLine);
    using std::endl;
    auto target = std::filesystem::path(arg);
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
    std::wsmatch m;
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
        std::wstring stem = target.stem().c_str();
        auto ext = target.extension();
        int rev = 1;
        std::filesystem::path base;
        if (std::regex_search(stem, m, re_rev)){
            // リビジョン番号を取得し1増やす
            rev = atoi(utf8_encode(* m.begin()).c_str())+1;
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
