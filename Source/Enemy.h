#pragma once
namespace game_framework {
class Map;
class Bullte;
class Hero;
class ItemFruit;
class Weapon;
class EnemyBossMan;
class Enemy
{
    public:
        Enemy();	//�غc�l
        virtual ~Enemy() {};	//�Ѻc�l
        virtual void Initialize();	//��l�ƼĤH�ƾ�
        virtual void LoadBitmap() {}									// ���J�ϧ�
        virtual void OnMove() {}											// �ĤH�����ʤΨ�L�\��
        virtual void OnShow() {}											// �N�ϧζK��e��
        virtual void SetXY(int nx, int ny);								// �]�wXY���Э�
        virtual void SetIsAlive(bool alive);							// �]�w�O�_�s��
        virtual void BounceAway();	//�u���ĤH
        virtual void ResetBlood() {}	//���m��q
        void BeHit();	//�Q�l�u�����ɪ��\��
        void SetMovingLeft(bool);	//�]�w�O�_���b��������
        void SetMovingRight(bool);	//	�]�w�O�_���b���k����
        void SetMovingUp(bool);	//�]�w�O�_���b���W����
        void SetMovingDown(bool);	//�]�w�O�_���b���U����
        void FollowHero(int step);	//�l���^��
        void PlaySoundEffect();	//���񭵮�
        virtual int  GetX1();	//���o���W��X����
        virtual int  GetY1();	//���o���W��Y����
        virtual int  GetX2();	//���o�k�U��X����
        virtual int  GetY2();	//���o�k�U��Y����
        virtual int GetBloodAmount();	//���o��q
        virtual bool HitHero();						// �O�_�I��^��
        virtual bool HitByBullet();	//�P�_�O�_�Q�l�u����
        bool IsAlive();											// �O�_����
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I����w�ϧ�
    protected:
        CMovingBitmap bmp;	//�ĤH�ϧ�
        int x, y,blood,distance,count,stopTime;	//XY���СB��q�B�Z���B�p�ơB�Ȱ��ɶ�
        bool is_alive,isStop;	//�O�_�s���B�O�_�Ȱ�
        bool isMovingLeft;	//�O�_�V������
        bool isMovingRight;	//�O�_�V�k����
        bool isMovingUp;	//�O�_�V�W����
        bool isMovingDown;	//�O�_�V�U����
        CGameState* game;	//�C�����A������
        Map* m;	//�a�ϫ���
        Hero* h;	//�^������
        Bullet* b;	//�l�u����
        ItemFruit* f;	//���G�D�����
};



class EnemyShin :public Enemy {
    public:
        EnemyShin(Map* M, Hero* H, Bullet* B);
        void Initialize();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        ~EnemyShin();
    private:
        CAnimation shinR, shinL;
        int num,count;
        Weapon* w;
        const int default_blood=8;
};

class EnemySkullSoul :public Enemy {
    public:
        EnemySkullSoul(Map* M, Hero* H, Bullet* B);	//�غc�l
        ~EnemySkullSoul();	//�Ѻc�l
        void Initialize();	//��l�ƼĤH�ƾ�
        void LoadBitmap();	//���J�ϧ�
        void OnMove();	//�ĤH�����ʤΨ�L�\��
        void OnShow();	//��ܹϧ�
        void ResetBlood();	//���m��q
        void ReduceBlood(int);	//��֦�q
        void PlaySoundEffect();	//���񭵮�
    private:
        CAnimation skullR, skullL;	//�ĤH���ϧ�
        int num, count, hiddenTime;	//�ƶq�B�p�ơB���îɶ�
        bool isHidden;	//�O�_������
        Weapon* w;	//�Z��������
        const int default_blood = 20;	//�q�{��q
};

class Weapon {
    public:
        Weapon();
        Weapon(Map* m, Hero* h);
        void Initialize();
        bool IsAlive();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void SetShootLeft(bool);
        void SetShootRight(bool);
        void SetShootUp(bool);
        void SetShootDown(bool);
        void SetIsAlive(bool alive);
        void SetIsHit(bool flag);
        void SetReadyShoot(bool);
        void AddCount();
        void ResetCount();
        int GetCount();
        bool GetReadyShoot();
        int GetScreenX();
        int GetScreenY();
        int GetX1();
        int GetY1();
        int GetX2();
        int GetY2();
        void MoveX(int);
        void MoveY(int);
        bool HitHero();
        bool HitBlock();
        bool OverBoundary();
        void SetXY(int, int);
        string type;
        ~Weapon();
    protected:
        bool is_alive;
        bool isShootLeft;
        bool isShootRight;
        bool isShootUp;
        bool isShootDown;
        bool isHit;
        bool isReadyShoot;
        int x, y, count;
        CMovingBitmap bmp;
        CAnimation blueFire;
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);
        Map* m;
        Hero* h;

};

class WeaponFire :public Weapon
{
    public:
        WeaponFire(Map* m, Hero* h);
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void SetXY(int, int);
        void MoveXY(int,int);
    private:
        CAnimation fire;
};

class WeaponCrescent :public Weapon
{
    public:
        WeaponCrescent(Map* M, Hero* H);
        void LoadBitmap();
        void OnShowL();
        void OnShowR();
        void OnMove();
        void SetXY(int, int);
        void MoveX(int);
    private:
        CMovingBitmap bmpL, bmpR;
};



class EnemyMummy :public Enemy {
    public:
        EnemyMummy(Map* M, Hero* H, Bullet* B);
        ~EnemyMummy();
        void Initialize();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
    private:
        CAnimation mummyR, mummyL,mummyF,mummyB;
        const int default_blood = 3; //6;
};


class EnemyFire :public Enemy {
    public:
        EnemyFire(Map* M, Hero* H, Bullet* B);
        ~EnemyFire();
        void Initialize();
        void FollowHero(int step);
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
    private:
        CAnimation fire;
        const int default_blood = 2;
};

class EnemyBigFire :public Enemy {
    public:
        EnemyBigFire(Map* M, Hero* H, Bullet* B);
        ~EnemyBigFire();
        void Initialize();
        void FollowHero(int step);
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
    private:
        CAnimation fire;
        EnemyFire* smallFire[4];
        const int default_blood = 5;
};

class EnemyBird :public Enemy {
    public:
        EnemyBird(Map* M, Hero* H, Bullet* B);
        ~EnemyBird();
        void Initialize();
        void FollowHero(int step);
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
    private:
        CAnimation birdL,birdR;
        const int default_blood = 5;
};

class EnemyBirdVertical :public Enemy {
    public:
        EnemyBirdVertical(Map* M, Hero* H, Bullet* B);
        ~EnemyBirdVertical();
        void Initialize();
        void FollowHero(int step);
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
    private:
        CAnimation birdU, birdD;
        const int default_blood = 5;
};

class EnemyPig :public Enemy {
    public:
        EnemyPig(Map* M, Hero* H, Bullet* B);
        ~EnemyPig();
        void Initialize();
        void FollowHero(int step);
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
    private:
        CAnimation pigL, pigR;
        const int default_blood = 5;
};

class EnemyBooger :public Enemy {
    public:
        EnemyBooger(Map* M, Hero* H, Bullet* B);
        ~EnemyBooger();
        void Initialize();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
    private:
        CAnimation boogerL, boogerR;
        const int default_blood = 5;
};

class EnemyMucus :public Enemy {
    public:
        EnemyMucus(Map* M, Hero* H, Bullet* B);
        ~EnemyMucus();
        void Initialize();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
    private:
        CAnimation mucusL, mucusR;
        const int default_blood = 5;
};

class EnemyBat :public Enemy {
    public:
        EnemyBat(Map* M, Hero* H, Bullet* B);
        ~EnemyBat();
        void Initialize();
        void LoadBitmap();
        void FollowHero(int);
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
    private:
        CAnimation bat;
        const int default_blood = 4;
};

class EnemyUFOBullet :public Weapon {
    public:
        EnemyUFOBullet(Map* M, Hero* H);
        void Initialize();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void SetXY(int, int);
        bool lu, ru, ld, rd;
};

class EnemyUFO :public Enemy {
    public:
        EnemyUFO(Map* M, Hero* H, Bullet* B);
        void Initialize();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ShootBullet();
        void ResetBlood();
        void PlaySoundEffect();
        ~EnemyUFO();
    private:
        CAnimation ufo;
        EnemyUFOBullet* bullet;
        const int default_blood = 5;
};


class EnemyTree :public Enemy {
    public:
        EnemyTree(Map* M, Hero* H, Bullet* B);
        ~EnemyTree();
        void Initialize();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
    private:
        CAnimation treeL,treeR;
        const int default_blood = 6;
};

class EnemyFirePig :public Enemy {
    public:
        EnemyFirePig(Map* M, Hero* H, Bullet* B);
        void Initialize();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void ResetBlood();
        void PlaySoundEffect();
        ~EnemyFirePig();
    private:
        int load;
        CAnimation firePig;
        WeaponFire* fire=NULL;
        const int default_blood = 6;
};

class EnemyFireBall :public Enemy {
    public:
        EnemyFireBall(Map* M, Hero* H);
        void Initialize();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void SetType(string);
    private:
        CAnimation fireD,fireU;
        string type;
};

class CastleRed :public Enemy {
    public:
        CastleRed(Map* M, Hero* H, Bullet* B);
        void Initialize();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        bool HitByBullet();
        void BeHit();
    private:
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);
        const int default_blood = 6;
};

class MovingBlock :public Enemy {
    public:
        MovingBlock(Map* M, Hero* H, Bullet* B);	//�غc�l
        void Initialize();	//���ʤ�����ƾڪ�l��
        void LoadBitmap();	//���J�ϧ�
        void OnMove();	//��������ʤΥ\��
        void OnShow();	//��ܹϧ�
        void BeHit();	//�Q�l�u�������\��
        int  GetX1();	//���o���W��X����
        int  GetY1();	//���o���W��Y����
        int  GetX2();	//���o�k�U��X����
        int  GetY2();	//���o�k�U��Y����
};

class EnemyBossMan :public Enemy {
    public:
        EnemyBossMan(Map* M, Hero* H, Bullet* B);	//�غc�l
        void Initialize();	//��l���]���ƾ�
        void LoadBitmap();	//���J�ϧ�
        void OnMove();	//�]�������ʤΨ䥦�\��
        void OnShow();	//��ܹϧ�
        void BeHit();	//�Q�l�u�������\��
        void SprayFire();	//�Q�X���y
        void MoveFire();	//���ʤ��y
        void ShowFire();	//��ܤ��y
        void ThrowWeapon();	//��X�Z��
        void MoveWeapon();	//���ʪZ��
        void ShowWeapon();	//��ܪZ��
        void ResetBlood();	//���m��q
        void ReduceBlood(int);	//��֦�q
        bool HitByBullet();	//�P�_�O�_�Q�l�u����
        bool HitHero();	//�P�_�O�_�I����^��
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	//�P�_�O�_�I������w�ϧ�
        ~EnemyBossMan();	//�Ѻc�l
    private:
        CAnimation left,right,attackL,attackR,spray,dead,left_h, right_h, attackL_h, attackR_h, spray_h;	//�]�����ϧ�
        CMovingBitmap dead_pic;	//�]�����`�ɪ��ϧ�
        bool isAttackL, isAttackR, isSpray,ishurt,isdead,isThrow;	//�����B�Q���B���ˡB��X�Z�������P�_��
        int distance, hurt_t,dead_t,counter;	//�Z���B���ˮɶ��B���`�ɶ��B�p��
        const int default_blood = 20;	//�q�{��q
        WeaponFire* fire[8];	//���y����
        WeaponCrescent* crescent[6];	//�Z������
};

}