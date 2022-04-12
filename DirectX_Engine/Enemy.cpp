#include "Enemy.h"
#include "Player.h"
#include <cstdlib>   
#include <time.h>
//#include<math.h>
//#include <DirectXMath.h>
/*
Enemy::ENEMY1* Enemy::createCell(void)
{

    //1個分のメモリを確保し、場所を返す
    ENEMY1* newEnemy1;
    newEnemy1 = (ENEMY1*)malloc(sizeof(ENEMY1));


    return newEnemy1;
}

void Enemy::create(float x, float y, float r, int flag, int cr_flag,int id)
{
  
    ENEMY1* newEnemy, * current;
    
    int input_id;

    int input_x;
    int input_y;

    int input_R;
    int input_Flag;
    int input_Color_Flag = 0;

    // 新し敵の中身を作成。(1)
    newEnemy = createCell();

    input_id = id;
    newEnemy->id = input_id;

    input_x = x;
    newEnemy->X = input_x;
  
    input_y = y;
    newEnemy->X = input_y;

    input_R = r;
    newEnemy->R = input_R;

    input_Flag=flag;
    newEnemy->Flag = input_Flag;


    input_Color_Flag = cr_flag;
    newEnemy->Color_Flag = input_Color_Flag;


    //ヘッドに変更する
    current = tail;
    if ((head == NULL) && (tail == NULL))
    {
        // リストが空
        // 新しいCellが、先頭かつ末尾になる。
        head = newEnemy;
        tail = newEnemy;
    }
    else
    {
        // リストに1件以上Cellが存在する場合。
        newEnemy->before = current;
        newEnemy->next = current->next;
        // 末尾Cell(tail)のnextに、新しいCellの場所をセット。
        tail->next = newEnemy;


        // 追加した新しいCellをtailとする。
        tail = newEnemy;
    }
    // 次のデータは無いので、nextにはNULLをセット。
    tail->next = NULL;
}

void Enemy::editCell(float x, float y, float r, int flag, int cr_flag, int id)
{
    ENEMY1* current;
    int input_id;

    int input_x;
    int input_y;

    int input_R;
    int input_Flag;
    int input_Color_Flag = 0;

    input_id = id;
    // リストの先頭から、挿入先を探します。
    current = head;
    while (current != NULL)
    {
        if (current->id == input_id)
        {
            break;
        }
        else
        {
            current = current->next;
        }
    }
    // 挿入先が見つからなかったら、何もせずreturn。
    if (current == NULL)
    {
      
        return;
    }

    //中身の編集
    input_id = id;
    current->id = input_id;


    input_x = x;
    current->X = input_x;

    input_y = y;
    current->X = input_y;

    input_R = r;
    current->R = input_R;

    input_Flag = flag;
    current->Flag = input_Flag;


    input_Color_Flag = cr_flag;
    current->Color_Flag = input_Color_Flag;

}
*/

Player player;

 //ここから配列Ver
void Enemy::Update()
{
    Generation();//フラグが立ったら生成する

    Move();//ゲーム内の動作

   
}

void Enemy::Generation()//生成
{
  
    Flagged();//条件が整ったらフラグが立つ
    
    //生成する
    if (Enemy1->Flag == 1)
    {
        //敵の出現する座標と色の設定
    }

}

void Enemy::Intialize()//初期化
{
  
    memset(Enemy1, 0, sizeof(Enemy1));

    //構造体初期化代入
    for (int i = 0; i++; i < 100)
    {
        Enemy1[i]={0,0,0,0,1,0,0,0,0};
    }


    Enemy1[1].X = rand()%800+400;
    Enemy1[1].Y = 0;
    Enemy1[1].R = 50;

    Enemy1->Flag = 1;

}

void Enemy::Flagged()//フラグ処理
{

    Enemy1->Timer++;
  //  Enemy1->Flag = 1;

    for (int i = 0; i++; i < 100)
    {
        
        //規定条件になったらフラグが立つ
        if (Enemy1->Timer >= 360 * i)
        {
           
        }
    }
}

void Enemy::Move()//敵の動作
{
    //キャラの座標毎フレーム確認
    player.Controll();

    //自キャラに寄ってく
    //自と敵の方向を確認する
   Enemy1[1].Angle = atan2(player.Central_y - Enemy1[1].Y,player.Central_x-Enemy1[1].X);

    if (Enemy1->Flag == 1)
    {
        //フラグが立っているときに自機に向かっていく
        Enemy1[1].X += cos(Enemy1[1].Angle)*1;
        Enemy1[1].Y += sin(Enemy1[1].Angle)*1;                                                                
    }
}

void Enemy::Rear()//敵の後処理
{

}
