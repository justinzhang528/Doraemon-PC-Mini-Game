#pragma once
namespace game_framework {
class Map;
class Hero;
class Bullet;

class Item {
    public:
        Item();		//默認建構子
        Item(Map* M, Hero* H, Bullet* B);	//建構子
        void Initialize();	//初始化
        virtual void LoadBitmap();	//載入圖形
        virtual void OnShow();	//顯示圖形s
        virtual void OnMove();	//圖形的移動及功能
        virtual void SetXY(int, int);	//設定圖形xy坐標
        void SetIsAlive(bool alive);	//設定是否存活
        void SetType(string);	//設定道具種類
        void SetHidden(bool);	//設定道具是否為隱藏
        void ResetBlood();	//重設血條
        bool HitHero();	//是否碰撞到英雄
        bool HitByBullet();	//是否被子彈擊中
        bool IsAlive();	//判斷是否存活
        int GetX1();	//取得左上角X坐標
        int GetY1();	//取得左上角Y坐標
        int GetX2();	//取得右下角X坐標
        int GetY2();	//取得右下角Y坐標
    protected:
        bool is_alive;
        int x, y, counter,blood;
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	//檢測圖形的碰撞
        CGameState* game;
        Hero* h;
        Map* m;
        Bullet* b;
        CMovingBitmap item;
    private:
        bool hidden;
        string type;
        CMovingBitmap lifeUp, bell, cureBottle, guard;
};

class ItemFruit :public Item	//繼承自Item class
{
    public:
        ItemFruit(Map* M, Hero* H);	//建構子
        void Initialize();	//初始化
        void LoadBitmap();	//載入圖形
        void OnShow();	//顯示圖形
        void OnMove();	//圖形的移動及功能
        void SetFruitType(int);	//設定水果種類
        int GetFruitType();	//取得水果種類
    private:
        int type;
        vector<CMovingBitmap> fruit;
};

class ItemKey :public Item
{
    public:
        ItemKey(Map* M, Hero* H);
        void Initialize();
        void LoadBitmap();
        void OnShow();
        void OnMove();
        void SetFollowHero(bool);
        bool FollowedHero();
    private:
        bool isFollowHero;
};

class ItemLetter :public Item
{
    public:
        ItemLetter(Map* M, Hero* H);
        void Initialize();
        void LoadBitmap();
        void OnShow();
        void OnMove();
};

class ItemKongQiPao :public Item
{
    public:
        ItemKongQiPao(Map* M, Hero* H);
        void Initialize();
        void LoadBitmap();
        void OnShow();
        void OnMove();
        void SetType(int);
    private:
        CMovingBitmap weapon[3];
        int type;
};

class ItemDoor :public Item
{
    public:
        ItemDoor(Map* M, Hero* H);
        void Initialize();
        void LoadBitmap();
        void OnShow();
    private:
        CMovingBitmap starDoor;
};

}