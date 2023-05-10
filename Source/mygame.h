#pragma once
#include "Hero.h"
#include "Enemy.h"
#include "Item.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID {				// 定義各種音效的編號
    AUDIO_map1,				// 2
    AUDIO_map1_,
    AUDIO_map3,
    AUDIO_map3_,
    AUDIO_Doraemon,
    AUDIO_START,
    AUDIO_DEAD,
    AUDIO_OVER,
    AUDIO_PASS,
    AUDIO_win,
    AUDIO_shoot2,
    AUDIO_shoot3,
    AUDIO_shoot5,
    AUDIO_pick01,
    AUDIO_pick02,
    AUDIO_hit01,
    AUDIO_hit02,
    AUDIO_hit05,
    AUDIO_hit07,
    AUDIO_hit11,
    AUDIO_hit12,
    AUDIO_hit13,
    AUDIO_soul,
    AUDIO_horror,
    AUDIO_firepig,
    AUDIO_injure,
    AUDIO_select,
    AUDIO_bird,
    AUDIO_pig,
    AUDIO_manhurt,
    AUDIO_manshout,
    AUDIO_startCG,
    AUDIO_intohole,
    AUDIO_castlered,
    AUDIO_boss1,
    AUDIO_key,
    AUDIO_letter,
    AUDIO_hittree,
    AUDIO_hitfire,
    AUDIO_hitfrog,
    AUDIO_getweapon,
    AUDIO_jump
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////


class Hero;
class Enemy;
class Item;
class Weapoon;

class Map
{
    public:
        Map(Hero* ptrhero);	//建構子
        void Initialize();	//初始化地圖數據
        void InitializeMovingBlockMap();	//初始化方塊地圖
        void LoadBitmap();	//載入圖形
        void OnShow();	//顯示圖形
        void OnMove();	//地圖的移動與功能
        void SetXY(int,int);	//設定地圖的XY坐標
        void ChangeMapValue(int, int, int);	//改變地圖陣列的值
        int GetX();	//取得地圖左上角X坐標
        int GetY();	//取得地圖左上角Y坐標
        int GetMapValue(int, int);	//取得地圖陣列的值
        int ScreenX(int x);	//取得熒幕X坐標
        int ScreenY(int y);	//取得熒幕Y坐標
        bool isEmpty(int, int);	//判斷是否為空
        bool isTrap(int, int);	//判斷是是否為陷阱
        bool isNotBlock(int, int);	//判斷是否有方塊
        int MapLevel;	//地圖卡關
    protected:
        CMovingBitmap background, background3;	//地圖圖形
        vector<CMovingBitmap> v_background;	//地圖圖形
        Hero* hero;	//英雄指標
        int map[40][50], map_[4][11][26],map_castle[11][51],map3[11][200],map3_[5][14][16];	//地圖陣列
        int sx, sy;	//地圖XY坐標
        const int mW, mH;	//地圖的寬高
};

class CGameStateInit : public CGameState {
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnKeyDown(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
    protected:
        void OnShow();									// 顯示這個狀態的遊戲畫面
        void OnMove();
    private:
        CMovingBitmap background,help,about,start,cg2_1,cg2_2;
        CAnimation button0, button1, button2,cg1,cg3;
        int pressCount,counter,music_counter;
        bool playCG,isShowHelp,isShowAbout;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void ResetEverything();
        void ResetMovingBlock();
        void MapChangingEffect();
        int enemyNumber_1=32,enemyNumber_3=16;
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        CAnimation nobi,allWalking,firework;
        vector<CMovingBitmap> dialogueText;
        CMovingBitmap bmp,black,door,door2,endingBackground,end;
        Enemy* enemies_1[32], *enemies_3[16];
        EnemyBossMan* bossMan;
        EnemyFireBall* fireBall[17];
        EnemySkullSoul* skullSoul;
        WeaponFire* weaponFire[1];			//目前只在1-3地圖用了一個
        CastleRed* castle;
        Hero hero;		// 英雄
        Map* map = nullptr;
        Item* item_1[16];
        ItemKongQiPao* heroWeapon[3];
        ItemKey* itemKey[2];
        ItemDoor* starDoor[7];
        ItemLetter* itemLetter[2];
        Bullet* heroBullet;
        MovingBlock* movingBlock[23];
        int dead_counter,space_counter,black_counter,weaponFire_distance, dialogue_counter,endingMoveX;
        bool isTalking, isPressSpace, isShowBlack, isDoorOpen[2],isPlayEnding;


};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        int counter;	// 倒數之計數器
        CMovingBitmap over;

};

class CGameStateRest :public CGameState {
    public:
        CGameStateRest(CGame* g);
        void OnBeginState();
        void OnInit();
    protected:
        void OnMove();
        void OnShow();
    private:
        int counter;
        CMovingBitmap rest,rest2;
};



}