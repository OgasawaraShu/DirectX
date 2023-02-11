#pragma once
#include "Sprite.h"
#include "SpriteCommon.h"
#include <windows.h>
#include <string>

class DebugText
{
public:
    static const int maxCharCount = 256;
    //�t�H���g�̉��T�C�Y
    static const int fontWidth = 80;
    //�t�H���g�̏c�T�C�Y
    static const int fontHeight = 85;
    //�t�H���g�̃��C���J�E���g
    static const int fontLineCount = 16;

    Sprite* sprites_[maxCharCount];

    int spriteIndex_ = 0;

    SpriteCommon* spriteCommon_ = nullptr;

    void debugTextInit(SpriteCommon* spritecommon, UINT texnumber);

    void Print(const std::string& text, float x, float y, float scale = 1.0f);

    void DrawAll();
};
