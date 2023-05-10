#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Item.h"
#include "Hero.h"
#include "mygame.h"

namespace game_framework {
Item::Item() :x(0), y(0)	//默認建構子
{
    is_alive = false;
}
Item::Item(Map* M,Hero* H, Bullet* B)	//建構子
{
    m = M;
    h = H;
    b = B;
    Initialize();
}
void Item::Initialize()	//初始化
{
    x = y = counter = 0;
    hidden = false;
    blood = 2;
    type = "BloodUp";
    is_alive = true;
}
bool Item::IsAlive()	//判定是否存活
{
    return is_alive;
}
void Item::LoadBitmap()	//載入圖形
{
    item.LoadBitmap("Bitmaps/item/bld+1.bmp", RGB(255, 255, 255));
    lifeUp.LoadBitmap("Bitmaps/item/1up.bmp", RGB(255, 255, 255));
    cureBottle.LoadBitmap("Bitmaps/item/curebottle.bmp", RGB(255, 255, 255));
    bell.LoadBitmap("Bitmaps/item/bell.bmp", RGB(255, 255, 255));
    guard.LoadBitmap("Bitmaps/item/guard.bmp", RGB(255, 255, 255));
}
void Item::OnShow()	//顯示圖形
{
    if (!is_alive || hidden) {
        return;
    }
    if (type == "BloodUp")
    {
        item.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        item.ShowBitmap();
    }
    if (type == "LifeUp")
    {
        lifeUp.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        lifeUp.ShowBitmap();
    }
    if (type == "CureBottle")
    {
        cureBottle.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        cureBottle.ShowBitmap();
    }
    if (type == "Bell")
    {
        bell.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        bell.ShowBitmap();
    }
    if (type == "Guard")
    {
        guard.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        guard.ShowBitmap();
    }
}
void Item::OnMove()	//圖形的功能
{
    if (!is_alive) {
        return;
    }
    if (hidden)
    {
        if (blood > 0)
        {
            if (HitByBullet())
            {
                blood--;
                b->SetIsHit(true);
                CAudio::Instance()->Play(AUDIO_hit02);
                if ((h->GetActivedBullet() || h->GetActivedMagic())) {
                    b->SetIsAlive(false);
                    b->SetIsHit(true);
                }
            }
        }
        else {
            hidden = false;
        }
    }
    else
    {
        if (HitHero())
        {
            if (type == "BloodUp")
            {
                if (h->GetBloodAmount() < 6) {
                    h->AddBlood(1);
                }
            }
            if (type == "LifeUp")
            {
                if (h->GetLifeAmount() < 5) {
                    h->AddLife(1);
                }
            }
            if (type == "CureBottle")
            {
                h->AddBlood(6 - h->GetBloodAmount());
            }
            if (type == "Bell")
            {
                game->AddScore(1000);
            }
            if (type == "Guard")
            {
                h->SetInvincible(true);
                h->SetGuard(true);
            }
            SetIsAlive(false);
            CAudio::Instance()->Play(AUDIO_pick02);	//播放音效
        }
    }
}
void Item::SetIsAlive(bool alive)	//設定是否存活
{
    is_alive = alive;
}
void Item::SetType(string s)	//設定道具種類
{
    type = s;
}
void Item::SetHidden(bool flag)	//設定道具是否為隱藏
{
    hidden = flag;
}
bool Item::HitHero()	//是否碰撞到英雄
{
    if (is_alive && !h->Dead())
    {
        return HitRectangle(h->GetX1() + 10, h->GetY1() + 10,
                            h->GetX2() - 10, h->GetY2() - 10);
    }
    else {
        return false;
    }
}
bool Item::HitByBullet()	//是否被子彈擊中
{
    if (!is_alive || !b->IsAlive())
        return false;
    return HitRectangle(b->GetX1(), b->GetY1(), b->GetX2(h), b->GetY2(h));
}
void Item::ResetBlood()	//重置血量
{
    blood = 2;// 5;
}
void Item::SetXY(int xn, int yn)	//設定xy坐標
{
    x = xn;
    y = yn;
}
int Item::GetX1()	//取得左上角X坐標
{
    return x;
}
int Item::GetY1()	//取得左上角Y坐標
{
    return y;
}
int Item::GetX2()	//取得右下角X坐標
{
    int width = item.Width();
    if (type == "guard") {
        width = guard.Width();
    }
    return x + width;
}
int Item::GetY2()	//取得右下角Y坐標
{
    int height = item.Height();
    if (type == "guard") {
        height = guard.Height();
    }
    return y + height;
}

bool Item::HitRectangle(int tx1, int ty1, int tx2, int ty2)	//檢測圖形的碰撞
{
    int x1 = x+5;				// 球的左上角x座標
    int y1 = y+5;				// 球的左上角y座標
    int x2 = x1+10+item.Width();	// 球的右下角x座標
    int y2 = y1+10+item.Height();	// 球的右下角y座標
    //
    // 檢測球的矩形與參數矩形是否有交集
    //
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

ItemFruit::ItemFruit(Map* M, Hero* H)	//建構子
{
    m = M;
    h = H;
    Initialize();
    for (int i = 0; i < 15; i++)
    {
        fruit.push_back(item);
    }
}
void ItemFruit::Initialize()	//初始化
{
    x = 2800;
    y = 2300;
    type = 1;
    is_alive = false;
}
void ItemFruit::LoadBitmap()	//載入圖形
{
    const int a = 15;
    char* fruitname[a] = { "Bitmaps/item/fruit1.bmp","Bitmaps/item/fruit2.bmp","Bitmaps/item/fruit3.bmp","Bitmaps/item/fruit4.bmp","Bitmaps/item/fruit5.bmp",
                           "Bitmaps/item/fruit6.bmp", "Bitmaps/item/fruit7.bmp", "Bitmaps/item/fruit8.bmp", "Bitmaps/item/fruit9.bmp", "Bitmaps/item/fruit10.bmp",
                           "Bitmaps/item/fruit11.bmp", "Bitmaps/item/fruit12.bmp", "Bitmaps/item/fruit13.bmp", "Bitmaps/item/fruit14.bmp", "Bitmaps/item/fruit15.bmp"
                         };
    for (int i = 0; i < a; i++)
    {
        fruit[i].LoadBitmap(fruitname[i], RGB(255, 255, 255));
    }
    item.LoadBitmap("Bitmaps/item/fruit1.bmp", RGB(255, 255, 255));
}
void ItemFruit::OnShow()	//顯示圖形
{
    if (!is_alive) {
        return;
    }
    for (int i = 0; i < 15; i++)
    {
        if (GetFruitType() == i + 1)
        {
            fruit[i].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            fruit[i].ShowBitmap();
        }
    }
}
void ItemFruit::OnMove()	//圖形的功能
{
    if (!is_alive) {
        return;
    }
    if (HitHero())
    {
        game->AddScore(100);
        SetIsAlive(false);
        CAudio::Instance()->Play(AUDIO_pick01);
    }
}
void ItemFruit::SetFruitType(int n)	//設定水果種類
{
    type = n;
}
int ItemFruit::GetFruitType()	//取得水果種類
{
    return type;
}

ItemKey::ItemKey(Map* M, Hero* H)
{
    m = M;
    h = H;
    Initialize();
}
void ItemKey::Initialize()
{
    x = y = counter = 0;
    is_alive = isFollowHero = false;
}
void ItemKey::LoadBitmap()
{
    item.LoadBitmap("Bitmaps/item/key.bmp", RGB(255, 255, 255));
}
void ItemKey::OnShow()
{
    if (!is_alive) {
        return;
    }
    item.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    item.ShowBitmap();
}
void ItemKey::OnMove()
{
    if (!is_alive) {
        return;
    }
    if (HitHero() && !isFollowHero)
    {
        CAudio::Instance()->Play(AUDIO_key);
    }
    if (isFollowHero)
    {
        x = h->GetX1();
        y = h->GetY1()-40;
    }
}
void ItemKey::SetFollowHero(bool flag)
{
    isFollowHero = flag;
}
bool ItemKey::FollowedHero()
{
    return isFollowHero;
}
ItemLetter::ItemLetter(Map* M, Hero* H)
{
    m = M;
    h = H;
    Initialize();
}
void ItemLetter::Initialize()
{
    x = y = counter = 0;
    is_alive = true;
}

void ItemLetter::LoadBitmap()
{
    item.LoadBitmap("Bitmaps/item/letter.bmp", RGB(255, 255, 255));
}
void ItemLetter::OnShow()
{
    if (!is_alive) {
        return;
    }
    item.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    item.ShowBitmap();
}
void ItemLetter::OnMove()
{
    if (!is_alive) {
        return;
    }
    if (HitHero())
    {
        SetIsAlive(false);
        CAudio::Instance()->Play(AUDIO_letter);
    }
}

ItemKongQiPao::ItemKongQiPao(Map* M, Hero* H)
{
    m = M;
    h = H;
    Initialize();
}
void ItemKongQiPao::Initialize()
{
    x = y = counter = type = 0;
    is_alive = false;
}

void ItemKongQiPao::LoadBitmap()
{
    item.LoadBitmap("Bitmaps/item/bullet.bmp",RGB(255, 255, 255));
    weapon[0].LoadBitmap("Bitmaps/item/bullet.bmp", RGB(255, 255, 255));
    weapon[1].LoadBitmap("Bitmaps/item/slash.bmp", RGB(255, 255, 255));
    weapon[2].LoadBitmap("Bitmaps/item/magic.bmp", RGB(255, 255, 255));
}
void ItemKongQiPao::OnShow()
{
    if (!is_alive) {
        return;
    }
    if (type == 0)
    {
        weapon[0].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        weapon[0].ShowBitmap();
    }
    else if (type == 1)
    {
        weapon[1].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        weapon[1].ShowBitmap();
    }
    else
    {
        weapon[2].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        weapon[2].ShowBitmap();
    }
}
void ItemKongQiPao::OnMove()
{
    if (HitHero())
    {
        SetIsAlive(false);
        CAudio::Instance()->Play(AUDIO_letter);
    }
}
void ItemKongQiPao::SetType(int n)
{
    type = n;
}

ItemDoor::ItemDoor(Map* M, Hero* H)
{
    m = M;
    h = H;
    Initialize();
}
void ItemDoor::Initialize()
{
    x = y = counter = blood = 0;
    is_alive = true;
}
void ItemDoor::LoadBitmap()
{
    item.LoadBitmap("Bitmaps/item/starDoor.bmp", RGB(255, 255, 255));
}
void ItemDoor::OnShow()
{
    if (!is_alive) {
        return;
    }
    item.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
    item.ShowBitmap();
}
}