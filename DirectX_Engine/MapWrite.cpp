#include "MapWrite.h"

void MapWrite::WriteSet(int type, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate,XMFLOAT3 ColisionSize)
{

    FILE* fp1;
    //テキスト暗号化した状態で保存
    fp1 = _fsopen("STAGE2.txt", "w", _SH_DENYNO);

    if (fp1 == NULL) {
        assert(0);
    }
    else {
        //fileがnullじゃないのなら各情報を入れる
        fprintf(fp1, "TYPE,%d\r\n", type);//モデルタイプ
        fprintf(fp1, "POSITION,%f,%f,%f\r\n",pos.x,pos.y,pos.z);//pos
        fprintf(fp1, "SCALE,%f,%f,%f\r\n", scale.x, scale.y, scale.z);//scale
        fprintf(fp1, "ROTATE,%f,%f,%f\r\n", rotate.x, rotate.y, rotate.z);//rotate
        fprintf(fp1, "COLISIONSIZE,%f,%f,%f\r\n", ColisionSize.x, ColisionSize.y, ColisionSize.z);//rotate
        fclose(fp1);
    }
}
