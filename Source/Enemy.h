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
        Enemy();	//建構子
        virtual ~Enemy() {};	//解構子
        virtual void Initialize();	//初始化敵人數據
        virtual void LoadBitmap() {}									// 載入圖形
        virtual void OnMove() {}											// 敵人的移動及其他功能
        virtual void OnShow() {}											// 將圖形貼到畫面
        virtual void SetXY(int nx, int ny);								// 設定XY坐標值
        virtual void SetIsAlive(bool alive);							// 設定是否存活
        virtual void BounceAway();	//彈走敵人
        virtual void ResetBlood() {}	//重置血量
        void BeHit();	//被子彈擊中時的功能
        void SetMovingLeft(bool);	//設定是否正在往左移動
        void SetMovingRight(bool);	//	設定是否正在往右移動
        void SetMovingUp(bool);	//設定是否正在往上移動
        void SetMovingDown(bool);	//設定是否正在往下移動
        void FollowHero(int step);	//追擊英雄
        void PlaySoundEffect();	//播放音效
        virtual int  GetX1();	//取得左上角X坐標
        virtual int  GetY1();	//取得左上角Y坐標
        virtual int  GetX2();	//取得右下角X坐標
        virtual int  GetY2();	//取得右下角Y坐標
        virtual int GetBloodAmount();	//取得血量
        virtual bool HitHero();						// 是否碰到英雄
        virtual bool HitByBullet();	//判斷是否被子彈擊中
        bool IsAlive();											// 是否活著
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到指定圖形
    protected:
        CMovingBitmap bmp;	//敵人圖形
        int x, y,blood,distance,count,stopTime;	//XY坐標、血量、距離、計數、暫停時間
        bool is_alive,isStop;	//是否存活、是否暫停
        bool isMovingLeft;	//是否向左移動
        bool isMovingRight;	//是否向右移動
        bool isMovingUp;	//是否向上移動
        bool isMovingDown;	//是否向下移動
        CGameState* game;	//遊戲狀態的指標
        Map* m;	//地圖指標
        Hero* h;	//英雄指標
        Bullet* b;	//子彈指標
        ItemFruit* f;	//水果道具指標
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
        EnemySkullSoul(Map* M, Hero* H, Bullet* B);	//建構子
        ~EnemySkullSoul();	//解構子
        void Initialize();	//初始化敵人數據
        void LoadBitmap();	//載入圖形
        void OnMove();	//敵人的移動及其他功能
        void OnShow();	//顯示圖形
        void ResetBlood();	//重置血量
        void ReduceBlood(int);	//減少血量
        void PlaySoundEffect();	//播放音效
    private:
        CAnimation skullR, skullL;	//敵人的圖形
        int num, count, hiddenTime;	//數量、計數、隱藏時間
        bool isHidden;	//是否為隱藏
        Weapon* w;	//武器的指標
        const int default_blood = 20;	//默認血量
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
        MovingBlock(Map* M, Hero* H, Bullet* B);	//建構子
        void Initialize();	//移動方塊的數據初始化
        void LoadBitmap();	//載入圖形
        void OnMove();	//方塊的移動及功能
        void OnShow();	//顯示圖形
        void BeHit();	//被子彈擊中的功能
        int  GetX1();	//取得左上角X坐標
        int  GetY1();	//取得左上角Y坐標
        int  GetX2();	//取得右下角X坐標
        int  GetY2();	//取得右下角Y坐標
};

class EnemyBossMan :public Enemy {
    public:
        EnemyBossMan(Map* M, Hero* H, Bullet* B);	//建構子
        void Initialize();	//初始化魔王數據
        void LoadBitmap();	//載入圖形
        void OnMove();	//魔王的移動及其它功能
        void OnShow();	//顯示圖形
        void BeHit();	//被子彈擊中的功能
        void SprayFire();	//噴出火球
        void MoveFire();	//移動火球
        void ShowFire();	//顯示火球
        void ThrowWeapon();	//丟出武器
        void MoveWeapon();	//移動武器
        void ShowWeapon();	//顯示武器
        void ResetBlood();	//重置血量
        void ReduceBlood(int);	//減少血量
        bool HitByBullet();	//判斷是否被子彈擊中
        bool HitHero();	//判斷是否碰撞到英雄
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	//判斷是否碰撞到指定圖形
        ~EnemyBossMan();	//解構子
    private:
        CAnimation left,right,attackL,attackR,spray,dead,left_h, right_h, attackL_h, attackR_h, spray_h;	//魔王的圖形
        CMovingBitmap dead_pic;	//魔王死亡時的圖形
        bool isAttackL, isAttackR, isSpray,ishurt,isdead,isThrow;	//攻擊、噴火、受傷、丟出武器等的判斷值
        int distance, hurt_t,dead_t,counter;	//距離、受傷時間、死亡時間、計數
        const int default_blood = 20;	//默認血量
        WeaponFire* fire[8];	//火球指標
        WeaponCrescent* crescent[6];	//武器指標
};

}