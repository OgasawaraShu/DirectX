#include "MapWrite.h"

void MapWrite::WriteSet(int type, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate,XMFLOAT3 ColisionSize)
{

    FILE* fp1;
    //�e�L�X�g�Í���������Ԃŕۑ�
    fp1 = _fsopen("STAGE2.txt", "w", _SH_DENYNO);

    if (fp1 == NULL) {
        assert(0);
    }
    else {
        //file��null����Ȃ��̂Ȃ�e��������
        fprintf(fp1, "TYPE,%d\r\n", type);//���f���^�C�v
        fprintf(fp1, "POSITION,%f,%f,%f\r\n",pos.x,pos.y,pos.z);//pos
        fprintf(fp1, "SCALE,%f,%f,%f\r\n", scale.x, scale.y, scale.z);//scale
        fprintf(fp1, "ROTATE,%f,%f,%f\r\n", rotate.x, rotate.y, rotate.z);//rotate
        fprintf(fp1, "COLISIONSIZE,%f,%f,%f\r\n", ColisionSize.x, ColisionSize.y, ColisionSize.z);//rotate
        fclose(fp1);
    }
}
