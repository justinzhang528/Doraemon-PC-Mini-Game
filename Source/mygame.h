#pragma once
#include "Hero.h"
#include "Enemy.h"
#include "Item.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
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
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////


class Hero;
class Enemy;
class Item;
class Weapoon;

class Map
{
    public:
        Map(Hero* ptrhero);	//�غc�l
        void Initialize();	//��l�Ʀa�ϼƾ�
        void InitializeMovingBlockMap();	//��l�Ƥ���a��
        void LoadBitmap();	//���J�ϧ�
        void OnShow();	//��ܹϧ�
        void OnMove();	//�a�Ϫ����ʻP�\��
        void SetXY(int,int);	//�]�w�a�Ϫ�XY����
        void ChangeMapValue(int, int, int);	//���ܦa�ϰ}�C����
        int GetX();	//���o�a�ϥ��W��X����
        int GetY();	//���o�a�ϥ��W��Y����
        int GetMapValue(int, int);	//���o�a�ϰ}�C����
        int ScreenX(int x);	//���o����X����
        int ScreenY(int y);	//���o����Y����
        bool isEmpty(int, int);	//�P�_�O�_����
        bool isTrap(int, int);	//�P�_�O�O�_������
        bool isNotBlock(int, int);	//�P�_�O�_�����
        int MapLevel;	//�a�ϥd��
    protected:
        CMovingBitmap background, background3;	//�a�Ϲϧ�
        vector<CMovingBitmap> v_background;	//�a�Ϲϧ�
        Hero* hero;	//�^������
        int map[40][50], map_[4][11][26],map_castle[11][51],map3[11][200],map3_[5][14][16];	//�a�ϰ}�C
        int sx, sy;	//�a��XY����
        const int mW, mH;	//�a�Ϫ��e��
};

class CGameStateInit : public CGameState {
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnKeyDown(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
    protected:
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
        void OnMove();
    private:
        CMovingBitmap background,help,about,start,cg2_1,cg2_2;
        CAnimation button0, button1, button2,cg1,cg3;
        int pressCount,counter,music_counter;
        bool playCG,isShowHelp,isShowAbout;
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void ResetEverything();
        void ResetMovingBlock();
        void MapChangingEffect();
        int enemyNumber_1=32,enemyNumber_3=16;
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        CAnimation nobi,allWalking,firework;
        vector<CMovingBitmap> dialogueText;
        CMovingBitmap bmp,black,door,door2,endingBackground,end;
        Enemy* enemies_1[32], *enemies_3[16];
        EnemyBossMan* bossMan;
        EnemyFireBall* fireBall[17];
        EnemySkullSoul* skullSoul;
        WeaponFire* weaponFire[1];			//�ثe�u�b1-3�a�ϥΤF�@��
        CastleRed* castle;
        Hero hero;		// �^��
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
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        int counter;	// �˼Ƥ��p�ƾ�
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