#pragma once
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѥi�H����L�ηƹ�������l
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class Map;
class EnemyShin;
class Enemy;
class Bullet;
class Hero
{
    public:
        Hero();
        int GetX1();					//���o���W�� x �y��
        int GetY1();					//���o���W�� y �y��
        int GetX2();					//���o�k�U�� x �y��
        int GetY2();					//���o�k�U�� y �y��
        int GetBloodAmount();	//���o��e��q
        int GetLifeAmount();	//���o��e�ͩR��
        int GetJumpHeight();	//���o���D����
        int GetStepSize();	//���o�B��
        int GetFootX();	//���o�^���}��X����
        int GetFootY();	//���o�^���}��Y����
        void Initialize();				//�]�w�^������l��
        void RefreshAnimation();	//��s�^�����A
        void LoadBitmap();				//���J�ϧ�
        void OnMove(Map* m);					//���ʭ^��
        void OnShow(Map* m,Bullet* b);					//��ܹϧ�
        void SetMovingDown(bool flag);	//�]�w�O�_���b���U����
        void SetMovingLeft(bool flag);	//�]�w�O�_���b��������
        void SetMovingRight(bool flag); //�]�w�O�_���b���k����
        void SetMovingUp(bool flag);	//�]�w�O�_���b���W����
        void SetPressedDown(bool flag);	//�]�w�O�_����U��
        void SetPressedLeft(bool flag);	//�]�w�O�_���쥪��
        void SetPressedRight(bool flag); //�]�w�O�_����k��
        void SetPressedUp(bool flag);	//�]�w�O�_����W��
        void SetJumping(bool flag);	//�]�����D�Ҧ�
        void SetXY(int nx, int ny);		//�]�wXY�y��
        void SetPressedSpace(bool flag);	//�]�w�O�_���U�Ů���
        void ReduceBlood(int);	//��֦�q
        void ReduceLife();	//��֥ͩR��
        void AddBlood(int);	//�W�[��q
        void AddLife(int);	//�W�[�ͩR��
        void ResetBloodAmount();	//���m��q
        void ResetLifeAmount();	//���m�ͩR��
        void GetedBullet(bool);	//�]�w�O�_�w�o��Ů𬶪Z��
        void GetedMagic(bool);	//�]�w�O�_�w�o���]�k
        void GetedBlade(bool);	//�]�w�O�_�w�o�쭸�b
        void ActivedBullet(bool);	//�]�w�O�_�w�E���Ů�
        void ActivedMagic(bool);	//�]�w�A�O�_�w�E���]�k
        void ActivedBlade(bool);	//�]�w�O�_�w�E�����b
        void ChangeWeapon();	//�����Z��
        void SetShootAnimation(bool);	//�]�w�O�_���������ĪG���ϧ�
        void SetInvincible(bool);	//�]�w�O�_���L�Ī��A
        void SetGuard(bool);	//�]�w�O�_���쨾�@
        void SetInjured(bool);	//�]�w�O�_�����˪��A
        void ExecuteInjureEffect();	//������ˮɪ��ʧ@
        void ChangeToJumpMode();	//��������D�Ҧ�
        void ChangeToNormalMode();	//������@��Ҧ�
        bool GetMovingDown();	//�P�_�O�_���b�U��
        bool GetMovingLeft();	//�P�_�O�_���b����
        bool GetMovingRight();	//�P�_�O�_���b�k��
        bool GetMovingUp();	//�P�_�O�_���b�W��
        bool GetJumping();	//�P�_�O�_���b���D
        bool GetPressedDown();	//�P�_�O�_�w���U�U��
        bool GetPressedLeft();	//�P�_�O�_�w���U����
        bool GetPressedRight();	//�P�_�O�_�w���U�k��
        bool GetPressedUp();	//�P�_�O�_�w���U�W��
        bool GetPressedSpace();	//�P�_�O�_�w���U�Ů���
        bool GetBullet();	//�P�_�O�_�w�o��Ů�
        bool GetMagic();	//�P�_�O�_�w�o���]�k
        bool GetBlade();	//�P�_�O�_�w�o�쭸�b
        bool IsTransAnimation();	//�P�_�O�_���b��������Z�����ĪG�ϧ�
        bool GetActivedBullet();	//�P�_�O�_�w�E���Ů�
        bool GetActivedMagic();	//�P�_�O�_�w�E���]�k
        bool GetActivedBlade();	//�P�_�O�_�w�E�����b
        bool Over();	//�P�_�C���O�_�w����
        bool Dead();	//�P�_�^���O�_�w���`
        bool GetInvincible();	//�P�_�O�_�w�E���L�Ī��A
        bool GetGuard();	//�P�_�O�_�w�o�쨾�@
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
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	//�P�_�O�_���I��
};

class Bullet
{
    public:
        Bullet(Map* m);	//�غc�l
        void LoadBitmap();	//���J�ϧ�
        void OnMove(Hero* h,Map* m);	//�ϧΪ��\��
        void OnShow(Map* m,Hero* h);	//��ܹϧ�
        void OnKeyDown(Hero* h);	//�]�w���U�Ů���
        void SetIsAlive(bool alive);	//�]�w�O�_�s��
        void SetIsHit(bool flag);	//�]�w�O�_�w�ǳƦn�g���ʧ@
        void SetIsBlood(bool flag);	//�]�w�O�_����Q��ĪG
        void SetBloodXY(int x, int y);	//�]�w�Q��ĪG�ϧΪ�XY����
        void SetXY(Hero* h);	//�]�wXY����
        int GetScreenX(Map* m);	//�ഫ������X��
        int GetScreenY(Map* m);	//�ഫ������Y��
        int GetX1();	//���o���W��X��
        int GetY1();	//���o���W��Y��
        int GetX2(Hero* h);	//���o�k�U��X��
        int GetY2(Hero* h);	//���o�k�U��Y��
        bool IsAlive();	//�P�_�O�_�s��
        bool GetShootUp();	//�P�_�O�_�V�W�g��
        bool GetBlood();	//�P�_�O�_�ǳƼ���Q��ĪG
        bool GetShootDown();	//�P�_�O�_�V�U�g��
        bool GetShootRight();	//�P�_�O�_�V�k�g��
        bool GetShootLeft();	//�P�_�O�_�V���g��
        bool HitBlock(Map* m,Hero* h);	//�P�_�O�_�g�����ê��
        bool OverBoundary(Map* m);	//�P�_�O�_�W�L���
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