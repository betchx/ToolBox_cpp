
// // SDKDDKVer.h ���C���N���[�h����ƁA���p�ł���ł��������x���� Windows �v���b�g�t�H�[������`����܂��B
// �ȑO�� Windows �v���b�g�t�H�[���p�ɃA�v���P�[�V�������r���h����ꍇ�́AWinSDKVer.h ���C���N���[�h���A
// �T�|�[�g�������v���b�g�t�H�[���� _WIN32_WINNT �}�N����ݒ肵�Ă��� SDKDDKVer.h ���C���N���[�h���܂��B
#include <SDKDDKVer.h>


#define WIN32_LEAN_AND_MEAN             // Windows �w�b�_�[����قƂ�ǎg�p����Ă��Ȃ����������O����
// Windows �w�b�_�[ �t�@�C��
#include <windows.h>

// C �����^�C�� �w�b�_�[ �t�@�C��
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
    // �������Ώۃt�@�C�����̎擾
    auto arg = std::wstring(lpCmdLine);

    // �_�u���N�H�[�e�[�V�����Ŋ����Ă���΁C���O���D
    std::wsmatch m;
    auto re_dq = std::wregex(L"\"(.+)\"");
    if (std::regex_match(arg, m, re_dq)) {
        arg = m[1];
    }

    using std::endl;
    auto target = std::filesystem::path(arg).lexically_normal();

    // �t�@�C���̑��݊m�F
    // ������D&D�ɂ͑Ή����Ȃ�
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
        // ���r�W�����ԍ����擾��1���₷
        std::string num = utf8_encode(m[1]);
        rev = std::stoi(num) + 1;
        base = std::regex_replace(stem, re_rev, L"");
    }
    else {
        base = stem;
    }
    base += L"_r";

    // �i�v���[�v�h�~�̂��߂ɁC999�܂łɂ���D
    for (; rev < 1000; rev++) {
        dest = parent / base;
        dest += std::to_string(rev);
        dest += ext;
        if (!std::filesystem::exists(dest))
            break;
    }

    // copy�����s�D�G���[���o��\��������̂ŁC��O���L���b�`�D
    try {
        std::filesystem::copy_file(target, dest);
    }
    catch (std::filesystem::filesystem_error& e)
    {
        return 2;
    }

    return 0;
}
