#pragma once
namespace game_framework {
class Map;
class Hero;
class Bullet;

class Item {
    public:
        Item();		//�q�{�غc�l
        Item(Map* M, Hero* H, Bullet* B);	//�غc�l
        void Initialize();	//��l��
        virtual void LoadBitmap();	//���J�ϧ�
        virtual void OnShow();	//��ܹϧ�s
        virtual void OnMove();	//�ϧΪ����ʤΥ\��
        virtual void SetXY(int, int);	//�]�w�ϧ�xy����
        void SetIsAlive(bool alive);	//�]�w�O�_�s��
        void SetType(string);	//�]�w�D�����
        void SetHidden(bool);	//�]�w�D��O�_������
        void ResetBlood();	//���]���
        bool HitHero();	//�O�_�I����^��
        bool HitByBullet();	//�O�_�Q�l�u����
        bool IsAlive();	//�P�_�O�_�s��
        int GetX1();	//���o���W��X����
        int GetY1();	//���o���W��Y����
        int GetX2();	//���o�k�U��X����
        int GetY2();	//���o�k�U��Y����
    protected:
        bool is_alive;
        int x, y, counter,blood;
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	//�˴��ϧΪ��I��
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

class ItemFruit :public Item	//�~�Ӧ�Item class
{
    public:
        ItemFruit(Map* M, Hero* H);	//�غc�l
        void Initialize();	//��l��
        void LoadBitmap();	//���J�ϧ�
        void OnShow();	//��ܹϧ�
        void OnMove();	//�ϧΪ����ʤΥ\��
        void SetFruitType(int);	//�]�w���G����
        int GetFruitType();	//���o���G����
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