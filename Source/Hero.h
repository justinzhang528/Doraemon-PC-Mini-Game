#pragma once
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class提供可以用鍵盤或滑鼠控制的擦子
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class Map;
class EnemyShin;
class Enemy;
class Bullet;
class Hero
{
    public:
        Hero();
        int GetX1();					//取得左上角 x 座標
        int GetY1();					//取得左上角 y 座標
        int GetX2();					//取得右下角 x 座標
        int GetY2();					//取得右下角 y 座標
        int GetBloodAmount();	//取得當前血量
        int GetLifeAmount();	//取得當前生命值
        int GetJumpHeight();	//取得跳躍高度
        int GetStepSize();	//取得步數
        int GetFootX();	//取得英雄腳底X坐標
        int GetFootY();	//取得英雄腳底Y坐標
        void Initialize();				//設定英雄為初始值
        void RefreshAnimation();	//刷新英雄狀態
        void LoadBitmap();				//載入圖形
        void OnMove(Map* m);					//移動英雄
        void OnShow(Map* m,Bullet* b);					//顯示圖形
        void SetMovingDown(bool flag);	//設定是否正在往下移動
        void SetMovingLeft(bool flag);	//設定是否正在往左移動
        void SetMovingRight(bool flag); //設定是否正在往右移動
        void SetMovingUp(bool flag);	//設定是否正在往上移動
        void SetPressedDown(bool flag);	//設定是否按到下鍵
        void SetPressedLeft(bool flag);	//設定是否按到左鍵
        void SetPressedRight(bool flag); //設定是否按到右鍵
        void SetPressedUp(bool flag);	//設定是否按到上鍵
        void SetJumping(bool flag);	//設為跳躍模式
        void SetXY(int nx, int ny);		//設定XY座標
        void SetPressedSpace(bool flag);	//設定是否按下空格鍵
        void ReduceBlood(int);	//減少血量
        void ReduceLife();	//減少生命值
        void AddBlood(int);	//增加血量
        void AddLife(int);	//增加生命值
        void ResetBloodAmount();	//重置血量
        void ResetLifeAmount();	//重置生命值
        void GetedBullet(bool);	//設定是否已得到空氣炮武器
        void GetedMagic(bool);	//設定是否已得到魔法
        void GetedBlade(bool);	//設定是否已得到飛刃
        void ActivedBullet(bool);	//設定是否已激活空氣炮
        void ActivedMagic(bool);	//設定你是否已激活魔法
        void ActivedBlade(bool);	//設定是否已激活飛刃
        void ChangeWeapon();	//切換武器
        void SetShootAnimation(bool);	//設定是否播放擊中效果的圖形
        void SetInvincible(bool);	//設定是否為無敵狀態
        void SetGuard(bool);	//設定是否拿到防護
        void SetInjured(bool);	//設定是否為受傷狀態
        void ExecuteInjureEffect();	//執行受傷時的動作
        void ChangeToJumpMode();	//切換到跳躍模式
        void ChangeToNormalMode();	//切換到一般模式
        bool GetMovingDown();	//判斷是否正在下移
        bool GetMovingLeft();	//判斷是否正在左移
        bool GetMovingRight();	//判斷是否正在右移
        bool GetMovingUp();	//判斷是否正在上移
        bool GetJumping();	//判斷是否正在跳躍
        bool GetPressedDown();	//判斷是否已按下下鍵
        bool GetPressedLeft();	//判斷是否已按下左鍵
        bool GetPressedRight();	//判斷是否已按下右鍵
        bool GetPressedUp();	//判斷是否已按下上鍵
        bool GetPressedSpace();	//判斷是否已按下空格鍵
        bool GetBullet();	//判斷是否已得到空氣炮
        bool GetMagic();	//判斷是否已得到魔法
        bool GetBlade();	//判斷是否已得到飛刃
        bool IsTransAnimation();	//判斷是否正在播放切換武器的效果圖形
        bool GetActivedBullet();	//判斷是否已激活空氣炮
        bool GetActivedMagic();	//判斷是否已激活魔法
        bool GetActivedBlade();	//判斷是否已激活飛刃
        bool Over();	//判斷遊戲是否已結束
        bool Dead();	//判斷英雄是否已死亡
        bool GetInvincible();	//判斷是否已激活無敵狀態
        bool GetGuard();	//判斷是否已得到防護
        bool isReadyJump, isJumpBuffer, isFallBuffer, isJumpMode, isNormalMode;
    private:
        CAnimation animation_R,animation_L,animation_F,animation_B;
        CAnimation animationH_R, animationH_L, animationH_F, animationH_B;
        CAnimation animationM_R, animationM_L, animationM_F, animationM_B;
        CAnimation animation_Ri, animation_Li, animation_Fi, animation_Bi;
        CAnimation animationH_Ri, animationH_Li, animationH_Fi, animationH_Bi;
        CAnimation animationM_Ri, animationM_Li, animationM_Fi, animationM_Bi;
        CAnimation animationJump_Ri, animationJump_Li, animationJumpH_Ri, animationJumpH_Li, animationJumpM_Ri, animationJumpM_Li;
        CAnimation st_Ri, st_Li, st_Fi, st_Bi;
        CAnimation stH_Ri, stH_Li, stH_Fi, stH_Bi;
        CAnimation stM_Ri, stM_Li, stM_Fi, stM_Bi,injuredAnimation, injuredAnimationH, injuredAnimationM,jumpR,jumpL;
        CMovingBitmap bmp;
        vector<CMovingBitmap> v_blood, v_life,v_hero_st,v_heroH_st,v_heroM_st, v_weapon,v_dead,v_jump,v_transformation;
        CGameState* game;
        int x, y, step_size, jumpHeight, fallHeight, blood, life, injured_counter, guard_counter, invincible_counter, transformation_counter;
        double reduceValue;
        bool isMovingDown, isMovingLeft, isMovingRight, isMovingUp, isPressedDown, isPressedLeft, isPressedRight,isPressedUp, isShoot, isGetBullet, isGetMagic, isGetBlade, isActivedBullet, isActivedMagic, isActivedBlade,isShootAnimation, isInvincible, isGuard, isInjured, isJumping,isTransAnimation;
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	//判斷是否有碰撞
};

class Bullet
{
    public:
        Bullet(Map* m);	//建構子
        void LoadBitmap();	//載入圖形
        void OnMove(Hero* h,Map* m);	//圖形的功能
        void OnShow(Map* m,Hero* h);	//顯示圖形
        void OnKeyDown(Hero* h);	//設定按下空格鍵
        void SetIsAlive(bool alive);	//設定是否存活
        void SetIsHit(bool flag);	//設定是否已準備好射擊動作
        void SetIsBlood(bool flag);	//設定是否播放噴血效果
        void SetBloodXY(int x, int y);	//設定噴血效果圖形的XY坐標
        void SetXY(Hero* h);	//設定XY坐標
        int GetScreenX(Map* m);	//轉換熒幕的X值
        int GetScreenY(Map* m);	//轉換熒幕的Y值
        int GetX1();	//取得左上角X值
        int GetY1();	//取得左上角Y值
        int GetX2(Hero* h);	//取得右下角X值
        int GetY2(Hero* h);	//取得右下角Y值
        bool IsAlive();	//判斷是否存活
        bool GetShootUp();	//判斷是否向上射擊
        bool GetBlood();	//判斷是否準備播放噴血效果
        bool GetShootDown();	//判斷是否向下射擊
        bool GetShootRight();	//判斷是否向右射擊
        bool GetShootLeft();	//判斷是否向左射擊
        bool HitBlock(Map* m,Hero* h);	//判斷是否射擊到障礙物
        bool OverBoundary(Map* m);	//判斷是否超過邊界
    private:
        bool is_alive;
        bool isShootLeft;
        bool isShootRight;
        bool isShootUp;
        bool isShootDown;
        bool isHit;
        bool isBlood;
        int x, y,bx,by,hx,hy,picDir,showHitCounter,showBloodCounter;
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2,Hero* h);
        CMovingBitmap bullet;
        vector <CMovingBitmap> magic,blood,hit,kongqipao;
        CAnimation Blade;
        Map* m;
};
}