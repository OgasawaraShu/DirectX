#include "DebugText.h"
#include <cassert>
//#include <cassert>

void DebugText::debugTextInit(SpriteCommon* spritecommon, UINT texnumber)
{
    assert(spritecommon);

    spriteCommon_ = spritecommon;

    for (int i = 0; i < _countof(sprites_); i++)
    {
        sprites_[i] = Sprite::Create(spriteCommon_, texnumber, { 0,0 });
    }
}

void DebugText::Print(const std::string& text, float x, float y, float scale)
{
    for (int i = 0; i < text.size(); i++)
    {
        if (spriteIndex_ >= maxCharCount)
        {
            break;
        }

        const unsigned char& charctor = text[i];

        int fontIndex = charctor - 32;

        if (charctor >= 0x7f)
        {
            fontIndex = 0;
        }

        int fontIndexX = fontIndex % fontLineCount;

        int fontIndexY = fontIndex / fontLineCount;

        // sprites[spriteIndex]->position = { x + fontWidth * scale * i,y,0 };
        sprites_[spriteIndex_]->SetPosition({ x + fontWidth * scale * i,y,0 });
        //sprites[spriteIndex].texLeftTop = { (float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight };
        sprites_[spriteIndex_]->SettexLeftTop({ (float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight });
        // sprites[spriteIndex].texSize = { fontWidth,fontHeight };
        sprites_[spriteIndex_]->SettexSize({ fontWidth,fontHeight });
        //sprites[spriteIndex].size = { fontWidth * scale,fontHeight * scale };
        sprites_[spriteIndex_]->SetSize({ fontWidth * scale,fontHeight * scale });

        sprites_[spriteIndex_]->SpriteTransVertexBuffer();
        //   SpriteTransVertexBuffer(sprites[spriteIndex], spritecommon);
        sprites_[spriteIndex_]->Update();
        //    SpriteUpdate(sprites[spriteIndex], spritecommon);

        spriteIndex_++;


    }
}

void DebugText::DrawAll()
{
    for (int i = 0; i < spriteIndex_; i++)
    {
        sprites_[i]->SpriteDraw();
        // SpriteDraw(sprites[i], cmdList, spriteCommon, dev);
    }

    spriteIndex_ = 0;
}
