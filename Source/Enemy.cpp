#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Hero.h"
#include "Enemy.h"
#include "mygame.h"

#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace game_framework {
Enemy::Enemy()
{
    Initialize();
}

void Enemy::Initialize()
{
    is_alive = false;
    x = y = blood = 0;
}

bool Enemy::HitHero()
{
    // 浪代璣动┮篶Θ痻琌窱瞴
    if (!h->GetInvincible() && !h->GetGuard() && is_alive && !h->Dead())
    {
        return HitRectangle(h->GetX1(), h->GetY1(),
                            h->GetX2(), h->GetY2());
    }
    else
        return false;
}

bool Enemy::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
    int x1 = x;				// 瞴オàx畒夹
    int y1 = y;				// 瞴オày畒夹
    int x2 = x1 + bmp.Width();	// 瞴àx畒夹
    int y2 = y1 + bmp.Height();	// 瞴ày畒夹
    //
    // 浪代瞴痻籔把计痻琌Τユ栋
    //
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

bool Enemy::IsAlive()
{
    return is_alive;
}

int Enemy::GetX1()
{
    return x;
}

int Enemy::GetY1()
{
    return y;
}

int Enemy::GetX2()
{
    return x+bmp.Width();
}

int Enemy::GetY2()
{
    return y+bmp.Height();
}

void Enemy::BounceAway()
{
    const int step = 50;
    for (int i = 0; i < step; i++)
    {
        if (b->GetShootUp())
        {
            if (m->isEmpty(GetX1(), GetY1() - 1) && m->isEmpty(GetX2(), GetY1() - 1)) {
                y--;
            }
        }
        else if (b->GetShootDown())
        {
            if (m->isEmpty(GetX1(), GetY2() + 1) && m->isEmpty(GetX2(), GetY2() + 1)) {
                y++;
            }
        }
        else if (b->GetShootLeft())
        {
            if (m->isEmpty(GetX1()-1, GetY1()) && m->isEmpty(GetX1()-1, GetY2())) {
                x--;
            }
        }
        else if (b->GetShootRight())
        {
            if (m->isEmpty(GetX2() + 1, GetY1()) && m->isEmpty(GetX2() + 1, GetY2())) {
                x++;
            }
        }
    }
}

bool Enemy::HitByBullet()
{
    if (!is_alive || !b->IsAlive())
        return false;
    return HitRectangle(b->GetX1(), b->GetY1(), b->GetX2(h), b->GetY2(h));
}

int Enemy::GetBloodAmount()
{
    return blood;
}

void Enemy::BeHit()
{
    if (HitByBullet())
    {
        if(blood==1) {
            f->SetXY(x, y);
            f->SetFruitType((rand() % 15) + 1);
            f->SetIsAlive(true);
            b->SetBloodXY(x-20, y-20);
            b->SetIsBlood(true);
            game->ReduceEnemyCount();
        }
        blood -= 1;
        b->SetIsHit(true);
        if ((h->GetActivedBullet() || h->GetActivedMagic())) {
            b->SetIsAlive(false);
            b->SetIsHit(true);
        }
        if (h->GetActivedMagic()) {
            BounceAway();
        }
        game->AddScore(10);
        isStop = true;
    }
    if (blood < 1) {
        is_alive = false;
    }
}

void Enemy::SetMovingLeft(bool flag)
{
    isMovingLeft = flag;
}

void Enemy::SetMovingRight(bool flag)
{
    isMovingRight = flag;
}

void Enemy::SetMovingUp(bool flag)
{
    isMovingUp = flag;
}

void Enemy::SetMovingDown(bool flag)
{
    isMovingDown = flag;
}

void Enemy::FollowHero(int step)
{
    const int n=8;
    if (h->GetX1() > x+n && m->isEmpty(GetX2(),y) && m->isEmpty(GetX2(),GetY2())) {
        x += step;
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
    }
    else if (h->GetX1() < x-n && m->isEmpty(x, y) && m->isEmpty(x, GetY2())) {
        x -= step;
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
    }
    else if (h->GetY1() > y+n && m->isEmpty(x, GetY2()) && m->isEmpty(GetX2(), GetY2())) {
        y += step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = true;
    }
    else if (h->GetY1() < y-n && m->isEmpty(x, y) && m->isEmpty(GetX2(), y)) {
        y -= step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = true;
        isMovingDown = false;
    }
}

void Enemy::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_hit01);
}

void Enemy::SetIsAlive(bool alive)
{
    is_alive = alive;
}

void Enemy::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

bool Weapon::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
    int width = bmp.Width();
    int height = bmp.Height();
    if (type == "blueFire") {
        width = blueFire.Width();
        height = blueFire.Height();
    }
    int x2 = x + width;	// 子弹的右下角x座標
    int y2 = y + height;	// 子弹的右下角y座標
    //
    // 檢測子弹的矩形與參數矩形是否有交集
    //
    return (tx2 >= x && tx1 <= x2 && ty2 >= y && ty1 <= y2);
}

Weapon::Weapon()
{
    Initialize();
}

Weapon::Weapon(Map* M, Hero* H)
{
    m = M;
    h = H;
    Initialize();
}

void Weapon::Initialize()
{
    x = y = count = 0;
    is_alive = isHit = isShootDown = isShootLeft = isShootRight = isShootUp = false;
    isReadyShoot = true;
    type = "dart";
}

bool Weapon::IsAlive()
{
    return is_alive;
}

void Weapon::LoadBitmap()
{
    char* bluefire[6] = { ".\\bitmaps\\enemy\\25-1.bmp",".\\bitmaps\\enemy\\25-2.bmp",".\\bitmaps\\enemy\\25-3.bmp",
                          ".\\bitmaps\\enemy\\25-4.bmp",".\\bitmaps\\enemy\\25-5.bmp",".\\bitmaps\\enemy\\25-6.bmp"
                        };
    for (int i = 0; i < 6; i++)
    {
        blueFire.AddBitmap(bluefire[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\a1.bmp", RGB(255, 255, 255));
}

void Weapon::OnMove()
{
    if (!is_alive) {
        return;
    }
    const int step = 10;
    if (isShootUp && !isReadyShoot) {
        y -= step;
    }
    else if (isShootDown && !isReadyShoot) {
        y += step;
    }
    else if (isShootRight && !isReadyShoot) {
        x += step;
    }
    else if (isShootLeft && !isReadyShoot) {
        x -= step;
    }
    if (HitBlock() || OverBoundary())					//禬娩 ┪ ゴ毁锚┪璣动
    {
        SetIsAlive(false);
        SetReadyShoot(true);
    }
    if (HitHero()) {
        h->ExecuteInjureEffect();
    }
    blueFire.OnMove();
}

void Weapon::OnShow()
{
    if (!is_alive) {
        return;
    }
    if (type == "blueFire")
    {
        blueFire.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        blueFire.OnShow();
    }
    if (type == "dart")
    {
        bmp.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        bmp.ShowBitmap();
    }
}

void Weapon::SetShootLeft(bool flag)
{
    isShootLeft = flag;
}

void Weapon::SetShootRight(bool flag)
{
    isShootRight = flag;
}

void Weapon::SetShootUp(bool flag)
{
    isShootUp = flag;
}

void Weapon::SetShootDown(bool flag)
{
    isShootDown = flag;
}

void Weapon::SetIsAlive(bool alive)
{
    is_alive = alive;
}

void Weapon::SetIsHit(bool flag)
{
    isHit = flag;
}

void Weapon::SetReadyShoot(bool flag)
{
    isReadyShoot = flag;
}

void Weapon::AddCount()
{
    count++;
}

void Weapon::ResetCount()
{
    count = 0;
}

int Weapon::GetCount()
{
    return count;
}

bool Weapon::GetReadyShoot()
{
    return isReadyShoot;
}

int Weapon::GetScreenX()
{
    return m->ScreenX(x);
}

int Weapon::GetScreenY()
{
    return m->ScreenY(y);
}

int Weapon::GetX1()
{
    return x;
}

int Weapon::GetY1()
{
    return y;
}

int Weapon::GetX2()
{
    return x + bmp.Width();
}

int Weapon::GetY2()
{
    return y + bmp.Height();
}

void Weapon::MoveX(int step)
{
    x += step;
}

void Weapon::MoveY(int step)
{
    y += step;
}

bool Weapon::HitHero()
{
    if (!h->GetInvincible() && !h->GetGuard() && is_alive && !h->Dead())
    {
        return HitRectangle(h->GetX1() + 10, h->GetY1() + 10,
                            h->GetX2() - 10, h->GetY2() - 10);
    }
    return false;
}

bool Weapon::HitBlock()
{
    return (!(m->isEmpty(x, y) && m->isEmpty(GetX2(), GetY2())));
}

bool Weapon::OverBoundary()
{
    return (GetScreenX() < 0 || GetScreenX() > 800 || GetScreenY() < 0 || GetScreenY() > 600);
}

void Weapon::SetXY(int X, int Y)
{
    x = X;
    y = Y;
}

Weapon::~Weapon()
{
}


EnemyShin::EnemyShin(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
    w = new Weapon(m, h);
}

void EnemyShin::Initialize()
{
    x = y = count = stopTime = 0;
    blood = default_blood;
    num = 1;
    is_alive = isStop = false;
}

void EnemyShin::LoadBitmap()
{
    const int a = 6;
    char* shin1[a] = { ".\\bitmaps\\enemy\\dg1.bmp", ".\\bitmaps\\enemy\\dg2.bmp", ".\\bitmaps\\enemy\\dg3.bmp", ".\\bitmaps\\enemy\\dg4.bmp", ".\\bitmaps\\enemy\\dg5.bmp", ".\\bitmaps\\enemy\\dg6.bmp"};
    char* shin2[a] = { ".\\bitmaps\\enemy\\dg7.bmp", ".\\bitmaps\\enemy\\dg8.bmp", ".\\bitmaps\\enemy\\dg9.bmp", ".\\bitmaps\\enemy\\dg10.bmp", ".\\bitmaps\\enemy\\dg11.bmp", ".\\bitmaps\\enemy\\dg12.bmp" };
    for (int i = 0; i < a; i++)
        shinR.AddBitmap(shin1[i], RGB(255, 255, 255));
    for (int i = 0; i < a; i++)
        shinL.AddBitmap(shin2[i], RGB(255, 255, 255));
    bmp.LoadBitmapA(".\\bitmaps\\enemy\\dg1.bmp");
    f->LoadBitmap();
    w->LoadBitmap();
}

void EnemyShin::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    w->OnMove();
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step=4;
    const int step2= 4;
    shinR.SetDelayCount(3);
    if (!(m->isEmpty(x - step, y) && m->isEmpty(x - step, y + bmp.Height())) ||
            !(m->isEmpty(x + step + bmp.Width(), y) && m->isEmpty(x + step + bmp.Width(), y + bmp.Height())) ||
            !(m->isEmpty(x, y - step) && m->isEmpty(x + bmp.Width(), y - step)) ||
            !(m->isEmpty(x, y + step + bmp.Height()) && m->isEmpty(x + bmp.Width(), y + step + bmp.Height())))   //窱毁锚杠
    {
        num = (rand() % 4) + 1;    //繦诀ネΘ1-4い计┕耞璶ǐよ
        count=0;
    }
    if (num == 1) {
        w->AddCount();
        if (m->isEmpty(x - step, y) && m->isEmpty(x - step, y + bmp.Height()))
        {
            SetMovingLeft(true);
            SetMovingRight(false);
            SetMovingUp(false);
            SetMovingDown(false);
            x -= step2;
            count++;
            if (w->GetReadyShoot()) {
                w->SetShootLeft(true);
                w->SetShootRight(false);
                w->SetShootUp(false);
                w->SetShootDown(false);
            }
        }
        if (count > (rand() % 100) + 20) {
            num = (rand() % 4) + 1;    //砞﹚繦诀˙计э跑1-4よ
            count = 0;
        }
    }
    else if (num == 2) {
        w->AddCount();
        if (m->isEmpty(x + step + bmp.Width(), y) && m->isEmpty(x + step + bmp.Width(), y + bmp.Height()))
        {
            SetMovingLeft(false);
            SetMovingRight(true);
            SetMovingUp(false);
            SetMovingDown(false);
            x += step2;
            count++;
            if (w->GetReadyShoot()) {
                w->SetShootLeft(false);
                w->SetShootRight(true);
                w->SetShootUp(false);
                w->SetShootDown(false);
            }
        }
        if (count > (rand()%100)+20) {
            num = (rand() % 4) + 1;
            count = 0;
        }
    }
    else if (num == 3) {
        w->AddCount();
        if(m->isEmpty(x, y - step) && m->isEmpty(x + bmp.Width(), y - step))
        {
            SetMovingLeft(false);
            SetMovingRight(false);
            SetMovingUp(true);
            SetMovingDown(false);
            y -= step2;
            count++;
            if(w->GetReadyShoot()) {
                w->SetShootLeft(false);
                w->SetShootRight(false);
                w->SetShootUp(true);
                w->SetShootDown(false);
            }
        }
        if (count > (rand() % 100) + 20) {
            num = (rand() % 4) + 1;
            count = 0;
        }
    }
    else if (num == 4) {
        w->AddCount();
        if (m->isEmpty(x, y + step + bmp.Height()) && m->isEmpty(x + bmp.Width(), y + step + bmp.Height()))
        {
            SetMovingLeft(false);
            SetMovingRight(false);
            SetMovingUp(false);
            SetMovingDown(true);
            y += step2;
            count++;
            if (w->GetReadyShoot()) {
                w->SetShootLeft(false);
                w->SetShootRight(false);
                w->SetShootUp(false);
                w->SetShootDown(true);
            }
        }
        if (count > (rand() % 100) + 20) {
            num = (rand() % 4) + 1;
            count = 0;
        }
    }
    shinR.OnMove();
    shinL.OnMove();
    if (!w->IsAlive() && w->GetCount() % 15 == 0) {
        w->SetIsAlive(true);
        w->SetXY(x+bmp.Width()/2,y+bmp.Height()/2);
        w->ResetCount();
    }
    if (w->IsAlive()) {
        w->SetReadyShoot(false);
    }
    PlaySoundEffect();
    BeHit();
}

void EnemyShin::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    w->OnShow();
    shinR.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    shinL.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    if(isMovingRight || isMovingDown) {
        shinR.OnShow();
    }
    else {
        shinL.OnShow();
    }
}

void EnemyShin::ResetBlood()
{
    blood = default_blood;
}

EnemyShin::~EnemyShin()
{
    delete f;
    delete w;
}



EnemyMummy::EnemyMummy(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
}

EnemyMummy::~EnemyMummy()
{
    delete f;
}

void EnemyMummy::Initialize()
{
    x = 2960;
    y = 2300;
    distance = count = stopTime = 0;
    blood = default_blood ;
    isMovingRight = true;
    is_alive = isMovingDown = isMovingUp = isMovingLeft = isStop = false;
}

void EnemyMummy::LoadBitmap()
{
    const int a = 4;
    char* mummyf[a] = { ".\\bitmaps\\enemy\\9-1.bmp", ".\\bitmaps\\enemy\\9-2.bmp", ".\\bitmaps\\enemy\\9-3.bmp", ".\\bitmaps\\enemy\\9-4.bmp" };
    char* mummyb[a] = { ".\\bitmaps\\enemy\\9-5.bmp", ".\\bitmaps\\enemy\\9-6.bmp", ".\\bitmaps\\enemy\\9-7.bmp", ".\\bitmaps\\enemy\\9-8.bmp" };
    char* mummyl[a] = { ".\\bitmaps\\enemy\\9-9.bmp", ".\\bitmaps\\enemy\\9-10.bmp", ".\\bitmaps\\enemy\\9-11.bmp", ".\\bitmaps\\enemy\\9-12.bmp"};
    char* mummyr[a] = { ".\\bitmaps\\enemy\\9-13.bmp", ".\\bitmaps\\enemy\\9-14.bmp", ".\\bitmaps\\enemy\\9-15.bmp", ".\\bitmaps\\enemy\\9-16.bmp"};
    for (int i = 0; i < a; i++)
    {
        mummyF.AddBitmap(mummyf[i], RGB(255, 255, 255));
        mummyB.AddBitmap(mummyb[i], RGB(255, 255, 255));
        mummyL.AddBitmap(mummyl[i], RGB(255, 255, 255));
        mummyR.AddBitmap(mummyr[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmapA(".\\bitmaps\\enemy\\9-9.bmp");
    f->LoadBitmap();
}

void EnemyMummy::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 1;
    const int step2 = 3;
    int distanceX = abs(h->GetX1() - x);		//distanceX between enemy and hero
    int distanceY = abs(h->GetY1() - y);		//distanceY between enemy and hero
    if (distanceX <= 400 && distanceY <= 250)
    {
        mummyL.SetDelayCount(3);
        mummyR.SetDelayCount(3);
        mummyB.SetDelayCount(3);
        mummyF.SetDelayCount(3);
        FollowHero(step2);
    }
    else
    {
        mummyL.SetDelayCount(6);
        mummyR.SetDelayCount(6);
        mummyB.SetDelayCount(6);
        mummyF.SetDelayCount(6);
        distance += step;
        if (count % 2 == 0)
        {
            isMovingRight = true;
            isMovingLeft = false;
            isMovingUp = false;
            isMovingDown = false;
            x += step;
            if (!m->isEmpty(GetX2(),y) || !m->isEmpty(GetX2(),GetY2()) || distance>200 )
            {
                distance = 0;
                count++;
            }
        }
        else
        {
            isMovingRight = false;
            isMovingLeft = true;
            isMovingUp = false;
            isMovingDown = false;
            x -= step;
            if (!m->isEmpty(x, y) || !m->isEmpty(x, GetY2()) || distance > 200)
            {
                distance = 0;
                count++;
            }
        }
    }
    PlaySoundEffect();
    BeHit();
    mummyL.OnMove();
    mummyR.OnMove();
    mummyB.OnMove();
    mummyF.OnMove();
}

void EnemyMummy::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    if (is_alive)
    {
        if (isMovingUp) {
            mummyB.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            mummyB.OnShow();
        }
        else if (isMovingDown) {
            mummyF.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            mummyF.OnShow();
        }
        else if (isMovingRight) {
            mummyR.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            mummyR.OnShow();
        }
        else if(isMovingLeft) {
            mummyL.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            mummyL.OnShow();
        }
    }
}

void EnemyMummy::ResetBlood()
{
    blood = default_blood;
}

void EnemyMummy::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_hit11);
}

EnemyFire::EnemyFire(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
}

EnemyFire::~EnemyFire()
{
    delete f;
}

void EnemyFire::Initialize()
{
    x = 2200;
    y = 2300;
    distance = count = stopTime = 0;
    blood = default_blood;
    isMovingRight = true;
    is_alive = isMovingDown = isMovingUp = isMovingLeft = isStop = false;
}

void EnemyFire::FollowHero(int step)
{
    const int n = 8;
    if (h->GetX1()+20 > x + n ) {
        x += step;
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetX1()+20 < x - n) {
        x -= step;
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetY2()-30 > y + n) {
        y += step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = true;
    }
    if (h->GetY2()-30 < y - n ) {
        y -= step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = true;
        isMovingDown = false;
    }
}

void EnemyFire::LoadBitmap()
{
    const int a = 16;
    char* firepic[a] = { ".\\bitmaps\\enemy\\10-1.bmp", ".\\bitmaps\\enemy\\10-2.bmp",".\\bitmaps\\enemy\\10-3.bmp",".\\bitmaps\\enemy\\10-4.bmp",
                         ".\\bitmaps\\enemy\\10-5.bmp",".\\bitmaps\\enemy\\10-6.bmp",".\\bitmaps\\enemy\\10-7.bmp",".\\bitmaps\\enemy\\10-8.bmp",
                         ".\\bitmaps\\enemy\\10-9.bmp",".\\bitmaps\\enemy\\10-10.bmp",".\\bitmaps\\enemy\\10-11.bmp",".\\bitmaps\\enemy\\10-12.bmp",
                         ".\\bitmaps\\enemy\\10-13.bmp",".\\bitmaps\\enemy\\10-14.bmp",".\\bitmaps\\enemy\\10-15.bmp",".\\bitmaps\\enemy\\10-16.bmp",
                       };
    for (int i = 0; i < a; i++)
    {
        fire.AddBitmap(firepic[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\10-1.bmp");
    f->LoadBitmap();
}

void EnemyFire::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 2;
    const int step2 = 3;
    int distanceX = abs(h->GetX1() - x);		//distanceX between enemy and hero
    int distanceY = abs(h->GetY1() - y);		//distanceY between enemy and hero
    if (distanceX <= 300 && distanceY <= 200)
    {
        fire.SetDelayCount(3);
        FollowHero(step2);
    }
    else
    {
        fire.SetDelayCount(6);
        distance += step;
        if (count % 2 == 0)
        {
            isMovingRight = true;
            isMovingLeft = false;
            isMovingUp = false;
            isMovingDown = false;
            x += step;
            if (distance > 400)
            {
                distance = 0;
                count++;
            }
        }
        else
        {
            isMovingRight = false;
            isMovingLeft = true;
            isMovingUp = false;
            isMovingDown = false;
            x -= step;
            if (distance > 400)
            {
                distance = 0;
                count++;
            }
        }
    }
    PlaySoundEffect();
    BeHit();
    if(HitHero()) {
        h->ExecuteInjureEffect();
    }
    fire.OnMove();
}

void EnemyFire::OnShow()
{
    f->OnShow();
    if(!is_alive) {
        return;
    }
    if (is_alive)
    {
        fire.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        fire.OnShow();
    }
}

void EnemyFire::ResetBlood()
{
    blood = 2;
}

void EnemyFire::PlaySoundEffect()
{
    if (HitByBullet())
    {
        CAudio::Instance()->Play(AUDIO_hitfire);
    }
}


EnemyBigFire::EnemyBigFire(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
    for (int i = 0; i < 4; i++)
    {
        smallFire[i] = new EnemyFire(m, h, b);
    }
}

EnemyBigFire::~EnemyBigFire()
{
    delete f;
    for (int i = 0; i < 4; i++)
    {
        delete smallFire[i];
    }
}

void EnemyBigFire::Initialize()
{
    x = 2200;
    y = 2300;
    distance = count = stopTime = 0;
    blood = default_blood;
    isMovingRight = true;
    is_alive = isMovingDown = isMovingUp = isMovingLeft = isStop = false;
}

void EnemyBigFire::FollowHero(int step)
{
    const int n = 8;
    if (h->GetX1() + 20 > x + n) {
        x += step;
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetX1() + 20 < x - n) {
        x -= step;
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetY1() + 30 > y + n) {
        y += step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = true;
    }
    if (h->GetY1() + 30 < y - n) {
        y -= step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = true;
        isMovingDown = false;
    }
}

void EnemyBigFire::LoadBitmap()
{
    const int a = 4;
    char* firepic[a] = { ".\\bitmaps\\enemy\\18-1.bmp", ".\\bitmaps\\enemy\\18-2.bmp",".\\bitmaps\\enemy\\18-3.bmp",".\\bitmaps\\enemy\\18-4.bmp" };
    for (int i = 0; i < a; i++)
    {
        fire.AddBitmap(firepic[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\18-1.bmp");
    f->LoadBitmap();
    for (int i = 0; i < 4; i++)
    {
        smallFire[i]->LoadBitmap();
    }
}

void EnemyBigFire::OnMove()
{
    f->OnMove();
    for (int i = 0; i < 4; i++)
    {
        smallFire[i]->OnMove();
    }
    if (!is_alive) {
        return;
    }
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 2;
    int distanceX = abs(h->GetX1() - x);		//distanceX between enemy and hero
    int distanceY = abs(h->GetY1() - y);		//distanceY between enemy and hero
    distance += step;
    if (count % 2 == 0)
    {
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
        x += step;
        if (distance > 500)
        {
            distance = 0;
            count++;
        }
    }
    else
    {
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
        x -= step;
        if (distance > 500)
        {
            distance = 0;
            count++;
        }
    }
    if (blood == 1)
    {
        smallFire[0]->SetXY(GetX1() - 50, GetY1() - 30);
        smallFire[1]->SetXY(GetX1() - 50, GetY2() + 30);
        smallFire[2]->SetXY(GetX2() + 30, GetY2() + 30);
        smallFire[3]->SetXY(GetX2() + 30, GetY1() - 30);
        for (int i = 0; i <4; i++)
        {
            smallFire[i]->ResetBlood();
            smallFire[i]->SetIsAlive(true);
        }
    }
    PlaySoundEffect();
    BeHit();
    fire.OnMove();
}

void EnemyBigFire::OnShow()
{
    f->OnShow();
    for (int i = 0; i < 4; i++)
    {
        smallFire[i]->OnShow();
    }
    if (!is_alive) {
        return;
    }
    if (is_alive)
    {
        fire.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        fire.OnShow();
    }
}

void EnemyBigFire::ResetBlood()
{
    blood = default_blood;
}

void EnemyBigFire::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_hitfire);
}

EnemyBird::EnemyBird(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
}

EnemyBird::~EnemyBird()
{
    delete f;
}

void EnemyBird::Initialize()
{
    x = 2200;
    y = 2300;
    distance =  count = stopTime = 0;
    blood = default_blood;
    isMovingLeft = true;
    is_alive = isMovingDown = isMovingUp = isMovingRight = isStop = false;
}

void EnemyBird::FollowHero(int step)
{
    const int n = 8;
    if (h->GetX1() > x + n ) {
        x += step;
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetX1() < x - n ) {
        x -= step;
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetY1() > y + n ) {
        y += step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = true;
    }
    if (h->GetY1() < y - n ) {
        y -= step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = true;
        isMovingDown = false;
    }
}

void EnemyBird::LoadBitmap()
{
    const int a = 4;
    char* bird_l[a] = { ".\\bitmaps\\enemy\\11-9.bmp", ".\\bitmaps\\enemy\\11-10.bmp",".\\bitmaps\\enemy\\11-11.bmp",".\\bitmaps\\enemy\\11-12.bmp" };
    char* bird_r[a] = { ".\\bitmaps\\enemy\\11-13.bmp", ".\\bitmaps\\enemy\\11-14.bmp",".\\bitmaps\\enemy\\11-15.bmp",".\\bitmaps\\enemy\\11-16.bmp" };
    for (int i = 0; i < a; i++)
    {
        birdL.AddBitmap(bird_l[i], RGB(255, 255, 255));
        birdR.AddBitmap(bird_r[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\11-9.bmp");
    f->LoadBitmap();
}

void EnemyBird::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 8;
    int distanceX = abs(h->GetX1() - x);
    int distanceY = abs(h->GetY1() - y);
    distance += step;
    if (count % 2 == 0)
    {
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
        x -= step;
        if (!m->isEmpty(x, y) || !m->isEmpty(GetX2(), GetY2()) || distance > 400)
        {
            distance = 0;
            count++;
        }
    }
    else
    {
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
        x += step;
        if (!m->isEmpty(GetX2(), y) || !m->isEmpty(x, GetY2()) || distance > 400)
        {
            distance = 0;
            count++;
        }
    }
    PlaySoundEffect();
    BeHit();
    birdL.OnMove();
    birdR.OnMove();
}

void EnemyBird::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    if (isMovingUp || isMovingRight) {
        birdR.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        birdR.OnShow();
    }
    else if(isMovingDown || isMovingLeft) {
        birdL.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        birdL.OnShow();
    }
}

void EnemyBird::ResetBlood()
{
    blood = default_blood;
}

void EnemyBird::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_bird);
}


EnemyBirdVertical::EnemyBirdVertical(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
}

EnemyBirdVertical::~EnemyBirdVertical()
{
    delete f;
}

void EnemyBirdVertical::Initialize()
{
    x = 2200;
    y = 2300;
    distance = count = stopTime = 0;
    blood = default_blood;
    isMovingUp = true;
    is_alive = isMovingDown = isMovingRight = isMovingRight = isStop = false;
}

void EnemyBirdVertical::FollowHero(int step)
{
    const int n = 8;
    if (h->GetX1() > x + n) {
        x += step;
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetX1() < x - n) {
        x -= step;
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetY1() > y + n) {
        y += step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = true;
    }
    if (h->GetY1() < y - n) {
        y -= step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = true;
        isMovingDown = false;
    }
}

void EnemyBirdVertical::LoadBitmap()
{
    const int a = 4;
    char* bird_d[a] = { ".\\bitmaps\\enemy\\11-1.bmp", ".\\bitmaps\\enemy\\11-2.bmp",".\\bitmaps\\enemy\\11-3.bmp",".\\bitmaps\\enemy\\11-4.bmp" };
    char* bird_u[a] = { ".\\bitmaps\\enemy\\11-5.bmp", ".\\bitmaps\\enemy\\11-6.bmp",".\\bitmaps\\enemy\\11-7.bmp",".\\bitmaps\\enemy\\11-8.bmp" };
    for (int i = 0; i < a; i++)
    {
        birdD.AddBitmap(bird_d[i], RGB(255, 255, 255));
        birdU.AddBitmap(bird_u[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\11-1.bmp");
    f->LoadBitmap();
}

void EnemyBirdVertical::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 7;
    int distanceX = abs(h->GetX1() - x);
    int distanceY = abs(h->GetY1() - y);
    distance += step;
    if (count % 2 == 0)
    {
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = true;
        isMovingDown = false;
        y -= step;
        if (!m->isEmpty(x, y) || !m->isEmpty(GetX2(), y) || distance > 600)
        {
            distance = 0;
            count++;
        }
    }
    else
    {
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = true;
        y += step;
        if (!m->isEmpty(x, GetY2()) || !m->isEmpty(GetX2(), GetY2()) || distance > 600)
        {
            distance = 0;
            count++;
        }
    }
    PlaySoundEffect();
    BeHit();
    birdD.OnMove();
    birdU.OnMove();
}

void EnemyBirdVertical::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    if (isMovingUp) {
        birdU.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        birdU.OnShow();
    }
    else if (isMovingDown) {
        birdD.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        birdD.OnShow();
    }
}

void EnemyBirdVertical::ResetBlood()
{
    blood = default_blood;
}

void EnemyBirdVertical::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_bird);
}

EnemyPig::EnemyPig(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
}

EnemyPig::~EnemyPig()
{
    delete f;
}

void EnemyPig::Initialize()
{
    x = 2200;
    y = 2300;
    distance = count = stopTime = 0;
    blood = default_blood;
    isMovingRight = true;
    is_alive = isMovingDown = isMovingUp = isMovingLeft = isStop = false;
}

void EnemyPig::FollowHero(int step)
{
    const int n = 8;
    if (h->GetX1() > x + n) {
        x += step;
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetX1() < x - n) {
        x -= step;
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetY1() > y + n) {
        y += step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = true;
    }
    if (h->GetY1() < y - n) {
        y -= step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = true;
        isMovingDown = false;
    }
}

void EnemyPig::LoadBitmap()
{
    const int a = 4;
    char* pig_l[a] = { ".\\bitmaps\\enemy\\8-1.bmp", ".\\bitmaps\\enemy\\8-2.bmp",".\\bitmaps\\enemy\\8-3.bmp",".\\bitmaps\\enemy\\8-4.bmp" };
    char* pig_r[a] = { ".\\bitmaps\\enemy\\8-5.bmp", ".\\bitmaps\\enemy\\8-6.bmp",".\\bitmaps\\enemy\\8-7.bmp",".\\bitmaps\\enemy\\8-8.bmp" };
    for (int i = 0; i < a; i++)
    {
        pigL.AddBitmap(pig_l[i], RGB(255, 255, 255));
        pigR.AddBitmap(pig_r[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\8-1.bmp");
    f->LoadBitmap();
}

void EnemyPig::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    pigL.SetDelayCount(2);
    pigR.SetDelayCount(2);
    const int step = 5;
    int distanceX = abs(h->GetX1() - x);		//distanceX between enemy and hero
    int distanceY = abs(h->GetY1() - y);		//distanceY between enemy and hero
    distance += step;
    if (count % 2 == 0)
    {
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
        x += step;
        if (!m->isEmpty(GetX2(), y) || !m->isEmpty(GetX2(), GetY2()) || distance > 400)
        {
            distance = 0;
            count++;
        }
    }
    else
    {
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
        x -= step;
        if (!m->isEmpty(x, y) || !m->isEmpty(x, GetY2()) || distance > 400)
        {
            distance = 0;
            count++;
        }
    }
    PlaySoundEffect();
    BeHit();
    pigL.OnMove();
    pigR.OnMove();
}

void EnemyPig::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    if (isMovingUp || isMovingRight) {
        pigR.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        pigR.OnShow();
    }
    else if (isMovingDown || isMovingLeft) {
        pigL.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        pigL.OnShow();
    }
}

void EnemyPig::ResetBlood()
{
    blood = default_blood;
}

void EnemyPig::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_pig);
}

EnemyBossMan::EnemyBossMan(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
}

void EnemyBossMan::Initialize()
{
    for (int i = 0; i < 8; i++) {
        fire[i] = NULL;
    }
    for (int i = 0; i < 6; i++) {
        crescent[i] = NULL;
    }
    x = 65;
    y = 400;
    distance = 0;
    counter = hurt_t = dead_t = 0;
    blood = default_blood;
    is_alive = isAttackR = isSpray = isMovingRight = isMovingDown = isMovingUp = isMovingLeft = isAttackL = isdead = ishurt = false;
}

void EnemyBossMan::LoadBitmap()
{
    char* picspray[4] = { ".\\bitmaps\\enemy\\bossman\\0-1.bmp", ".\\bitmaps\\enemy\\bossman\\0-2.bmp", ".\\bitmaps\\enemy\\bossman\\0-3.bmp", ".\\bitmaps\\enemy\\bossman\\0-4.bmp" };
    char* picr[5] = { ".\\bitmaps\\enemy\\bossman\\1-1.bmp", ".\\bitmaps\\enemy\\bossman\\1-2.bmp", ".\\bitmaps\\enemy\\bossman\\1-3.bmp", ".\\bitmaps\\enemy\\bossman\\1-4.bmp", ".\\bitmaps\\enemy\\bossman\\1-5.bmp" };
    char* picl[5] = { ".\\bitmaps\\enemy\\bossman\\2-1.bmp", ".\\bitmaps\\enemy\\bossman\\2-2.bmp", ".\\bitmaps\\enemy\\bossman\\2-3.bmp", ".\\bitmaps\\enemy\\bossman\\2-4.bmp", ".\\bitmaps\\enemy\\bossman\\2-5.bmp" };
    char* picar[12] = { ".\\bitmaps\\enemy\\bossman\\3-1.bmp", ".\\bitmaps\\enemy\\bossman\\3-2.bmp", ".\\bitmaps\\enemy\\bossman\\3-3.bmp", ".\\bitmaps\\enemy\\bossman\\3-4.bmp",
                        ".\\bitmaps\\enemy\\bossman\\3-5.bmp", ".\\bitmaps\\enemy\\bossman\\3-6.bmp", ".\\bitmaps\\enemy\\bossman\\3-7.bmp", ".\\bitmaps\\enemy\\bossman\\3-8.bmp",
                        ".\\bitmaps\\enemy\\bossman\\3-9.bmp", ".\\bitmaps\\enemy\\bossman\\3-10.bmp", ".\\bitmaps\\enemy\\bossman\\3-11.bmp", ".\\bitmaps\\enemy\\bossman\\3-12.bmp"
                      };
    char* pical[12] = { ".\\bitmaps\\enemy\\bossman\\4-1.bmp", ".\\bitmaps\\enemy\\bossman\\4-2.bmp", ".\\bitmaps\\enemy\\bossman\\4-3.bmp", ".\\bitmaps\\enemy\\bossman\\3-4.bmp",
                        ".\\bitmaps\\enemy\\bossman\\4-5.bmp", ".\\bitmaps\\enemy\\bossman\\4-6.bmp", ".\\bitmaps\\enemy\\bossman\\4-7.bmp", ".\\bitmaps\\enemy\\bossman\\4-8.bmp",
                        ".\\bitmaps\\enemy\\bossman\\4-9.bmp", ".\\bitmaps\\enemy\\bossman\\4-10.bmp", ".\\bitmaps\\enemy\\bossman\\4-11.bmp", ".\\bitmaps\\enemy\\bossman\\4-12.bmp"
                      };
    char* picspray_h[4] = { ".\\bitmaps\\enemy\\bossman\\i\\0-1.bmp", ".\\bitmaps\\enemy\\bossman\\i\\0-2.bmp", ".\\bitmaps\\enemy\\bossman\\i\\0-3.bmp", ".\\bitmaps\\enemy\\bossman\\i\\0-4.bmp" };
    char* picr_h[5] = { ".\\bitmaps\\enemy\\bossman\\i\\1-1.bmp", ".\\bitmaps\\enemy\\bossman\\i\\1-2.bmp", ".\\bitmaps\\enemy\\bossman\\i\\1-3.bmp", ".\\bitmaps\\enemy\\bossman\\i\\1-4.bmp", ".\\bitmaps\\enemy\\bossman\\i\\1-5.bmp" };
    char* picl_h[5] = { ".\\bitmaps\\enemy\\bossman\\i\\2-1.bmp", ".\\bitmaps\\enemy\\bossman\\i\\2-2.bmp", ".\\bitmaps\\enemy\\bossman\\i\\2-3.bmp", ".\\bitmaps\\enemy\\bossman\\i\\2-4.bmp", ".\\bitmaps\\enemy\\bossman\\i\\2-5.bmp" };
    char* picar_h[12] = { ".\\bitmaps\\enemy\\bossman\\i\\3-1.bmp", ".\\bitmaps\\enemy\\bossman\\i\\3-2.bmp", ".\\bitmaps\\enemy\\bossman\\i\\3-3.bmp", ".\\bitmaps\\enemy\\bossman\\i\\3-4.bmp",
                          ".\\bitmaps\\enemy\\bossman\\i\\3-5.bmp", ".\\bitmaps\\enemy\\bossman\\i\\3-6.bmp", ".\\bitmaps\\enemy\\bossman\\i\\3-7.bmp", ".\\bitmaps\\enemy\\bossman\\i\\3-8.bmp",
                          ".\\bitmaps\\enemy\\bossman\\i\\3-9.bmp", ".\\bitmaps\\enemy\\bossman\\i\\3-10.bmp", ".\\bitmaps\\enemy\\bossman\\i\\3-11.bmp", ".\\bitmaps\\enemy\\bossman\\i\\3-12.bmp"
                        };
    char* pical_h[12] = { ".\\bitmaps\\enemy\\bossman\\i\\4-1.bmp", ".\\bitmaps\\enemy\\bossman\\i\\4-2.bmp", ".\\bitmaps\\enemy\\bossman\\i\\4-3.bmp", ".\\bitmaps\\enemy\\bossman\\i\\3-4.bmp",
                          ".\\bitmaps\\enemy\\bossman\\i\\4-5.bmp", ".\\bitmaps\\enemy\\bossman\\i\\4-6.bmp", ".\\bitmaps\\enemy\\bossman\\i\\4-7.bmp", ".\\bitmaps\\enemy\\bossman\\i\\4-8.bmp",
                          ".\\bitmaps\\enemy\\bossman\\i\\4-9.bmp", ".\\bitmaps\\enemy\\bossman\\i\\4-10.bmp", ".\\bitmaps\\enemy\\bossman\\i\\4-11.bmp", ".\\bitmaps\\enemy\\bossman\\i\\4-12.bmp"
                        };
    char* picdead[23]=
    {   ".\\bitmaps\\enemy\\bossman\\7-1.bmp", ".\\bitmaps\\enemy\\bossman\\7-2.bmp",".\\bitmaps\\enemy\\bossman\\7-3.bmp", ".\\bitmaps\\enemy\\bossman\\7-4.bmp",
        ".\\bitmaps\\enemy\\bossman\\7-5.bmp", ".\\bitmaps\\enemy\\bossman\\7-6.bmp", ".\\bitmaps\\enemy\\bossman\\7-7.bmp", ".\\bitmaps\\enemy\\bossman\\7-8.bmp",
        ".\\bitmaps\\enemy\\bossman\\7-9.bmp", ".\\bitmaps\\enemy\\bossman\\7-10.bmp", ".\\bitmaps\\enemy\\bossman\\7-11.bmp", ".\\bitmaps\\enemy\\bossman\\7-12.bmp",
        ".\\bitmaps\\enemy\\bossman\\7-13.bmp", ".\\bitmaps\\enemy\\bossman\\7-14.bmp", ".\\bitmaps\\enemy\\bossman\\7-15.bmp", ".\\bitmaps\\enemy\\bossman\\7-16.bmp",
        ".\\bitmaps\\enemy\\bossman\\7-17.bmp", ".\\bitmaps\\enemy\\bossman\\7-18.bmp", ".\\bitmaps\\enemy\\bossman\\7-19.bmp", ".\\bitmaps\\enemy\\bossman\\7-20.bmp",
        ".\\bitmaps\\enemy\\bossman\\7-21.bmp", ".\\bitmaps\\enemy\\bossman\\7-22.bmp", ".\\bitmaps\\enemy\\bossman\\7-23.bmp"
    };
    for (int i = 0; i < 4; i++)
    {
        spray.AddBitmap(picspray[i], RGB(255, 255, 255));
        spray_h.AddBitmap(picspray_h[i], RGB(255, 255, 255));
    }
    for (int i = 0; i < 5; i++)
    {
        right.AddBitmap(picr[i], RGB(255, 255, 255));
        left.AddBitmap(picl[i], RGB(255, 255, 255));
        right_h.AddBitmap(picr_h[i], RGB(255, 255, 255));
        left_h.AddBitmap(picl_h[i], RGB(255, 255, 255));
    }
    for (int i = 0; i < 12; i++)
    {
        attackR.AddBitmap(picar[i], RGB(255, 255, 255));
        attackL.AddBitmap(pical[i], RGB(255, 255, 255));
        attackR_h.AddBitmap(picar_h[i], RGB(255, 255, 255));
        attackL_h.AddBitmap(pical_h[i], RGB(255, 255, 255));
    }
    for (int i = 0; i < 23; i++)
    {
        dead.AddBitmap(picdead[i], RGB(255, 255, 255));
    }
    dead_pic.LoadBitmap(".\\bitmaps\\enemy\\bossman\\7-23.bmp");
    bmp.LoadBitmap(".\\bitmaps\\enemy\\bossman\\1-1.bmp");
}

void EnemyBossMan::OnMove()
{
    if (isdead) {
        dead_t++;
        dead.OnMove();
    }
    if (dead_t > 200) {
        isdead = false;
        dead_t = 0;
    }
    if (!is_alive) {
        return;
    }
    counter++;
    right.SetDelayCount(2);
    left.SetDelayCount(2);
    attackL.SetDelayCount(1);
    attackR.SetDelayCount(1);
    right_h.SetDelayCount(2);
    left_h.SetDelayCount(2);
    attackL_h.SetDelayCount(1);
    attackR_h.SetDelayCount(1);
    right.OnMove();
    left.OnMove();
    attackL.OnMove();
    attackR.OnMove();
    spray.OnMove();
    right_h.OnMove();
    left_h.OnMove();
    attackL_h.OnMove();
    attackR_h.OnMove();
    spray_h.OnMove();
    if (!ishurt) {
        BeHit();
    }
    if (ishurt) {
        hurt_t++;
    }
    if (hurt_t > 90) {
        hurt_t = 0;
        ishurt = false;
    }
    if (counter == 50 || counter == 300) {
        SprayFire();
        CAudio::Instance()->Play(AUDIO_manshout);
    }
    if (counter == 156 || counter == 171 || counter == 186 || counter == 201 || counter == 216 || counter == 231
            || counter == 406 || counter == 421 || counter == 436 || counter == 451 || counter == 466 || counter == 481) {
        CAudio::Instance()->Play(AUDIO_manshout);
    }
    if (counter == 152 || counter == 402) {
        ThrowWeapon();
    }
    if (counter > 50 && counter < 250 || counter>300 && counter < 500) {
        MoveFire();
    }
    if (counter > 152 && counter < 300 || counter> 402 && counter < 500) {
        MoveWeapon();
    }
    if (counter > 0 && counter <= 50) {
        isMovingRight = true;
        x += 7;
    }
    if (counter > 50 && counter <= 80) {
        isSpray = true;
        isMovingRight = false;
    }
    if (counter > 100 && counter <= 150) {
        isMovingRight = true;
        isSpray = false;
        x += 7;
    }
    if (counter > 150 && counter <= 250) {
        isMovingRight = false;
        isAttackL = true;
    }
    if (counter > 250 && counter <= 300) {
        isAttackL = false;
        isMovingLeft = true;
        x -= 7;
    }
    if (counter > 300 && counter <= 350) {
        isSpray = true;
        isMovingLeft = false;
    }
    if (counter > 350 && counter <= 400) {
        isMovingLeft = true;
        isSpray = false;
        x -= 7;
    }
    if (counter > 400 && counter <= 500) {
        isMovingLeft = false;
        isAttackR = true;
    }
    if (counter > 500) {
        counter = 0;
        isAttackR = false;
    }
}

void EnemyBossMan::OnShow()
{
    if (dead_t>0 && dead_t<=60) {
        dead.SetTopLeft(m->ScreenX(x-100), m->ScreenY(y-20));
        dead.OnShow();
    }
    if (dead_t > 60 && dead_t < 200) {
        dead_pic.SetTopLeft(m->ScreenX(x-100), m->ScreenY(y-20));
        dead_pic.ShowBitmap();
    }
    if (!is_alive) {
        return;
    }
    if (!ishurt)		//show unhurt animation
    {
        if (isMovingRight) {
            right.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            right.OnShow();
        }
        else if (isMovingLeft) {
            left.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            left.OnShow();
        }
        else if (isAttackR) {
            attackR.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            attackR.OnShow();
        }
        else if (isAttackL) {
            attackL.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            attackL.OnShow();
        }
        else if (isSpray) {
            spray.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            spray.OnShow();
        }
    }
    else       //show hurt animation
    {
        if (isMovingRight) {
            right_h.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            right_h.OnShow();
        }
        else if (isMovingLeft) {
            left_h.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            left_h.OnShow();
        }
        else if (isAttackR) {
            attackR_h.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            attackR_h.OnShow();
        }
        else if (isAttackL) {
            attackL_h.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            attackL_h.OnShow();
        }
        else if (isSpray) {
            spray_h.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            spray_h.OnShow();
        }
    }
    if (counter > 50 && counter < 250 || counter > 300 && counter < 500) {
        ShowFire();
    }
    if (counter > 152 && counter < 300 || counter> 402 && counter < 500) {
        ShowWeapon();
    }
}

void EnemyBossMan::BeHit()
{
    if (HitByBullet())
    {
        ishurt = true;
        if (blood == 1) {
            isdead = true;
            CAudio::Instance()->Play(AUDIO_win);
        }
        blood -= 1;
        b->SetIsHit(true);
        if ((h->GetActivedBullet() || h->GetActivedMagic())) {
            b->SetIsAlive(false);
            b->SetIsHit(true);
        }
        CAudio::Instance()->Play(AUDIO_manhurt);
        game->AddScore(10);
    }
    if (blood < 1) {
        is_alive = false;
    }
}

void EnemyBossMan::SprayFire()
{
    const int a = 8;
    for (int i = 0; i < a; i++)
        delete fire[i];
    for (int i = 0; i < a; i++) {
        fire[i] = new WeaponFire(m, h);
    }
    for (int i = 0; i < a; i++) {
        fire[i]->LoadBitmap();
        fire[i]->SetXY(x + (bmp.Width() / 2), y + (bmp.Height() / 2));
        fire[i]->SetIsAlive(true);
    }
}

void EnemyBossMan::MoveFire()
{
    for(int i=0; i<8; i++)
        fire[i]->OnMove();
    if(fire[0]->IsAlive())
        fire[0]->MoveXY(-18, 0);
    if (fire[1]->IsAlive())
        fire[1]->MoveXY(18, 0);
    if (fire[2]->IsAlive())
        fire[2]->MoveXY(0, 18);
    if (fire[3]->IsAlive())
        fire[3]->MoveXY(0, -18);
    if (fire[4]->IsAlive())
        fire[4]->MoveXY(-14, -14);
    if (fire[5]->IsAlive())
        fire[5]->MoveXY(14, 14);
    if (fire[6]->IsAlive())
        fire[6]->MoveXY(-14, 14);
    if (fire[7]->IsAlive())
        fire[7]->MoveXY(14, -14);
}

void EnemyBossMan::ShowFire()
{
    for (int i = 0; i < 8; i++) {
        fire[i]->OnShow();
    }
}

void EnemyBossMan::ThrowWeapon()
{
    const int a = 6;
    for (int i = 0; i < a; i++)
        delete crescent[i];
    for (int i = 0; i < a; i++) {
        crescent[i] = new WeaponCrescent(m, h);
    }
    for (int i = 0; i < a; i++) {
        crescent[i]->LoadBitmap();
        if(isAttackL) {
            crescent[i]->SetXY(x-30, y);
        }
        if(isAttackR) {
            crescent[i]->SetXY(GetX2()-50,y);
        }
        crescent[i]->SetIsAlive(true);
    }
}

void EnemyBossMan::MoveWeapon()
{
    for (int i = 0; i < 6; i++)
    {
        crescent[i]->OnMove();
    }
    if (counter > 155 && counter < 300) {
        crescent[0]->MoveX(-20);
    }
    if (counter > 170 && counter < 300) {
        crescent[1]->MoveX(-20);
    }
    if (counter > 185 && counter < 300) {
        crescent[2]->MoveX(-20);
    }
    if (counter > 200 && counter < 300) {
        crescent[3]->MoveX(-20);
    }
    if (counter > 215 && counter < 300) {
        crescent[4]->MoveX(-20);
    }
    if (counter > 230 && counter < 300) {
        crescent[5]->MoveX(-20);
    }
    if (counter > 405 && counter < 500) {
        crescent[0]->MoveX(20);
    }
    if (counter > 420 && counter < 500) {
        crescent[1]->MoveX(20);
    }
    if (counter > 435 && counter < 500) {
        crescent[2]->MoveX(20);
    }
    if (counter > 450 && counter < 500) {
        crescent[3]->MoveX(20);
    }
    if (counter > 465 && counter < 500) {
        crescent[4]->MoveX(20);
    }
    if (counter > 480 && counter < 500) {
        crescent[5]->MoveX(20);
    }
}

void EnemyBossMan::ShowWeapon()
{
    if (counter > 155 && counter < 300) {
        crescent[0]->OnShowL();
    }
    if (counter > 170 && counter < 300) {
        crescent[1]->OnShowL();
    }
    if (counter > 185 && counter < 300) {
        crescent[2]->OnShowL();
    }
    if (counter > 200 && counter < 300) {
        crescent[3]->OnShowL();
    }
    if (counter > 215 && counter < 300) {
        crescent[4]->OnShowL();
    }
    if (counter > 230 && counter < 300) {
        crescent[5]->OnShowL();
    }
    if (counter > 405 && counter < 500) {
        crescent[0]->OnShowR();
    }
    if (counter > 420 && counter < 500) {
        crescent[1]->OnShowR();
    }
    if (counter > 435 && counter < 500) {
        crescent[2]->OnShowR();
    }
    if (counter > 450 && counter < 500) {
        crescent[3]->OnShowR();
    }
    if (counter > 465 && counter < 500) {
        crescent[4]->OnShowR();
    }
    if (counter > 480 && counter < 500) {
        crescent[5]->OnShowR();
    }
}

EnemyBossMan::~EnemyBossMan()
{
    for (int i = 0; i < 8; i++)
    {
        delete fire[i];
    }
    for (int i = 0; i < 6; i++)
    {
        delete crescent[i];
    }
}

bool EnemyBossMan::HitByBullet()
{
    if (!is_alive)
        return false;
    if (b->IsAlive() == true)
    {
        return HitRectangle(b->GetX1(), b->GetY1(), b->GetX2(h), b->GetY2(h));
    }
    else
    {
        return false;
    }
}

bool EnemyBossMan::HitHero()
{
    if (ishurt) {
        return false;
    }
    if (!h->GetInvincible() && !h->GetGuard() && is_alive && !h->Dead())
    {
        return HitRectangle(h->GetX1(), h->GetY1(),
                            h->GetX2(), h->GetY2());
    }
    else
        return false;
}

void EnemyBossMan::ResetBlood()
{
    blood = default_blood;
}

void EnemyBossMan::ReduceBlood(int b)
{
    blood -= b;
}

bool EnemyBossMan::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
    int x1 = x+50;
    int y1 = y+20;
    int x2 = x1 + bmp.Width()-120;
    int y2 = y1 + bmp.Height()-40;
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
    return false;
}


WeaponFire::WeaponFire(Map* M, Hero* H)
{
    m = M;
    h = H;
    Initialize();
    is_alive = false;
}

void WeaponFire::LoadBitmap()
{
    char* fire_pic[3] = { ".\\bitmaps\\enemy\\bossman\\8-1.bmp",".\\bitmaps\\enemy\\bossman\\8-2.bmp",".\\bitmaps\\enemy\\bossman\\8-3.bmp" };
    for (int i = 0; i < 3; i++)
    {
        fire.AddBitmap(fire_pic[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\bossman\\8-1.bmp");
}

void WeaponFire::OnMove()
{
    if (!is_alive) {
        return;
    }
    fire.SetDelayCount(2);
    fire.OnMove();
    if (HitHero() || HitBlock())
    {
        if (HitHero()) {
            h->ExecuteInjureEffect();
        }
        else {
            is_alive = false;
        }
    }
}

void WeaponFire::OnShow()
{
    if (!is_alive) {
        return;
    }
    fire.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    fire.OnShow();
}

void WeaponFire::SetXY(int X, int Y)
{
    x = X;
    y = Y;
}

void WeaponFire::MoveXY(int X, int Y)
{
    x += X;
    y += Y;
}

WeaponCrescent::WeaponCrescent(Map* M, Hero* H)
{
    m = M;
    h = H;
    Initialize();
    is_alive = false;
}

void WeaponCrescent::LoadBitmap()
{
    bmp.LoadBitmap(".\\bitmaps\\enemy\\bossman\\6-1.bmp",RGB(255,255,255));
    bmpL.LoadBitmap(".\\bitmaps\\enemy\\bossman\\6-1.bmp", RGB(255, 255, 255));
    bmpR.LoadBitmap(".\\bitmaps\\enemy\\bossman\\6-2.bmp", RGB(255, 255, 255));
}

void WeaponCrescent::OnShowL()
{
    if (!is_alive) {
        return;
    }
    bmpL.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    bmpL.ShowBitmap();
}

void WeaponCrescent::OnShowR()
{
    if (!is_alive) {
        return;
    }
    bmpR.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    bmpR.ShowBitmap();
}

void WeaponCrescent::OnMove()
{
    if (!is_alive) {
        return;
    }
    if (HitHero() || HitBlock())
    {
        if (HitHero()) {
            h->ExecuteInjureEffect();
        }
        is_alive = false;
    }
}

void WeaponCrescent::SetXY(int X, int Y)
{
    x = X;
    y = Y;
}
void WeaponCrescent::MoveX(int X)
{
    x += X;
}

EnemyBooger::EnemyBooger(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
}
EnemyBooger::~EnemyBooger()
{
    delete f;
}
void EnemyBooger::Initialize()
{
    x = y = distance = count = stopTime = 0;
    blood = default_blood;
    isMovingRight = true;
    is_alive = isMovingDown = isMovingUp = isMovingLeft = isStop = false;
}
void EnemyBooger::LoadBitmap()
{
    const int a = 3;
    char* booger_L[a] = { ".\\bitmaps\\enemy\\3-1.bmp",".\\bitmaps\\enemy\\3-2.bmp",".\\bitmaps\\enemy\\3-3.bmp" };
    char* booger_R[a] = { ".\\bitmaps\\enemy\\3-4.bmp",".\\bitmaps\\enemy\\3-5.bmp",".\\bitmaps\\enemy\\3-6.bmp" };
    for (int i = 0; i < a; i++)
    {
        boogerR.AddBitmap(booger_R[i], RGB(255, 255, 255));
        boogerL.AddBitmap(booger_L[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\3-1.bmp", RGB(255, 255, 255));
    f->LoadBitmap();
}
void EnemyBooger::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 6;
    int distanceX = abs(h->GetX1() - x);		//distanceX between enemy and hero
    int distanceY = abs(h->GetY1() - y);		//distanceY between enemy and hero
    distance += step;
    if (count % 2 == 0)
    {
        isMovingRight = true;
        isMovingLeft = false;
        x += step;
        if (!m->isEmpty(GetX2(), y) || !m->isEmpty(GetX2(), GetY2()) || distance > 300)
        {
            distance = 0;
            count++;
        }
    }
    else
    {
        isMovingRight = false;
        isMovingLeft = true;
        x -= step;
        if (!m->isEmpty(x, y) || !m->isEmpty(x, GetY2()) || distance > 300)
        {
            distance = 0;
            count++;
        }
    }
    PlaySoundEffect();
    BeHit();
    boogerR.OnMove();
    boogerL.OnMove();
}
void EnemyBooger::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    boogerR.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    boogerL.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    if (isMovingRight) {
        boogerR.OnShow();
    }
    if (isMovingLeft) {
        boogerL.OnShow();
    }
}
void EnemyBooger::ResetBlood()
{
    blood = default_blood;
}

void EnemyBooger::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_hitfrog);
}

EnemyMucus::EnemyMucus(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
}

EnemyMucus::~EnemyMucus()
{
    delete f;
}

void EnemyMucus::Initialize()
{
    x = y = distance = count = stopTime = 0;
    blood = default_blood;
    isMovingLeft = true;
    is_alive = isMovingDown = isMovingUp = isMovingRight = isStop = false;
}

void EnemyMucus::LoadBitmap()
{
    const int a = 4;
    char* booger_L[a] = { ".\\bitmaps\\enemy\\5-1.bmp",".\\bitmaps\\enemy\\5-2.bmp",".\\bitmaps\\enemy\\5-3.bmp",".\\bitmaps\\enemy\\5-4.bmp" };
    char* booger_R[a] = { ".\\bitmaps\\enemy\\5-5.bmp",".\\bitmaps\\enemy\\5-6.bmp",".\\bitmaps\\enemy\\5-7.bmp",".\\bitmaps\\enemy\\5-8.bmp" };
    for (int i = 0; i < a; i++)
    {
        mucusR.AddBitmap(booger_R[i], RGB(255, 255, 255));
        mucusL.AddBitmap(booger_L[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\5-1.bmp", RGB(255, 255, 255));
    f->LoadBitmap();
}

void EnemyMucus::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 5;
    int distanceX = abs(h->GetX1() - x);		//distanceX between enemy and hero
    int distanceY = abs(h->GetY1() - y);		//distanceY between enemy and hero
    distance += step;
    if (count % 2 == 0)
    {
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
        x -= step;
        if (!m->isEmpty(x, y) || !m->isEmpty(GetX2(), GetY2()) || distance > 1000)
        {
            distance = 0;
            count++;
        }
    }
    else
    {
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
        x += step;
        if (!m->isEmpty(GetX2(), y) || !m->isEmpty(x, GetY2()) || distance > 1000)
        {
            distance = 0;
            count++;
        }
    }
    PlaySoundEffect();
    BeHit();
    mucusR.OnMove();
    mucusL.OnMove();
}

void EnemyMucus::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    mucusR.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    mucusL.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    if (isMovingRight) {
        mucusR.OnShow();
    }
    if (isMovingLeft) {
        mucusL.OnShow();
    }
}

void EnemyMucus::ResetBlood()
{
    blood = default_blood;
}

void EnemyMucus::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_hit07);
}

EnemyBat::EnemyBat(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
}

EnemyBat::~EnemyBat()
{
    delete f;
}

void EnemyBat::Initialize()
{
    x = y = distance = count = stopTime = 0;
    blood = default_blood;
    isMovingRight = true;
    is_alive = isMovingDown = isMovingUp = isMovingLeft = isStop = false;
}

void EnemyBat::LoadBitmap()
{
    char* file[4] = { ".\\bitmaps\\enemy\\22-1.bmp",".\\bitmaps\\enemy\\22-2.bmp",".\\bitmaps\\enemy\\22-3.bmp",".\\bitmaps\\enemy\\22-4.bmp" };
    for (int i = 0; i < 4; i++)
    {
        bat.AddBitmap(file[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\22-1.bmp", RGB(255, 255, 255));
    f->LoadBitmap();
}

void EnemyBat::FollowHero(int step)
{
    const int n = 8;
    if (h->GetX1() + 20 > x + n) {
        x += step;
        isMovingRight = true;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetX1() + 20 < x - n) {
        x -= step;
        isMovingRight = false;
        isMovingLeft = true;
        isMovingUp = false;
        isMovingDown = false;
    }
    if (h->GetY2() - 30 > y + n) {
        y += step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = false;
        isMovingDown = true;
    }
    if (h->GetY2() - 30 < y - n) {
        y -= step;
        isMovingRight = false;
        isMovingLeft = false;
        isMovingUp = true;
        isMovingDown = false;
    }
}

void EnemyBat::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 3;
    int distanceX = abs(h->GetX1() - x);		//distanceX between enemy and hero
    int distanceY = abs(h->GetY1() - y);		//distanceY between enemy and hero
    if (distanceX < 350) {
        FollowHero(4);
    }
    else
    {
        distance += step;
        if (count % 2 == 0)
        {
            x += step;
            if (distance > 400)
            {
                distance = 0;
                count++;
            }
        }
        else
        {
            x -= step;
            if (distance > 400)
            {
                distance = 0;
                count++;
            }
        }
    }
    PlaySoundEffect();
    BeHit();
    bat.SetDelayCount(1);
    bat.OnMove();
}

void EnemyBat::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    bat.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    bat.OnShow();
}

void EnemyBat::ResetBlood()
{
    blood = default_blood;
}

void EnemyBat::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_hit07);
}

EnemyUFO::EnemyUFO(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    bullet = NULL;
    f = new ItemFruit(m, h);
    Initialize();
}

void EnemyUFO::Initialize()
{
    x = y = distance = count = stopTime = 0;
    blood = default_blood;
    isMovingRight = true;
    is_alive = isMovingDown = isMovingUp = isMovingLeft = isStop = false;
}

void EnemyUFO::LoadBitmap()
{
    const int a = 4;
    char* UFO[a] = { ".\\bitmaps\\enemy\\12-1.bmp",".\\bitmaps\\enemy\\12-2.bmp",".\\bitmaps\\enemy\\12-3.bmp",".\\bitmaps\\enemy\\12-4.bmp" };
    for (int i = 0; i < a; i++)
    {
        ufo.AddBitmap(UFO[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\12-1.bmp", RGB(255, 255, 255));
    f->LoadBitmap();
}

void EnemyUFO::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    ufo.OnMove();
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 8;
    int distanceX = abs(h->GetX1() - x);		//distanceX between enemy and hero
    count++;
    if (count > 70) {
        count = 0;
    }
    if (count == 1) {
        ShootBullet();
    }
    if(count == 20) {
        bullet->SetXY(x + 15, y + 15);
    }
    if (count > 20 && count<50) {
        bullet->OnMove();
    }
    if (distanceX <= 400)
    {
        if (count > 1 && count < 20)
        {
            if (h->GetX1() > x && h->GetY1() < y - 20) {
                x += step;
                y -= step;
                bullet->lu = bullet->ld = bullet->rd = false;
                bullet->ru = true;
            }
            if (h->GetX1() > x && h->GetY1() > y - 20) {
                x += step;
                y += step;
                bullet->lu = bullet->ru = bullet->ld = false;
                bullet->rd = true;
            }
            if (h->GetX1() < x && h->GetY1() < y - 20) {
                x -= step;
                y -= step;
                bullet->ru = bullet->ld = bullet->rd = false;
                bullet->lu = true;
            }
            if (h->GetX1() < x && h->GetY1() > y - 20) {
                x -= step;
                y += step;
                bullet->lu = bullet->ru = bullet->rd = false;
                bullet->ld = true;
            }
        }
    }
    PlaySoundEffect();
    BeHit();
    ufo.OnMove();
}

void EnemyUFO::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    if (count > 20 && count < 50) {
        bullet->OnShow();
    }
    ufo.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    ufo.OnShow();
}

void EnemyUFO::ShootBullet()
{
    delete bullet;
    bullet = new EnemyUFOBullet(m, h);
    bullet->LoadBitmap();
}

void EnemyUFO::ResetBlood()
{
    blood = default_blood;
}

void EnemyUFO::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_hit05);
}

EnemyUFO::~EnemyUFO()
{
    delete bullet;
    delete f;
}

EnemyUFOBullet::EnemyUFOBullet(Map* M, Hero* H)
{
    m = M;
    h = H;
    Initialize();
}

void EnemyUFOBullet::Initialize()
{
    x = y = 0;
    is_alive = rd = true;
    lu = ru = ld = false;
}

void EnemyUFOBullet::LoadBitmap()
{
    bmp.LoadBitmap(".\\bitmaps\\enemy\\12-5.bmp", RGB(255, 255, 255));
}

void EnemyUFOBullet::OnMove()
{
    if (!is_alive) {
        return;
    }
    int step = 20;
    if (ru) {
        x += step;
        y -= step;
    }
    else if (rd) {
        x += step;
        y += step;
    }
    else if (lu) {
        x -= step;
        y -= step;
    }
    else if (ld) {
        x -= step;
        y += step;
    }
    if (HitHero() || HitBlock())
    {
        if (HitHero())
        {
            h->ExecuteInjureEffect();
            is_alive = false;
        }
    }
}

void EnemyUFOBullet::OnShow()
{
    if (!is_alive) {
        return;
    }
    bmp.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    bmp.ShowBitmap();
}

void EnemyUFOBullet::SetXY(int X, int Y)
{
    x = X;
    y = Y;
}

EnemyTree::EnemyTree(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    f = new ItemFruit(m, h);
}

EnemyTree::~EnemyTree()
{
    delete f;
}

void EnemyTree::Initialize()
{
    x = y = distance = count = stopTime = 0;
    blood = default_blood;
    isMovingRight = true;
    is_alive = isMovingDown = isMovingUp = isMovingLeft = isStop = false;
}

void EnemyTree::LoadBitmap()
{
    const int a = 4;
    char* tree_L[a] = { ".\\bitmaps\\enemy\\13-1.bmp",".\\bitmaps\\enemy\\13-2.bmp",".\\bitmaps\\enemy\\13-3.bmp",".\\bitmaps\\enemy\\13-4.bmp" };
    char* tree_R[a] = { ".\\bitmaps\\enemy\\13-5.bmp",".\\bitmaps\\enemy\\13-6.bmp",".\\bitmaps\\enemy\\13-7.bmp",".\\bitmaps\\enemy\\13-8.bmp" };
    for (int i = 0; i < a; i++)
    {
        treeR.AddBitmap(tree_R[i], RGB(255, 255, 255));
        treeL.AddBitmap(tree_L[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\13-1.bmp", RGB(255, 255, 255));
    f->LoadBitmap();
}

void EnemyTree::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 5) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 3;
    const int step2 = 3;
    int distanceX = abs(h->GetX1() - x);		//distanceX between enemy and hero
    int distanceY = abs(h->GetY1() - y);		//distanceY between enemy and hero
    if (distanceX <= 500 && distanceY <= 400)
    {
        FollowHero(step2);
    }
    else
    {
        distance += step;
        if (count % 2 == 0)
        {
            isMovingRight = true;
            isMovingLeft = false;
            isMovingUp = false;
            isMovingDown = false;
            x += step;
            if (!m->isEmpty(GetX2(), y) || !m->isEmpty(GetX2(), GetY2()) || distance > 200)
            {
                distance = 0;
                count++;
            }
        }
        else
        {
            isMovingRight = false;
            isMovingLeft = true;
            isMovingUp = false;
            isMovingDown = false;
            x -= step;
            if (!m->isEmpty(x, y) || !m->isEmpty(x, GetY2()) || distance > 200)
            {
                distance = 0;
                count++;
            }
        }
    }
    PlaySoundEffect();
    BeHit();
    treeL.OnMove();
    treeR.OnMove();
}

void EnemyTree::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    treeR.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    treeL.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    if (isMovingRight || isMovingUp) {
        treeR.OnShow();
    }
    if (isMovingLeft || isMovingDown) {
        treeL.OnShow();
    }
}

void EnemyTree::ResetBlood()
{
    blood = default_blood;
}

void EnemyTree::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_hittree);
}

CastleRed::CastleRed(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
}

void CastleRed::Initialize()
{
    x = 2048;
    y = 64;
    distance = count = 0;
    blood = default_blood;
    isMovingRight = true;
    isMovingDown = isMovingUp = isMovingLeft = is_alive = false;
}

void CastleRed::LoadBitmap()
{
    bmp.LoadBitmap(".\\bitmaps\\item\\castleRed.bmp",RGB(255,255,255));
}

void CastleRed::OnMove()
{
    if(HitByBullet() && blood==2) {
        CAudio::Instance()->Play(AUDIO_castlered);
    }
    if (!is_alive) {
        return;
    }
    if(blood>1) {
        BeHit();
    }
}

void CastleRed::OnShow()
{
    if (!is_alive) {
        return;
    }
    bmp.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    if(blood<2) {
        bmp.ShowBitmap();
    }
}

bool CastleRed::HitByBullet()
{
    if (!is_alive)
        return false;
    if (b->IsAlive() == true)
    {
        return HitRectangle(b->GetX1(), b->GetY1(), b->GetX2(h), b->GetY2(h));
    }
    else
    {
        return false;
    }
}

void CastleRed::BeHit()
{
    if (HitByBullet())
    {
        blood -= 1;
        b->SetIsHit(true);
        if ((h->GetActivedBullet() || h->GetActivedMagic())) {
            b->SetIsAlive(false);
            b->SetIsHit(true);
        }
        CAudio::Instance()->Play(AUDIO_hit02);
        game->AddScore(50);
    }
    if (blood < 1) {
        is_alive = false;
    }
}

bool CastleRed::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
    int x1 = x+40;				// 瞴オàx畒夹
    int y1 = y+40;				// 瞴オày畒夹
    int x2 = x1+40;	// 瞴àx畒夹
    int y2 = y1+40;	// 瞴ày畒夹
    //
    // 浪代瞴痻籔把计痻琌Τユ栋
    //
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}


EnemyFirePig::EnemyFirePig(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    Initialize();
    //if (fire) { delete fire; }
    fire = new WeaponFire(m, h);
    f = new ItemFruit(m, h);
}

void EnemyFirePig::Initialize()
{
    x = y = distance = count = stopTime = load = 0;
    blood = default_blood;
    is_alive = isStop = false;
}

void EnemyFirePig::LoadBitmap()
{
    const int a = 4;
    char* fire_pig[a] = { ".\\bitmaps\\enemy\\19-1.bmp",".\\bitmaps\\enemy\\19-2.bmp",".\\bitmaps\\enemy\\19-3.bmp",".\\bitmaps\\enemy\\19-4.bmp" };
    for (int i = 0; i < a; i++)
    {
        firePig.AddBitmap(fire_pig[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\19-1.bmp", RGB(255, 255, 255));
    f->LoadBitmap();
    fire->LoadBitmap();
}

void EnemyFirePig::OnMove()
{
    f->OnMove();
    load++;
    if (!is_alive) {
        return;
    }
    if (distance < 140) {
        y -= 20;
        distance += 20;
    }
    if (distance >= 140 && distance < 150) {
        y -= 5;
        distance += 5;
    }
    if (distance >= 150 && distance < 160) {
        y += 5;
        distance += 5;
    }
    if (distance >= 160 && distance < 300) {
        y += 20;
        distance += 20;
    }
    if(distance==280) {
        count++;
        load = 10;
    }
    if (distance == 300) {
        distance = 0;
    }
    if (count %2 == 1 && load==10) {
        fire->SetXY(x, y + 20);
        fire->SetIsAlive(true);
    }
    if (fire->IsAlive()) {
        fire->MoveXY(-30,0);
    }
    PlaySoundEffect();
    BeHit();
    firePig.OnMove();
    fire->OnMove();
}

void EnemyFirePig::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    firePig.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    firePig.OnShow();
    if (fire->IsAlive()) {
        fire->OnShow();
    }
}

void EnemyFirePig::ResetBlood()
{
    blood = default_blood;
}

void EnemyFirePig::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_firepig);
}

EnemyFirePig::~EnemyFirePig()
{
    delete fire;
    delete f;
}

EnemyFireBall::EnemyFireBall(Map* M, Hero* H)
{
    m = M;
    h = H;
    Initialize();
}

void EnemyFireBall::Initialize()
{
    x = y = distance = count = stopTime = 0;
    type = "up";
    is_alive = true;
    isMovingDown = isMovingLeft = isMovingRight = isMovingUp = false;
}

void EnemyFireBall::LoadBitmap()
{
    const int a = 4;
    char* fire_D[a] = { ".\\bitmaps\\enemy\\20-1.bmp",".\\bitmaps\\enemy\\20-2.bmp",".\\bitmaps\\enemy\\20-3.bmp",".\\bitmaps\\enemy\\20-4.bmp" };
    char* fire_U[a] = { ".\\bitmaps\\enemy\\20-5.bmp",".\\bitmaps\\enemy\\20-6.bmp",".\\bitmaps\\enemy\\20-7.bmp",".\\bitmaps\\enemy\\20-8.bmp" };
    for (int i = 0; i < a; i++)
    {
        fireD.AddBitmap(fire_D[i], RGB(255, 255, 255));
        fireU.AddBitmap(fire_U[i], RGB(255, 255, 255));
    }
    bmp.LoadBitmap(".\\bitmaps\\enemy\\20-1.bmp", RGB(255, 255, 255));
}

void EnemyFireBall::OnMove()
{
    count++;
    if(type=="up")
    {
        if (count > 0 && count <= 20) {
            isMovingUp = true;
            isMovingDown = false;
        }
        if (count > 20 && count <= 40) {
            isMovingUp = false;
            isMovingDown = true;
        }
    }
    else
    {
        if (count > 0 && count <= 20) {
            isMovingUp = false;
            isMovingDown = true;
        }
        if (count > 20 && count <= 40) {
            isMovingUp = true;
            isMovingDown = false;
        }
    }
    if (count == 40) {
        count = 0;
    }
    if (isMovingUp) {
        y -= 30;
    }
    if (isMovingDown) {
        y += 30;
    }
    if (HitHero())		//璣动窱寄笆
    {
        h->ExecuteInjureEffect();
    }
    fireU.OnMove();
    fireD.OnMove();
}

void EnemyFireBall::OnShow()
{
    if (isMovingUp)
    {
        fireU.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        fireU.OnShow();
    }
    if (isMovingDown)
    {
        fireD.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        fireD.OnShow();
    }
}

void EnemyFireBall::SetType(string t)
{
    type = t;
}

MovingBlock::MovingBlock(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H,b=B;
    Initialize();
}

void MovingBlock::Initialize()
{
    x = y = distance = count = stopTime = 0;
    is_alive = true;
    isMovingDown = isMovingLeft = isMovingRight = isMovingUp = false;
}

void MovingBlock::LoadBitmap()
{
    bmp.LoadBitmap(".\\bitmaps\\item\\block.bmp", RGB(255, 255, 255));
}

void MovingBlock::OnMove()
{
    if (isMovingDown)
    {
        if (m->isEmpty(x + 32, GetY2() + 15) && m->isNotBlock(x + 32, GetY2() + 15))
        {
            m->ChangeMapValue(y / 64, x / 64, 0);
            y +=16;
        }
        else
        {
            CAudio::Instance()->Play(AUDIO_hit12);
            isMovingDown = false;
            m->ChangeMapValue(y/64,x/64,3);
        }
    }
    else if (isMovingUp)
    {
        if (m->isEmpty(x + 32, y - 16) && m->isNotBlock(x + 32, y - 16))
        {
            m->ChangeMapValue(y / 64, x / 64, 0);
            y -=16;
        }
        else
        {
            CAudio::Instance()->Play(AUDIO_hit12);
            isMovingUp = false;
            m->ChangeMapValue(y / 64, x / 64, 3);
        }
    }
    else if (isMovingLeft)
    {
        if (m->isEmpty(x - 16, y + 32) && m->isNotBlock(x - 16, y + 32))
        {
            m->ChangeMapValue(y / 64, x / 64, 0);
            x -=16;
        }
        else
        {
            CAudio::Instance()->Play(AUDIO_hit12);
            isMovingLeft = false;
            m->ChangeMapValue(y / 64, x / 64, 3);
        }
    }
    else if (isMovingRight)
    {
        if (m->isEmpty(GetX2() + 15, y + 32) && m->isNotBlock(GetX2() + 15, y + 32))
        {
            m->ChangeMapValue(y / 64, x / 64, 0);
            x +=16;
        }
        else
        {
            CAudio::Instance()->Play(AUDIO_hit12);
            isMovingRight = false;
            m->ChangeMapValue(y / 64, x / 64, 3);
        }
    }
    BeHit();
}

void MovingBlock::OnShow()
{
    bmp.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    bmp.ShowBitmap();
}

void MovingBlock::BeHit()
{
    if (HitByBullet())
    {
        b->SetIsAlive(false);
        b->SetIsHit(true);
        if (h->GetActivedMagic())
        {
            if (b->GetShootDown()) {
                isMovingDown = true;
                isMovingUp = false;
                isMovingLeft = false;
                isMovingRight = false;
            }
            else if (b->GetShootUp()) {
                isMovingDown = false;
                isMovingUp = true;
                isMovingLeft = false;
                isMovingRight = false;
            }
            else if (b->GetShootLeft()) {
                isMovingDown = false;
                isMovingUp = false;
                isMovingLeft = true;
                isMovingRight = false;
            }
            else if (b->GetShootRight()) {
                isMovingDown = false;
                isMovingUp = false;
                isMovingLeft = false;
                isMovingRight = true;
            }
        }
    }
}

int MovingBlock::GetX1()
{
    return x;
}

int MovingBlock::GetY1()
{
    return y;
}

int MovingBlock::GetX2()
{
    return x+64;
}

int MovingBlock::GetY2()
{
    return y+64;
}

EnemySkullSoul::EnemySkullSoul(Map* M, Hero* H, Bullet* B)
{
    m = M;
    h = H;
    b = B;
    f = new ItemFruit(m, h);
    w = new Weapon(m, h);
    Initialize();
}

EnemySkullSoul::~EnemySkullSoul()
{
    delete f;
    delete w;
}

void EnemySkullSoul::Initialize()
{
    x = y = count = stopTime = hiddenTime = 0;
    blood = default_blood;
    num = 1;
    is_alive = isStop = isHidden = false;
    w->type = "blueFire";
}

void EnemySkullSoul::LoadBitmap()
{
    const int a = 6;
    char* skull_l[a] = { ".\\bitmaps\\enemy\\24-1.bmp", ".\\bitmaps\\enemy\\24-2.bmp", ".\\bitmaps\\enemy\\24-3.bmp", ".\\bitmaps\\enemy\\24-4.bmp", ".\\bitmaps\\enemy\\24-5.bmp", ".\\bitmaps\\enemy\\24-6.bmp" };
    char* skull_r[a] = { ".\\bitmaps\\enemy\\24-7.bmp", ".\\bitmaps\\enemy\\24-8.bmp", ".\\bitmaps\\enemy\\24-9.bmp", ".\\bitmaps\\enemy\\24-10.bmp", ".\\bitmaps\\enemy\\24-11.bmp", ".\\bitmaps\\enemy\\24-12.bmp" };
    for (int i = 0; i < a; i++)
        skullR.AddBitmap(skull_r[i], RGB(255, 255, 255));
    for (int i = 0; i < a; i++)
        skullL.AddBitmap(skull_l[i], RGB(255, 255, 255));
    bmp.LoadBitmapA(".\\bitmaps\\enemy\\24-1.bmp");
    f->LoadBitmap();
    w->LoadBitmap();
}

void EnemySkullSoul::OnMove()
{
    f->OnMove();
    if (!is_alive) {
        return;
    }
    if (isHidden) {
        hiddenTime++;
    }
    if (hiddenTime > 40) {
        isHidden = false;
        hiddenTime = 0;
    }
    w->OnMove();
    if (isStop) {
        stopTime++;
    }
    if (stopTime > 8) {
        stopTime = 0;
        isStop = false;
    }
    if (isStop) {
        return;
    }
    const int step = 7;
    if (!(m->isEmpty(x - step, y) && m->isEmpty(x - step, y + bmp.Height())) ||
            !(m->isEmpty(x + step + bmp.Width(), y) && m->isEmpty(x + step + bmp.Width(), y + bmp.Height())) ||
            !(m->isEmpty(x, y - step) && m->isEmpty(x + bmp.Width(), y - step)) ||
            !(m->isEmpty(x, y + step + bmp.Height()) && m->isEmpty(x + bmp.Width(), y + step + bmp.Height())))   //窱毁锚杠
    {
        CAudio::Instance()->Play(AUDIO_soul);
        num = (rand() % 6) + 1;
        count = 0;
    }  //繦诀ネΘ1-4い计┕耞璶ǐよ
    if (num == 1) {
        w->AddCount();
        if (m->isEmpty(x - step, y) && m->isEmpty(x - step, y + bmp.Height()))
        {
            SetMovingLeft(true);
            SetMovingRight(false);
            SetMovingUp(false);
            SetMovingDown(false);
            x -= step;
            count++;
            if (w->GetReadyShoot()) {
                w->SetShootLeft(true);
                w->SetShootRight(false);
                w->SetShootUp(false);
                w->SetShootDown(false);
            }
        }
        if (count > (rand() % 100) + 20) {
            num = (rand() % 4) + 1;    //砞﹚繦诀˙计э跑1-4よ
            count = 0;
        }
    }
    else if (num == 2) {
        w->AddCount();
        if (m->isEmpty(x + step + bmp.Width(), y) && m->isEmpty(x + step + bmp.Width(), y + bmp.Height()))
        {
            SetMovingLeft(false);
            SetMovingRight(true);
            SetMovingUp(false);
            SetMovingDown(false);
            x += step;
            count++;
            if (w->GetReadyShoot()) {
                w->SetShootLeft(false);
                w->SetShootRight(true);
            }
        }
        if (count > (rand() % 100) + 20) {
            num = (rand() % 4) + 1;
            count = 0;
        }
    }
    else if (num == 3) {
        w->AddCount();
        if (m->isEmpty(x, y - step) && m->isEmpty(x + bmp.Width(), y - step))
        {
            SetMovingLeft(false);
            SetMovingRight(false);
            SetMovingUp(true);
            SetMovingDown(false);
            y -= step;
            count++;
            if (w->GetReadyShoot()) {
                w->SetShootLeft(true);
                w->SetShootRight(false);
            }
        }
        if (count > (rand() % 100) + 20) {
            num = (rand() % 4) + 1;
            count = 0;
        }
    }
    else if (num == 4) {
        w->AddCount();
        if (m->isEmpty(x, y + step + bmp.Height()) && m->isEmpty(x + bmp.Width(), y + step + bmp.Height()))
        {
            SetMovingLeft(false);
            SetMovingRight(false);
            SetMovingUp(false);
            SetMovingDown(true);
            y += step;
            count++;
            if (w->GetReadyShoot()) {
                w->SetShootLeft(false);
                w->SetShootRight(true);
            }
        }
        if (count > (rand() % 100) + 20) {
            num = (rand() % 4) + 1;
            count = 0;
        }
    }
    else if (num == 5) {
        w->AddCount();
        if (m->isEmpty(x, y + step + bmp.Height()) && m->isEmpty(x + bmp.Width(), y + step + bmp.Height()))
        {
            SetMovingLeft(true);
            SetMovingRight(false);
            SetMovingUp(false);
            SetMovingDown(false);
            x -= step;
            y += step;
            count++;
            if (w->GetReadyShoot()) {
                w->SetShootLeft(false);
                w->SetShootRight(true);
            }
        }
        if (count > (rand() % 100) + 20) {
            num = (rand() % 4) + 1;
            count = 0;
        }
    }
    else if (num == 6) {
        w->AddCount();
        if (m->isEmpty(x, y + step + bmp.Height()) && m->isEmpty(x + bmp.Width(), y + step + bmp.Height()))
        {
            SetMovingLeft(false);
            SetMovingRight(true);
            SetMovingUp(false);
            SetMovingDown(false);
            x += step;
            y += step;
            count++;
            if (w->GetReadyShoot()) {
                w->SetShootLeft(false);
                w->SetShootRight(true);
            }
        }
        if (count > (rand() % 100) + 20) {
            num = (rand() % 4) + 1;
            count = 0;
        }
    }
    skullR.OnMove();
    skullL.OnMove();
    if (HitHero()) {
        h->ExecuteInjureEffect();
    }
    if (!w->IsAlive() && w->GetCount() % 15 == 0) {
        w->SetIsAlive(true);
        w->SetXY(x + bmp.Width() / 2, y + bmp.Height() / 2);
        w->ResetCount();
    }
    if (w->IsAlive()) {
        w->SetReadyShoot(false);
    }
    PlaySoundEffect();
    if (HitByBullet()) {
        isHidden = true;
    }
    BeHit();
}

void EnemySkullSoul::OnShow()
{
    f->OnShow();
    if (!is_alive) {
        return;
    }
    w->OnShow();
    skullR.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    skullL.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    if (isMovingRight || isMovingDown) {
        if(!isHidden) {
            skullR.OnShow();
        }
    }
    else {
        if (!isHidden) {
            skullL.OnShow();
        }
    }
}

void EnemySkullSoul::ResetBlood()
{
    blood = default_blood;
}

void EnemySkullSoul::ReduceBlood(int b)
{
    blood -= b;
}

void EnemySkullSoul::PlaySoundEffect()
{
    if (HitByBullet())
        CAudio::Instance()->Play(AUDIO_hit11);
}

}

