#pragma once


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

#include "string_convert.h"
