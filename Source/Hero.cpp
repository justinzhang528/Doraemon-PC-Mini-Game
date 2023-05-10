#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"


namespace game_framework {
Hero::Hero()
{
    Initialize();
}

int Hero::GetX1()
{
    return x;
}

int Hero::GetY1()
{
    if (isNormalMode) {
        return y + 30;
    }
    return y;
}

int Hero::GetX2()
{
    return x + animation_R.Width();
}

int Hero::GetY2()
{
    return y + animation_R.Height();
}

int Hero::GetFootX()
{
    return x+ animation_R.Width()/2;
}

int Hero::GetFootY()
{
    return y + animation_R.Height();
}

void Hero::Initialize()
{
    const int X_POS = 2960;//2960;
    const int Y_POS = 2300;//2300;
    const int Blood = 6;
    const int Life = 3;//2;
    jumpHeight = fallHeight = invincible_counter = guard_counter = injured_counter = transformation_counter = 0;
    reduceValue = 0;
    x = X_POS;
    y = Y_POS;
    blood = Blood;
    life = Life;
    step_size = 7;
    isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isJumping = isJumpBuffer = isFallBuffer = isTransAnimation = isPressedDown
                                   = isPressedRight = isPressedLeft = isShoot = isJumpMode = isGetBullet = isGetMagic = isGetBlade = isActivedBullet = isActivedBlade
                                           = isActivedMagic = isInvincible = isGuard = isInjured = false;
    isPressedUp = isReadyJump = isNormalMode = true;
    for (int i = 0; i < 6; i++) {
        v_blood.push_back(bmp);
        v_life.push_back(bmp);
        v_jump.push_back(bmp);
    }
    for (int i = 0; i < 4; i++) {
        v_hero_st.push_back(bmp);
        v_heroH_st.push_back(bmp);
        v_heroM_st.push_back(bmp);
    }
    for (int i = 0; i < 4; i++) {
        v_weapon.push_back(bmp);
        v_dead.push_back(bmp);
    }
    for (int i = 0; i < 9; i++) {
        v_transformation.push_back(bmp);
    }
}

void Hero::RefreshAnimation()
{
    step_size = 7;
    reduceValue = 0;
    jumpHeight = fallHeight = 0;
    isPressedUp = isPressedLeft = isPressedDown = isMovingDown = isMovingLeft = isMovingRight = isMovingUp = isJumping = isFallBuffer = isJumpBuffer = false;
    isPressedRight = isReadyJump = true;
}

void Hero::LoadBitmap()
{
    char* filename1[4] = { ".\\bitmaps\\hero\\r1.bmp",".\\bitmaps\\hero\\r2.bmp",".\\bitmaps\\hero\\r3.bmp",".\\bitmaps\\hero\\r4.bmp" };
    char* filename2[4] = { ".\\bitmaps\\hero\\l1.bmp",".\\bitmaps\\hero\\l2.bmp",".\\bitmaps\\hero\\l3.bmp",".\\bitmaps\\hero\\l4.bmp" };
    char* filename3[4] = { ".\\bitmaps\\hero\\f1.bmp",".\\bitmaps\\hero\\f2.bmp",".\\bitmaps\\hero\\f3.bmp",".\\bitmaps\\hero\\f4.bmp" };
    char* filename4[4] = { ".\\bitmaps\\hero\\b1.bmp",".\\bitmaps\\hero\\b2.bmp",".\\bitmaps\\hero\\b3.bmp",".\\bitmaps\\hero\\b4.bmp" };
    char* filename5[4] = { ".\\bitmaps\\hero\\b1h.bmp",".\\bitmaps\\hero\\b2h.bmp",".\\bitmaps\\hero\\b3h.bmp",".\\bitmaps\\hero\\b4h.bmp" };
    char* filename6[4] = { ".\\bitmaps\\hero\\f1h.bmp",".\\bitmaps\\hero\\f2h.bmp",".\\bitmaps\\hero\\f3h.bmp",".\\bitmaps\\hero\\f4h.bmp" };
    char* filename7[4] = { ".\\bitmaps\\hero\\l1h.bmp",".\\bitmaps\\hero\\l2h.bmp",".\\bitmaps\\hero\\l3h.bmp",".\\bitmaps\\hero\\l4h.bmp" };
    char* filename8[4] = { ".\\bitmaps\\hero\\r1h.bmp",".\\bitmaps\\hero\\r2h.bmp",".\\bitmaps\\hero\\r3h.bmp",".\\bitmaps\\hero\\r4h.bmp" };
    char* filename9[4] = { ".\\bitmaps\\hero\\b1m.bmp",".\\bitmaps\\hero\\b2m.bmp",".\\bitmaps\\hero\\b3m.bmp",".\\bitmaps\\hero\\b4m.bmp" };
    char* filename10[4] = { ".\\bitmaps\\hero\\f1m.bmp",".\\bitmaps\\hero\\f2m.bmp",".\\bitmaps\\hero\\f3m.bmp",".\\bitmaps\\hero\\f4m.bmp" };
    char* filename11[4] = { ".\\bitmaps\\hero\\l1m.bmp",".\\bitmaps\\hero\\l2m.bmp",".\\bitmaps\\hero\\l3m.bmp",".\\bitmaps\\hero\\l4m.bmp" };
    char* filename12[4] = { ".\\bitmaps\\hero\\r1m.bmp",".\\bitmaps\\hero\\r2m.bmp",".\\bitmaps\\hero\\r3m.bmp",".\\bitmaps\\hero\\r4m.bmp" };
    char* filename1i[4] = { ".\\bitmaps\\hero\\i\\r1.bmp",".\\bitmaps\\hero\\i\\r2.bmp",".\\bitmaps\\hero\\i\\r3.bmp",".\\bitmaps\\hero\\i\\r4.bmp" };
    char* filename2i[4] = { ".\\bitmaps\\hero\\i\\l1.bmp",".\\bitmaps\\hero\\i\\l2.bmp",".\\bitmaps\\hero\\i\\l3.bmp",".\\bitmaps\\hero\\i\\l4.bmp" };
    char* filename3i[4] = { ".\\bitmaps\\hero\\i\\f1.bmp",".\\bitmaps\\hero\\i\\f2.bmp",".\\bitmaps\\hero\\i\\f3.bmp",".\\bitmaps\\hero\\i\\f4.bmp" };
    char* filename4i[4] = { ".\\bitmaps\\hero\\i\\b1.bmp",".\\bitmaps\\hero\\i\\b2.bmp",".\\bitmaps\\hero\\i\\b3.bmp",".\\bitmaps\\hero\\i\\b4.bmp" };
    char* filename5i[4] = { ".\\bitmaps\\hero\\i\\b1h.bmp",".\\bitmaps\\hero\\i\\b2h.bmp",".\\bitmaps\\hero\\i\\b3h.bmp",".\\bitmaps\\hero\\i\\b4h.bmp" };
    char* filename6i[4] = { ".\\bitmaps\\hero\\i\\f1h.bmp",".\\bitmaps\\hero\\i\\f2h.bmp",".\\bitmaps\\hero\\i\\f3h.bmp",".\\bitmaps\\hero\\i\\f4h.bmp" };
    char* filename7i[4] = { ".\\bitmaps\\hero\\i\\l1h.bmp",".\\bitmaps\\hero\\i\\l2h.bmp",".\\bitmaps\\hero\\i\\l3h.bmp",".\\bitmaps\\hero\\i\\l4h.bmp" };
    char* filename8i[4] = { ".\\bitmaps\\hero\\i\\r1h.bmp",".\\bitmaps\\hero\\i\\r2h.bmp",".\\bitmaps\\hero\\i\\r3h.bmp",".\\bitmaps\\hero\\i\\r4h.bmp" };
    char* filename9i[4] = { ".\\bitmaps\\hero\\i\\b1m.bmp",".\\bitmaps\\hero\\i\\b2m.bmp",".\\bitmaps\\hero\\i\\b3m.bmp",".\\bitmaps\\hero\\i\\b4m.bmp" };
    char* filename10i[4] = { ".\\bitmaps\\hero\\i\\f1m.bmp",".\\bitmaps\\hero\\i\\f2m.bmp",".\\bitmaps\\hero\\i\\f3m.bmp",".\\bitmaps\\hero\\i\\f4m.bmp" };
    char* filename11i[4] = { ".\\bitmaps\\hero\\i\\l1m.bmp",".\\bitmaps\\hero\\i\\l2m.bmp",".\\bitmaps\\hero\\i\\l3m.bmp",".\\bitmaps\\hero\\i\\l4m.bmp" };
    char* filename12i[4] = { ".\\bitmaps\\hero\\i\\r1m.bmp",".\\bitmaps\\hero\\i\\r2m.bmp",".\\bitmaps\\hero\\i\\r3m.bmp",".\\bitmaps\\hero\\i\\r4m.bmp" };
    char* weapon[4] = { ".\\bitmaps\\hero\\weapon00.bmp",".\\bitmaps\\hero\\weapon01.bmp",".\\bitmaps\\hero\\weapon02.bmp",".\\bitmaps\\hero\\weapon03.bmp" };
    char* filename1s[2] = { ".\\bitmaps\\hero\\i\\r2.bmp",".\\bitmaps\\hero\\r2.bmp" };
    char* filename2s[2] = { ".\\bitmaps\\hero\\i\\l2.bmp",".\\bitmaps\\hero\\l2.bmp" };
    char* filename3s[2] = { ".\\bitmaps\\hero\\i\\f2.bmp",".\\bitmaps\\hero\\f2.bmp" };
    char* filename4s[2] = { ".\\bitmaps\\hero\\i\\b2.bmp",".\\bitmaps\\hero\\b2.bmp" };
    char* filename5s[2] = { ".\\bitmaps\\hero\\i\\b2h.bmp",".\\bitmaps\\hero\\b2h.bmp" };
    char* filename6s[2] = { ".\\bitmaps\\hero\\i\\f2h.bmp",".\\bitmaps\\hero\\f2h.bmp" };
    char* filename7s[2] = { ".\\bitmaps\\hero\\i\\l2h.bmp",".\\bitmaps\\hero\\l2h.bmp" };
    char* filename8s[2] = { ".\\bitmaps\\hero\\i\\r2h.bmp",".\\bitmaps\\hero\\r2h.bmp" };
    char* filename9s[2] = { ".\\bitmaps\\hero\\i\\b2m.bmp",".\\bitmaps\\hero\\b2m.bmp" };
    char* filename10s[2] = { ".\\bitmaps\\hero\\i\\f2m.bmp",".\\bitmaps\\hero\\f2m.bmp" };
    char* filename11s[2] = { ".\\bitmaps\\hero\\i\\l2m.bmp",".\\bitmaps\\hero\\l2m.bmp" };
    char* filename12s[2] = { ".\\bitmaps\\hero\\i\\r2m.bmp",".\\bitmaps\\hero\\r2m.bmp" };
    char* injured[2] = { ".\\bitmaps\\hero\\hurt1.bmp",".\\bitmaps\\hero\\hurt2.bmp" };
    char* injuredH[2] = { ".\\bitmaps\\hero\\hurt1h.bmp",".\\bitmaps\\hero\\hurt2h.bmp" };
    char* injuredM[2] = { ".\\bitmaps\\hero\\hurt1m.bmp",".\\bitmaps\\hero\\hurt2m.bmp" };
    char* dead[3] = { ".\\bitmaps\\hero\\dead.bmp",".\\bitmaps\\hero\\dead_h.bmp",".\\bitmaps\\hero\\dead_m.bmp" };
    char* jumpLi_jump[2] = { ".\\bitmaps\\hero\\i\\l3.bmp", ".\\bitmaps\\hero\\i\\l3j.bmp" };
    char* jumpRi_jump[2] = { ".\\bitmaps\\hero\\i\\r1.bmp", ".\\bitmaps\\hero\\i\\r1j.bmp" };
    char* jumpLih_jump[2] = { ".\\bitmaps\\hero\\i\\l3h.bmp", ".\\bitmaps\\hero\\i\\l3hj.bmp" };
    char* jumpRih_jump[2] = { ".\\bitmaps\\hero\\i\\r1h.bmp", ".\\bitmaps\\hero\\i\\r1hj.bmp" };
    char* jumpLim_jump[2] = { ".\\bitmaps\\hero\\i\\l3m.bmp", ".\\bitmaps\\hero\\i\\l3mj.bmp" };
    char* jumpRim_jump[2] = { ".\\bitmaps\\hero\\i\\r1m.bmp", ".\\bitmaps\\hero\\i\\r1mj.bmp" };
    char* transformation_pic[9] = { ".\\bitmaps\\c-1.bmp", ".\\bitmaps\\c-2.bmp",".\\bitmaps\\c-3.bmp",".\\bitmaps\\c-4.bmp",".\\bitmaps\\c-5.bmp",
                                    ".\\bitmaps\\c-6.bmp",".\\bitmaps\\c-7.bmp",".\\bitmaps\\c-8.bmp",".\\bitmaps\\c-9.bmp"
                                  };
    for (int i = 0; i < 4; i++)
    {
        animation_R.AddBitmap(filename1[i], RGB(255, 255, 255));
        animation_L.AddBitmap(filename2[i], RGB(255, 255, 255));
        animation_F.AddBitmap(filename3[i], RGB(255, 255, 255));
        animation_B.AddBitmap(filename4[i], RGB(255, 255, 255));
        animationH_B.AddBitmap(filename5[i], RGB(255, 255, 255));
        animationH_F.AddBitmap(filename6[i], RGB(255, 255, 255));
        animationH_L.AddBitmap(filename7[i], RGB(255, 255, 255));
        animationH_R.AddBitmap(filename8[i], RGB(255, 255, 255));
        animationM_B.AddBitmap(filename9[i], RGB(255, 255, 255));
        animationM_F.AddBitmap(filename10[i], RGB(255, 255, 255));
        animationM_L.AddBitmap(filename11[i], RGB(255, 255, 255));
        animationM_R.AddBitmap(filename12[i], RGB(255, 255, 255));
        animation_Ri.AddBitmap(filename1i[i], RGB(255, 255, 255));
        animation_Li.AddBitmap(filename2i[i], RGB(255, 255, 255));
        animation_Fi.AddBitmap(filename3i[i], RGB(255, 255, 255));
        animation_Bi.AddBitmap(filename4i[i], RGB(255, 255, 255));
        animationH_Bi.AddBitmap(filename5i[i], RGB(255, 255, 255));
        animationH_Fi.AddBitmap(filename6i[i], RGB(255, 255, 255));
        animationH_Li.AddBitmap(filename7i[i], RGB(255, 255, 255));
        animationH_Ri.AddBitmap(filename8i[i], RGB(255, 255, 255));
        animationM_Bi.AddBitmap(filename9i[i], RGB(255, 255, 255));
        animationM_Fi.AddBitmap(filename10i[i], RGB(255, 255, 255));
        animationM_Li.AddBitmap(filename11i[i], RGB(255, 255, 255));
        animationM_Ri.AddBitmap(filename12i[i], RGB(255, 255, 255));
    }
    for (int i = 0; i < 2; i++)
    {
        st_Ri.AddBitmap(filename1s[i], RGB(255, 255, 255));
        st_Li.AddBitmap(filename2s[i], RGB(255, 255, 255));
        st_Fi.AddBitmap(filename3s[i], RGB(255, 255, 255));
        st_Bi.AddBitmap(filename4s[i], RGB(255, 255, 255));
        stH_Bi.AddBitmap(filename5s[i], RGB(255, 255, 255));
        stH_Fi.AddBitmap(filename6s[i], RGB(255, 255, 255));
        stH_Li.AddBitmap(filename7s[i], RGB(255, 255, 255));
        stH_Ri.AddBitmap(filename8s[i], RGB(255, 255, 255));
        stM_Bi.AddBitmap(filename9s[i], RGB(255, 255, 255));
        stM_Fi.AddBitmap(filename10s[i], RGB(255, 255, 255));
        stM_Li.AddBitmap(filename11s[i], RGB(255, 255, 255));
        stM_Ri.AddBitmap(filename12s[i], RGB(255, 255, 255));
        injuredAnimation.AddBitmap(injured[i], RGB(255, 255, 255));
        injuredAnimationH.AddBitmap(injuredH[i], RGB(255, 255, 255));
        injuredAnimationM.AddBitmap(injuredM[i], RGB(255, 255, 255));
        animationJump_Li.AddBitmap(jumpLi_jump[i], RGB(255, 255, 255));
        animationJump_Ri.AddBitmap(jumpRi_jump[i], RGB(255, 255, 255));
        animationJumpH_Li.AddBitmap(jumpLih_jump[i], RGB(255, 255, 255));
        animationJumpH_Ri.AddBitmap(jumpRih_jump[i], RGB(255, 255, 255));
        animationJumpM_Li.AddBitmap(jumpLim_jump[i], RGB(255, 255, 255));
        animationJumpM_Ri.AddBitmap(jumpRim_jump[i], RGB(255, 255, 255));
    }
    v_hero_st[2].LoadBitmap(".\\bitmaps\\hero\\l2.bmp", RGB(255,255,255));
    v_hero_st[3].LoadBitmap(".\\bitmaps\\hero\\r2.bmp", RGB(255, 255, 255));
    v_hero_st[1].LoadBitmap(".\\bitmaps\\hero\\f2.bmp", RGB(255, 255, 255));
    v_hero_st[0].LoadBitmap(".\\bitmaps\\hero\\b2.bmp", RGB(255, 255, 255));
    v_heroH_st[2].LoadBitmap(".\\bitmaps\\hero\\l2h.bmp", RGB(255, 255, 255));
    v_heroH_st[3].LoadBitmap(".\\bitmaps\\hero\\r2h.bmp", RGB(255, 255, 255));
    v_heroH_st[1].LoadBitmap(".\\bitmaps\\hero\\f2h.bmp", RGB(255, 255, 255));
    v_heroH_st[0].LoadBitmap(".\\bitmaps\\hero\\b2h.bmp", RGB(255, 255, 255));
    v_heroM_st[2].LoadBitmap(".\\bitmaps\\hero\\l2m.bmp", RGB(255, 255, 255));
    v_heroM_st[3].LoadBitmap(".\\bitmaps\\hero\\r2m.bmp", RGB(255, 255, 255));
    v_heroM_st[1].LoadBitmap(".\\bitmaps\\hero\\f2m.bmp", RGB(255, 255, 255));
    v_heroM_st[0].LoadBitmap(".\\bitmaps\\hero\\b2m.bmp", RGB(255, 255, 255));
    v_jump[0].LoadBitmap(".\\bitmaps\\hero\\l3.bmp", RGB(255, 255, 255));
    v_jump[1].LoadBitmap(".\\bitmaps\\hero\\r1.bmp", RGB(255, 255, 255));
    v_jump[2].LoadBitmap(".\\bitmaps\\hero\\l3h.bmp", RGB(255, 255, 255));
    v_jump[3].LoadBitmap(".\\bitmaps\\hero\\r1h.bmp", RGB(255, 255, 255));
    v_jump[4].LoadBitmap(".\\bitmaps\\hero\\l3m.bmp", RGB(255, 255, 255));
    v_jump[5].LoadBitmap(".\\bitmaps\\hero\\r1m.bmp", RGB(255, 255, 255));
    char* bloodname[6] = { ".\\bitmaps\\hero\\blood1.bmp",".\\bitmaps\\hero\\blood2.bmp",".\\bitmaps\\hero\\blood3.bmp",".\\bitmaps\\hero\\blood4.bmp",".\\bitmaps\\hero\\blood5.bmp",".\\bitmaps\\hero\\blood6.bmp" };
    char* lifename[6]= { ".\\bitmaps\\hero\\life0.bmp",".\\bitmaps\\hero\\life1.bmp",".\\bitmaps\\hero\\life2.bmp",".\\bitmaps\\hero\\life3.bmp",".\\bitmaps\\hero\\life4.bmp",".\\bitmaps\\hero\\life5.bmp" };
    for (int i = 0; i < 6; i++)
    {
        v_blood[i].LoadBitmap(bloodname[i], RGB(255, 255, 255));
        v_life[i].LoadBitmap(lifename[i], RGB(255, 255, 255));
    }
    for (int i = 0; i < 4; i++)
    {
        v_weapon[i].LoadBitmap(weapon[i], RGB(255, 255, 255));
        v_dead[i].LoadBitmap(dead[i], RGB(255, 255, 255));
    }
    for (int i = 0; i < 9; i++)
    {
        v_transformation[i].LoadBitmap(transformation_pic[i], RGB(255, 255, 255));
    }
}

void Hero::OnMove(Map* m)
{
    step_size = 7;
    if (isTransAnimation) {
        transformation_counter++;
    }
    if (transformation_counter == 10) {
        isTransAnimation = false;
        transformation_counter = 0;
    }
    if (Dead() || isTransAnimation) {
        injuredAnimation.OnMove();
        injuredAnimationH.OnMove();
        injuredAnimationM.OnMove();
        return;
    }
    if (isFallBuffer || isJumpBuffer) {
        step_size = 10;
    }
    else {
        animation_R.OnMove();
        animation_L.OnMove();
        animation_F.OnMove();
        animation_B.OnMove();
        animationH_R.OnMove();
        animationH_L.OnMove();
        animationH_F.OnMove();
        animationH_B.OnMove();
        animationM_R.OnMove();
        animationM_L.OnMove();
        animationM_F.OnMove();
        animationM_B.OnMove();
        animation_Ri.OnMove();
        animation_Li.OnMove();
        animation_Fi.OnMove();
        animation_Bi.OnMove();
        animationH_Ri.OnMove();
        animationH_Li.OnMove();
        animationH_Fi.OnMove();
        animationH_Bi.OnMove();
        animationM_Ri.OnMove();
        animationM_Li.OnMove();
        animationM_Fi.OnMove();
        animationM_Bi.OnMove();
        animationJump_Li.OnMove();
        animationJump_Ri.OnMove();
        animationJumpH_Li.OnMove();
        animationJumpH_Ri.OnMove();
        animationJumpM_Li.OnMove();
        animationJumpM_Ri.OnMove();
        st_Ri.OnMove();
        st_Li.OnMove();
        st_Fi.OnMove();
        st_Bi.OnMove();
        stH_Ri.OnMove();
        stH_Li.OnMove();
        stH_Fi.OnMove();
        stH_Bi.OnMove();
        stM_Ri.OnMove();
        stM_Li.OnMove();
        stM_Fi.OnMove();
        stM_Bi.OnMove();
        injuredAnimationH.OnMove();
        injuredAnimationM.OnMove();
    }
    injuredAnimation.OnMove();
    if (isNormalMode)
    {
        for (int i = 0; i < step_size; i++)
        {
            if (isMovingLeft)
            {
                if (m->isEmpty(GetX1() - 1, GetY1()) && m->isEmpty(GetX1() - 1, GetY2())
                        && m->isNotBlock(GetX1() - 1, GetY1()) && m->isNotBlock(GetX1() - 1, GetY2()))
                    x --;
            }
            else if (isMovingRight)
            {
                if (m->isEmpty(GetX2() + 1, GetY1()) && m->isEmpty(GetX2() + 1, GetY2())
                        && m->isNotBlock(GetX2() + 1, GetY1()) && m->isNotBlock(GetX2() + 1, GetY2()))
                    x ++;
            }
            else if (isMovingUp)
            {
                if (m->isEmpty(GetX1(), GetY1() - 1) && m->isEmpty(GetX2(), GetY1() - 1)
                        && m->isNotBlock(GetX1(), GetY1() - 1) && m->isNotBlock(GetX2(), GetY1() - 1))
                    y --;
            }
            else if (isMovingDown)
            {
                if (m->isEmpty(GetX1(), GetY2() + 1) && m->isEmpty(GetX2(), GetY2() + 1)
                        && m->isNotBlock(GetX1(), GetY2() + 1) && m->isNotBlock(GetX2(), GetY2() + 1))
                    y ++;
            }
        }
    }
    else
    {
        int fallSpeed = 22;
        if (isFallBuffer) {
            fallSpeed = 4;
        }
        for (int i = 0; i < fallSpeed; i++)
        {
            if ((m->isEmpty(GetX1(), GetY2()+1) && m->isEmpty(GetX2(), GetY2()+1)) && !isJumping)
            {
                fallHeight++;
                y++;
            }
        }
        for (int i = 0; i < step_size; i++)
        {
            if (isMovingLeft)
            {
                if (m->isEmpty(GetX1() - 1, GetY1()) && m->isEmpty(GetX1() - 1, GetY2()))
                    x --;
            }
            if (isMovingRight)
            {
                if (m->isEmpty(GetX2() + 1, GetY1()) && m->isEmpty(GetX2() + 1, GetY2()))
                    x ++;
            }
        }
        if (isJumping)
        {
            if (m->isEmpty(GetX1(), GetY1() - step_size) && m->isEmpty(GetX2(), GetY1() - step_size))
            {
                y -= 20-(int)reduceValue;
                jumpHeight += 20-(int)reduceValue;
                reduceValue+=0.7;
            }
        }
        if (jumpHeight > 200 || !(m->isEmpty(GetX1(), GetY1() - step_size) && m->isEmpty(GetX2(), GetY1() - step_size))) {
            jumpHeight = 0;
            reduceValue = 0;
            isJumping = false;
            isJumpBuffer = false;
            isFallBuffer = true;
        }
        if (jumpHeight > 185 && jumpHeight<200) {
            isJumpBuffer = true;
        }
        if (fallHeight > 0) {
            isReadyJump = false;
        }
        if (fallHeight > 0 && fallHeight < 10) {
            isFallBuffer = true;
        }
        if (!(m->isEmpty(GetX1(), GetY2() + step_size) && m->isEmpty(GetX2(), GetY2() + step_size))) {
            isReadyJump = true;
            fallHeight = 0;
            isFallBuffer = false;
            isJumpBuffer = false;
        }
        if (fallHeight > 5) {
            fallHeight = 0;
            isFallBuffer = false;
        }
    }
    if (isInvincible) {
        invincible_counter++;    //無敵狀態下計時器往上數
    }
    if (isGuard) {
        guard_counter++;
    }
    if (isInjured) {
        injured_counter++;
    }
    if (invincible_counter > 60) {
        invincible_counter = 0;    //2秒後計時器歸零， 無敵失效
        SetInvincible(false);
    }
    if (guard_counter > 300) {
        isGuard = false;
        isInvincible = false;
        guard_counter=0;
    }
    if (injured_counter > 14) {
        injured_counter=0;
        isInjured = false;
    }
}

void Hero::SetMovingDown(bool flag)
{
    isMovingDown = flag;
}

void Hero::SetMovingLeft(bool flag)
{
    isMovingLeft = flag;
}

void Hero::SetMovingRight(bool flag)
{
    isMovingRight = flag;
}

void Hero::SetMovingUp(bool flag)
{
    isMovingUp = flag;
}

void Hero::SetPressedDown(bool flag)
{
    isPressedDown = flag;
}

void Hero::SetPressedLeft(bool flag)
{
    isPressedLeft = flag;
}

void Hero::SetPressedRight(bool flag)
{
    isPressedRight = flag;
}

void Hero::SetPressedUp(bool flag)
{
    isPressedUp = flag;
}

void Hero::SetJumping(bool flag)
{
    isJumping = flag;
}

void Hero::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

void Hero::SetPressedSpace(bool flag)
{
    isShoot = flag;
}

void Hero::ReduceBlood(int num)
{
    blood -= num;
}

void Hero::ReduceLife()
{
    life -= 1;
}

void Hero::AddBlood(int num)
{
    blood += num;
}

void Hero::AddLife(int n)
{
    life += n;
}

int Hero::GetBloodAmount()
{
    return blood;
}

int Hero::GetLifeAmount()
{
    return life;
}

void Hero::ResetBloodAmount()
{
    blood = 6;
}

void Hero::ResetLifeAmount()
{
    life = 2;
}

bool Hero::GetMovingDown()
{
    return isMovingDown;
}

bool Hero::GetMovingLeft()
{
    return isMovingLeft;
}

void Hero::OnShow(Map* m,Bullet* b)
{
    if (!Dead())   //沒有死亡的話
    {
        for (int i = 0; i < 6; i++)
        {
            v_blood[i].SetTopLeft(15, 15);
            v_life[i].SetTopLeft(10, 70);
        }
        if (!isInjured)
        {
            animation_L.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            animation_R.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            animation_F.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            animation_B.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            animationH_L.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationH_R.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationH_F.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationH_B.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationM_L.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationM_R.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationM_F.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationM_B.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animation_Li.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            animation_Ri.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            animation_Fi.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            animation_Bi.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            animationH_Li.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationH_Ri.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationH_Fi.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationH_Bi.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationM_Li.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationM_Ri.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationM_Fi.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationM_Bi.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationJump_Li.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            animationJump_Ri.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            animationJumpH_Li.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationJumpH_Ri.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationJumpM_Li.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            animationJumpM_Ri.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            st_Li.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            st_Ri.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            st_Fi.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            st_Bi.SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
            stH_Li.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            stH_Ri.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            stH_Fi.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            stH_Bi.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            stM_Li.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            stM_Ri.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            stM_Fi.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            stM_Bi.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            for (int i = 0; i < 6; i++)
            {
                v_jump[i].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            }
            for (int i = 0; i < 4; i++)
            {
                v_hero_st[i].SetTopLeft(m->ScreenX(x), m->ScreenY(y + 4));
                v_heroH_st[i].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
                v_heroM_st[i].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            }
            if (!isInvincible && !isGuard && !isInjured)
            {
                if (isPressedLeft && !isMovingLeft && !isMovingRight && !isMovingDown && !isMovingUp)
                {
                    if (!isReadyJump && isJumpMode)		//在跳躍或落下的時候播放的圖片
                    {
                        if (isActivedBlade) {
                            v_jump[2].ShowBitmap();
                        }
                        else if (isActivedMagic) {
                            v_jump[4].ShowBitmap();
                        }
                        else {
                            v_jump[0].ShowBitmap();
                        }
                    }
                    else
                    {
                        if (isActivedBlade) {
                            if (isShootAnimation && b->IsAlive()) {
                                animationH_L.OnShow();
                            }
                            else {
                                v_heroH_st[2].ShowBitmap();
                            }
                        }
                        else if (isActivedMagic) {
                            if (isShootAnimation && b->IsAlive()) {
                                animationM_L.OnShow();
                            }
                            else {
                                v_heroM_st[2].ShowBitmap();
                            }
                        }
                        else {
                            if (isShootAnimation && b->IsAlive()) {
                                animation_L.OnShow();
                            }
                            else {
                                v_hero_st[2].ShowBitmap();
                            }
                        }
                    }
                }
                else if (isPressedRight && !isMovingLeft && !isMovingRight && !isMovingDown && !isMovingUp)
                {
                    if (!isReadyJump && isJumpMode)		//在跳躍或落下的時候播放的圖片
                    {
                        if (isActivedBlade) {
                            v_jump[3].ShowBitmap();
                        }
                        else if (isActivedMagic) {
                            v_jump[5].ShowBitmap();
                        }
                        else {
                            v_jump[1].ShowBitmap();
                        }
                    }
                    else
                    {
                        if (isActivedBlade) {
                            if (isShootAnimation && b->IsAlive()) {
                                animationH_R.OnShow();
                            }
                            else {
                                v_heroH_st[3].ShowBitmap();
                            }
                        }
                        else if (isActivedMagic) {
                            if (isShootAnimation && b->IsAlive()) {
                                animationM_R.OnShow();
                            }
                            else {
                                v_heroM_st[3].ShowBitmap();
                            }
                        }
                        else {
                            if (isShootAnimation && b->IsAlive()) {
                                animation_R.OnShow();
                            }
                            else {
                                v_hero_st[3].ShowBitmap();
                            }
                        }
                    }
                }
                else if (isPressedUp && !isMovingLeft && !isMovingRight && !isMovingDown && !isMovingUp)
                {
                    if (isActivedBlade) {
                        if (isShootAnimation && b->IsAlive()) {
                            animationH_B.OnShow();
                        }
                        else {
                            v_heroH_st[0].ShowBitmap();
                        }
                    }
                    else if (isActivedMagic) {
                        if (isShootAnimation && b->IsAlive()) {
                            animationM_B.OnShow();
                        }
                        else {
                            v_heroM_st[0].ShowBitmap();
                        }
                    }
                    else {
                        if (isShootAnimation && b->IsAlive()) {
                            animation_B.OnShow();
                        }
                        else {
                            v_hero_st[0].ShowBitmap();
                        }
                    }
                }
                else if (isPressedDown && !isMovingLeft && !isMovingRight && !isMovingDown && !isMovingUp)
                {
                    if (isActivedBlade) {
                        if (isShootAnimation && b->IsAlive()) {
                            animationH_F.OnShow();
                        }
                        else {
                            v_heroH_st[1].ShowBitmap();
                        }
                    }
                    else if (isActivedMagic) {
                        if (isShootAnimation && b->IsAlive()) {
                            animationM_F.OnShow();
                        }
                        else {
                            v_heroM_st[1].ShowBitmap();
                        }
                    }
                    else {
                        if (isShootAnimation && b->IsAlive()) {
                            animation_F.OnShow();
                        }
                        else {
                            v_hero_st[1].ShowBitmap();
                        }
                    }
                }
                else if (isMovingLeft && !isMovingRight && !isMovingUp && !isMovingDown)
                {
                    if (!isReadyJump && isJumpMode)		//在跳躍或落下的時候播放的圖片
                    {
                        if (isActivedBlade) {
                            v_jump[2].ShowBitmap();
                        }
                        else if (isActivedMagic) {
                            v_jump[4].ShowBitmap();
                        }
                        else {
                            v_jump[0].ShowBitmap();
                        }
                    }
                    else
                    {
                        if (isActivedBlade) {
                            animationH_L.OnShow();
                        }
                        else if (isActivedMagic) {
                            animationM_L.OnShow();
                        }
                        else {
                            animation_L.OnShow();
                        }
                    }
                }
                else if (isMovingRight && !isMovingLeft && !isMovingUp && !isMovingDown)
                {
                    if (!isReadyJump && isJumpMode)		//在跳躍或落下的時候播放的圖片
                    {
                        if (isActivedBlade) {
                            v_jump[3].ShowBitmap();
                        }
                        else if (isActivedMagic) {
                            v_jump[5].ShowBitmap();
                        }
                        else {
                            v_jump[1].ShowBitmap();
                        }
                    }
                    else
                    {
                        if (isActivedBlade) {
                            animationH_R.OnShow();
                        }
                        else if (isActivedMagic) {
                            animationM_R.OnShow();
                        }
                        else {
                            animation_R.OnShow();
                        }
                    }
                }
                else if (isMovingDown && !isMovingUp && !isMovingLeft && !isMovingRight)
                {
                    if (isActivedBlade) {
                        animationH_F.OnShow();
                    }
                    else if (isActivedMagic) {
                        animationM_F.OnShow();
                    }
                    else {
                        animation_F.OnShow();
                    }
                }
                else if (isMovingUp && !isMovingDown && !isMovingLeft && !isMovingRight)
                {
                    if (isActivedBlade) {
                        animationH_B.OnShow();
                    }
                    else if (isActivedMagic) {
                        animationM_B.OnShow();
                    }
                    else {
                        animation_B.OnShow();
                    }
                }
            }
            else
            {
                if (isPressedLeft && !isMovingLeft && !isMovingRight && !isMovingDown && !isMovingUp)
                {
                    if (!isReadyJump && isJumpMode)		//在跳躍或落下的時候播放的圖片
                    {
                        if (isActivedBlade) {
                            animationJumpH_Li.OnShow();
                        }
                        else if (isActivedMagic) {
                            animationJumpM_Li.OnShow();
                        }
                        else {
                            animationJump_Li.OnShow();
                        }
                    }
                    else
                    {
                        if (isActivedBlade) {
                            if (isShootAnimation && b->IsAlive()) {
                                animationH_Li.OnShow();
                            }
                            else {
                                stH_Li.OnShow();
                            }
                        }
                        else if (isActivedMagic) {
                            if (isShootAnimation && b->IsAlive()) {
                                animationM_Li.OnShow();
                            }
                            else {
                                stM_Li.OnShow();
                            }
                        }
                        else {
                            if (isShootAnimation && b->IsAlive()) {
                                animation_Li.OnShow();
                            }
                            else {
                                st_Li.OnShow();
                            }
                        }
                    }
                }
                else if (isPressedRight && !isMovingLeft && !isMovingRight && !isMovingDown && !isMovingUp)
                {
                    if (!isReadyJump && isJumpMode)		//在跳躍或落下的時候播放的圖片
                    {
                        if (isActivedBlade) {
                            animationJumpH_Ri.OnShow();
                        }
                        else if (isActivedMagic) {
                            animationJumpM_Ri.OnShow();
                        }
                        else {
                            animationJump_Ri.OnShow();
                        }
                    }
                    else
                    {
                        if (isActivedBlade) {
                            if (isShootAnimation && b->IsAlive()) {
                                animationH_Ri.OnShow();
                            }
                            else {
                                stH_Ri.OnShow();
                            }
                        }
                        else if (isActivedMagic) {
                            if (isShootAnimation && b->IsAlive()) {
                                animationM_Ri.OnShow();
                            }
                            else {
                                stM_Ri.OnShow();
                            }
                        }
                        else {
                            if (isShootAnimation && b->IsAlive()) {
                                animation_Ri.OnShow();
                            }
                            else {
                                st_Ri.OnShow();
                            }
                        }
                    }
                }
                else if (isPressedUp && !isMovingLeft && !isMovingRight && !isMovingDown && !isMovingUp)
                {
                    if (isActivedBlade) {
                        if (isShootAnimation && b->IsAlive()) {
                            animationH_Bi.OnShow();
                        }
                        else {
                            stH_Bi.OnShow();
                        }
                    }
                    else if (isActivedMagic) {
                        if (isShootAnimation && b->IsAlive()) {
                            animationM_Bi.OnShow();
                        }
                        else {
                            stM_Bi.OnShow();
                        }
                    }
                    else {
                        if (isShootAnimation && b->IsAlive()) {
                            animation_Bi.OnShow();
                        }
                        else {
                            st_Bi.OnShow();
                        }
                    }
                }
                else if (isPressedDown && !isMovingLeft && !isMovingRight && !isMovingDown && !isMovingUp)
                {
                    if (isActivedBlade) {
                        if (isShootAnimation && b->IsAlive()) {
                            animationH_Fi.OnShow();
                        }
                        else {
                            stH_Fi.OnShow();
                        }
                    }
                    else if (isActivedMagic) {
                        if (isShootAnimation && b->IsAlive()) {
                            animationM_Fi.OnShow();
                        }
                        else {
                            stM_Fi.OnShow();
                        }
                    }
                    else {
                        if (isShootAnimation && b->IsAlive()) {
                            animation_Fi.OnShow();
                        }
                        else {
                            st_Fi.OnShow();
                        }
                    }
                }
                else if (isMovingLeft && !isMovingRight && !isMovingUp && !isMovingDown)
                {
                    if (!isReadyJump && isJumpMode)		//在跳躍或落下的時候播放的圖片
                    {
                        if (isActivedBlade) {
                            animationJumpH_Li.OnShow();
                        }
                        else if (isActivedMagic) {
                            animationJumpM_Li.OnShow();
                        }
                        else {
                            animationJump_Li.OnShow();
                        }
                    }
                    else
                    {
                        if (isActivedBlade) {
                            animationH_Li.OnShow();
                        }
                        else if (isActivedMagic) {
                            animationM_Li.OnShow();
                        }
                        else {
                            animation_Li.OnShow();
                        }
                    }
                }
                else if (isMovingRight && !isMovingLeft && !isMovingUp && !isMovingDown)
                {
                    if (!isReadyJump && isJumpMode)		//在跳躍或落下的時候播放的圖片
                    {
                        if (isActivedBlade) {
                            animationJumpH_Ri.OnShow();
                        }
                        else if (isActivedMagic) {
                            animationJumpM_Ri.OnShow();
                        }
                        else {
                            animationJump_Ri.OnShow();
                        }
                    }
                    else
                    {
                        if (isActivedBlade) {
                            animationH_Ri.OnShow();
                        }
                        else if (isActivedMagic) {
                            animationM_Ri.OnShow();
                        }
                        else {
                            animation_Ri.OnShow();
                        }
                    }
                }
                else if (isMovingDown && !isMovingUp && !isMovingLeft && !isMovingRight)
                {
                    if (isActivedBlade) {
                        animationH_Fi.OnShow();
                    }
                    else if (isActivedMagic) {
                        animationM_Fi.OnShow();
                    }
                    else {
                        animation_Fi.OnShow();
                    }
                }
                else if (isMovingUp && !isMovingDown && !isMovingLeft && !isMovingRight)
                {
                    if (isActivedBlade) {
                        animationH_Bi.OnShow();
                    }
                    else if (isActivedMagic) {
                        animationM_Bi.OnShow();
                    }
                    else {
                        animation_Bi.OnShow();
                    }
                }
            }
        }
        if (isInjured)
        {
            if (isActivedBlade) {
                injuredAnimationH.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
                injuredAnimationH.OnShow();
            }
            else if (isActivedMagic) {
                injuredAnimationM.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
                injuredAnimationM.OnShow();
            }
            else {
                injuredAnimation.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
                injuredAnimation.OnShow();
            }
        }
        if (isActivedBullet && !isGetMagic) {
            v_weapon[0].SetTopLeft(2, 105);
            v_weapon[0].ShowBitmap();
        }
        else if (isActivedBullet && isGetMagic) {
            v_weapon[1].SetTopLeft(2, 105);
            v_weapon[1].ShowBitmap();
        }
        else if (isActivedMagic) {
            v_weapon[2].SetTopLeft(2, 105);
            v_weapon[2].ShowBitmap();
        }
        else if (isActivedBlade) {
            v_weapon[3].SetTopLeft(2, 105);
            v_weapon[3].ShowBitmap();
        }
        switch (life)
        {
        case 0:
            v_life[0].ShowBitmap();
            break;
        case 1:
            v_life[1].ShowBitmap();
            break;
        case 2:
            v_life[2].ShowBitmap();
            break;
        case 3:
            v_life[3].ShowBitmap();
            break;
        case 4:
            v_life[4].ShowBitmap();
            break;
        case 5:
            v_life[5].ShowBitmap();
            break;
        default:
            v_life[5].ShowBitmap();
            break;
        }
        switch (blood)
        {
        case 1:
            v_blood[0].ShowBitmap();
            break;
        case 2:
            v_blood[1].ShowBitmap();
            break;
        case 3:
            v_blood[2].ShowBitmap();
            break;
        case 4:
            v_blood[3].ShowBitmap();
            break;
        case 5:
            v_blood[4].ShowBitmap();
            break;
        default:
            v_blood[5].ShowBitmap();
            break;
        }
        for (int i = 1; i < 9; i++)		//顯示變身動畫
        {
            if (transformation_counter == i)
            {
                v_transformation[i].SetTopLeft(m->ScreenX(x-15), m->ScreenY(y-10));
                v_transformation[i].ShowBitmap();
            }
        }
    }
    else    //死亡的話
    {
        if(isJumpMode)
        {
            injuredAnimation.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            injuredAnimationH.SetTopLeft(m->ScreenX(x), m->ScreenY(y-10));
            injuredAnimationM.SetTopLeft(m->ScreenX(x), m->ScreenY(y-10));
            if (isActivedBlade) {
                injuredAnimationH.OnShow();
            }
            else if (isActivedMagic) {
                injuredAnimationM.OnShow();
            }
            else {
                injuredAnimation.OnShow();
            }
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                v_dead[i].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
            }
            if (isActivedBlade) {
                v_dead[1].ShowBitmap();
            }
            else if (isActivedMagic) {
                v_dead[2].ShowBitmap();
            }
            else {
                v_dead[0].ShowBitmap();
            }
        }
    }
}

bool Hero::GetMovingRight()
{
    return isMovingRight;
}
bool Hero::GetMovingUp()
{
    return isMovingUp;
}
bool Hero::GetJumping()
{
    return isJumping;
}
bool Hero::GetPressedDown()
{
    return isPressedDown;
}
bool Hero::GetPressedLeft()
{
    return isPressedLeft;
}
bool Hero::GetPressedRight()
{
    return isPressedRight;
}
bool Hero::GetPressedUp()
{
    return isPressedUp;
}
bool Hero::GetPressedSpace()
{
    return isShoot;
}

void Hero::GetedBullet(bool flag)
{
    isGetBullet = flag;
}

void Hero::GetedMagic(bool flag)
{
    isGetMagic = flag;
}

void Hero::GetedBlade(bool flag)
{
    isGetBlade = flag;
}

void Hero::ActivedBullet(bool flag)
{
    isActivedBullet = flag;
}

void Hero::ChangeWeapon()
{
    isTransAnimation = true;
    if (isActivedBullet) {
        ActivedBlade(true);
        ActivedMagic(false);
        ActivedBullet(false);
    }
    else if (isActivedBlade) {
        ActivedBlade(false);
        ActivedMagic(true);
        ActivedBullet(false);
    }
    else if (isActivedMagic) {
        ActivedBlade(false);
        ActivedMagic(false);
        ActivedBullet(true);
    }
}

void Hero::ActivedMagic(bool flag)
{
    isActivedMagic = flag;
}

void Hero::ActivedBlade(bool flag)
{
    isActivedBlade = flag;
}

bool Hero::GetBullet()
{
    return isGetBullet;
}

bool Hero::GetMagic()
{
    return isGetMagic;
}

bool Hero::GetBlade()
{
    return isGetBlade;
}

bool Hero::GetActivedBullet()
{
    return isActivedBullet;
}

bool Hero::Over()
{
    return (life<0);
}

bool Hero::Dead()
{
    return (blood<1);
}

void Hero::SetInvincible(bool flag)
{
    isInvincible = flag;
}

void Hero::SetGuard(bool flag)
{
    isGuard = flag;
}

bool Hero::GetInvincible()
{
    return isInvincible;
}

bool Hero::GetGuard()
{
    return isGuard;
}

void Hero::SetInjured(bool flag)
{
    isInjured = flag;
}

void Hero::ExecuteInjureEffect()
{
    game->AddScore(-20);
    CAudio::Instance()->Play(AUDIO_injure);
    SetInjured(true);
    SetInvincible(true);
    ReduceBlood(1);
}

int Hero::GetJumpHeight()
{
    return jumpHeight;
}

void Hero::ChangeToJumpMode()
{
    isJumpMode = true;
    isNormalMode = false;
}

bool Hero::IsTransAnimation()
{
    return isTransAnimation;
}

void Hero::ChangeToNormalMode()
{
    isNormalMode = true;
    isJumpMode = false;
}

bool Hero::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
    int x1 = x;
    int y1 = y;
    int x2 = x1 + v_hero_st[0].Width();
    int y2 = y1 + v_hero_st[0].Height();
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

bool Hero::GetActivedMagic()
{
    return isActivedMagic;
}

bool Hero::GetActivedBlade()
{
    return isActivedBlade;
}

void Hero::SetShootAnimation(bool flag)
{
    isShootAnimation = flag;
}

int Hero::GetStepSize()
{
    return step_size;
}

Bullet::Bullet(Map* M) :x(0), y(0)
{
    m = M;
    is_alive = isHit = isBlood = isShootDown = isShootLeft = isShootRight = false;
    isShootUp = true;
    for (int i = 0; i < 4; i++)
    {
        magic.push_back(bullet);
        blood.push_back(bullet);
        hit.push_back(bullet);
        kongqipao.push_back(bullet);
    }
    picDir = 1;
    showHitCounter = showBloodCounter = 0;
    x = y = bx = by = hx = hy = 0;
}
bool Bullet::IsAlive()
{
    return is_alive;
}
void Bullet::LoadBitmap()
{
    char* hit_pic[5] = { ".\\bitmaps\\hit (1).bmp", ".\\bitmaps\\hit (2).bmp", ".\\bitmaps\\hit (3).bmp", ".\\bitmaps\\hit (4).bmp", ".\\bitmaps\\hit (5).bmp" };
    char* Blade_pic[4] = { ".\\bitmaps\\hero\\Blade (1).bmp",".\\bitmaps\\hero\\Blade (2).bmp",".\\bitmaps\\hero\\Blade (3).bmp",".\\bitmaps\\hero\\Blade (4).bmp"};
    bullet.LoadBitmap(".\\bitmaps\\hero\\bullet.bmp", RGB(255, 255, 255));
    magic[0].LoadBitmap(".\\bitmaps\\hero\\magic_b.bmp", RGB(255, 255, 255));
    magic[1].LoadBitmap(".\\bitmaps\\hero\\magic_f.bmp", RGB(255, 255, 255));
    magic[2].LoadBitmap(".\\bitmaps\\hero\\magic_l.bmp", RGB(255, 255, 255));
    magic[3].LoadBitmap(".\\bitmaps\\hero\\magic_r.bmp", RGB(255, 255, 255));
    blood[0].LoadBitmap(".\\bitmaps\\blood1.bmp", RGB(255, 255, 255));
    blood[1].LoadBitmap(".\\bitmaps\\blood2.bmp", RGB(255, 255, 255));
    blood[2].LoadBitmap(".\\bitmaps\\blood3.bmp", RGB(255, 255, 255));
    blood[3].LoadBitmap(".\\bitmaps\\blood4.bmp", RGB(255, 255, 255));
    hit[0].LoadBitmap(".\\bitmaps\\hit (1).bmp", RGB(0, 0, 0));
    hit[1].LoadBitmap(".\\bitmaps\\hit (2).bmp", RGB(0, 0, 0));
    hit[2].LoadBitmap(".\\bitmaps\\hit (3).bmp", RGB(0, 0, 0));
    hit[3].LoadBitmap(".\\bitmaps\\hit (4).bmp", RGB(0, 0, 0));
    kongqipao[0].LoadBitmap(".\\bitmaps\\hero\\kongqipaoU.bmp", RGB(255, 255, 255));
    kongqipao[1].LoadBitmap(".\\bitmaps\\hero\\kongqipaoD.bmp", RGB(255, 255, 255));
    kongqipao[2].LoadBitmap(".\\bitmaps\\hero\\kongqipaoL.bmp", RGB(255, 255, 255));
    kongqipao[3].LoadBitmap(".\\bitmaps\\hero\\kongqipaoR.bmp", RGB(255, 255, 255));
    for (int i = 0; i < 4; i++)
        Blade.AddBitmap(Blade_pic[i], RGB(255, 255, 255));
}

void Bullet::OnMove(Hero* h,Map* m)
{
    if (h->Dead()) {
        return;
    }
    Blade.SetDelayCount(3);
    Blade.OnMove();
    int step = 22;
    if (h->GetActivedMagic()) {
        step = 13;
    }
    if (h->GetActivedBlade()) {
        step = 20;
    }
    if (is_alive && (h->GetActivedBullet()|| h->GetActivedMagic() || h->GetActivedBlade()))
    {
        if (isShootUp) {
            y -= step;
        }
        else if (isShootDown) {
            y += step;
        }
        else if (isShootRight) {
            x += step;
        }
        else if (isShootLeft) {
            x -= step;
        }
    }
    if(is_alive)
    {
        if (OverBoundary(m) || HitBlock(m, h))	//閉堤?賜麼岆湖善梤焰昜
        {
            SetIsAlive(false);
            if (HitBlock(m, h)) {
                SetIsHit(true);
            }
        }
    }
    if (isHit) {
        showHitCounter++;
    }
    if (isBlood) {
        showBloodCounter++;
    }
}

void Bullet::OnShow(Map* m,Hero* h)
{
    if (h->Dead()) {
        return;
    }
    if (isHit && !isBlood) {
        if (showHitCounter == 1) {
            hx = x;
            hy = y;
        }
        for(int i=0; i<4; i++)
            hit[i].SetTopLeft(m->ScreenX(hx), m->ScreenY(hy));
        if(showHitCounter >1 && showHitCounter <=3) {
            hit[0].ShowBitmap();
        }
        if (showHitCounter > 3 && showHitCounter <=5) {
            hit[1].ShowBitmap();
        }
        if (showHitCounter > 5 && showHitCounter <=7) {
            hit[2].ShowBitmap();
        }
        if (showHitCounter > 7 && showHitCounter <=9) {
            hit[3].ShowBitmap();
        }
    }
    if (showHitCounter > 10) {
        SetIsHit(false);
        showHitCounter = 0;
    }
    if (isBlood) {
        if (showBloodCounter > 1 && showBloodCounter <=6)
        {
            blood[0].SetTopLeft(m->ScreenX(bx), m->ScreenY(by));
            blood[0].ShowBitmap();
        }
        if (showBloodCounter > 6 && showBloodCounter <= 11)
        {
            blood[1].SetTopLeft(m->ScreenX(bx), m->ScreenY(by));
            blood[1].ShowBitmap();
        }
        if (showBloodCounter > 11 && showBloodCounter <= 16)
        {
            blood[2].SetTopLeft(m->ScreenX(bx), m->ScreenY(by));
            blood[2].ShowBitmap();
        }
        if (showBloodCounter > 16 && showBloodCounter <= 20)
        {
            blood[3].SetTopLeft(m->ScreenX(bx), m->ScreenY(by));
            blood[3].ShowBitmap();
        }
    }
    if (showBloodCounter >= 20) {
        SetIsBlood(false);
        showBloodCounter = 0;
    }
    if (is_alive && h->GetBullet() && h->GetActivedBullet()) {
        for (int i = 0; i < 4; i++)
            kongqipao[i].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        if (picDir == 1) {
            kongqipao[0].ShowBitmap();
        }
        else if (picDir == 2) {
            kongqipao[1].ShowBitmap();
        }
        else if (picDir == 3) {
            kongqipao[2].ShowBitmap();
        }
        else if (picDir == 4) {
            kongqipao[3].ShowBitmap();
        }
    }
    else if(is_alive && h->GetMagic() && h->GetActivedMagic()) {
        for (int i = 0; i < 4; i++)
            magic[i].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        if (picDir==1) {
            magic[0].ShowBitmap();
        }
        else if (picDir == 2) {
            magic[1].ShowBitmap();
        }
        else if (picDir == 3) {
            magic[2].ShowBitmap();
        }
        else if (picDir == 4) {
            magic[3].ShowBitmap();
        }
    }
    else if (is_alive && h->GetBlade() && h->GetActivedBlade()) {
        Blade.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
        Blade.OnShow();
    }
}

void Bullet::OnKeyDown(Hero* h)
{
    if (h->Dead()) {
        return;
    }
    if (h->GetMovingUp()) {
        isShootUp = true;
        isShootDown = false;
        isShootLeft = false;
        isShootRight = false;
        picDir = 1;
    }
    else if (h->GetMovingDown()) {
        isShootUp = false;
        isShootDown = true;
        isShootLeft = false;
        isShootRight = false;
        picDir = 2;
    }
    else if (h->GetMovingRight()) {
        isShootUp = false;
        isShootDown = false;
        isShootLeft = false;
        isShootRight = true;
        picDir = 4;
    }
    else if (h->GetMovingLeft()) {
        isShootUp = false;
        isShootDown = false;
        isShootLeft = true;
        isShootRight = false;
        picDir = 3;
    }
    else if (h->GetPressedUp()) {
        isShootUp = true;
        isShootDown = false;
        isShootLeft = false;
        isShootRight = false;
        picDir = 1;
    }
    else if (h->GetPressedDown()) {
        isShootUp = false;
        isShootDown = true;
        isShootLeft = false;
        isShootRight = false;
        picDir = 2;
    }
    else if (h->GetPressedRight()) {
        isShootUp = false;
        isShootDown = false;
        isShootLeft = false;
        isShootRight = true;
        picDir = 4;
    }
    else if (h->GetPressedLeft()) {
        isShootUp = false;
        isShootDown = false;
        isShootLeft = true;
        isShootRight = false;
        picDir = 3;
    }
    SetXY(h);
    is_alive = true;
}

void Bullet::SetIsAlive(bool alive)
{
    is_alive = alive;
}

void Bullet::SetIsHit(bool flag)
{
    isHit = flag;
}

void Bullet::SetIsBlood(bool flag)
{
    isBlood = flag;
}

void Bullet::SetBloodXY(int X, int Y)
{
    bx = X;
    by = Y;
}

bool Bullet::GetBlood()
{
    return isBlood;
}

int Bullet::GetScreenX(Map* m)
{
    return m->ScreenX(x);
}

int Bullet::GetScreenY(Map* m)
{
    return m->ScreenY(y);
}

void Bullet::SetXY(Hero* h)
{
    if (isShootUp)
    {
        if (h->GetActivedBullet()) {
            x = h->GetX1() + 15;
            y = h->GetY1()-5;
        }
        else if (h->GetActivedMagic()) {
            x = h->GetX1()+5;
            y = h->GetY1()-25;
        }
        else if (h->GetActivedBlade()) {
            x = h->GetX1();
            y = h->GetY1()-25;
        }
    }
    else if (isShootDown)
    {
        if (h->GetActivedBullet()) {
            x = h->GetX1() + 15;
            y = h->GetY2()-30;
        }
        else if (h->GetActivedMagic()) {
            x = h->GetX1()+5;
            y = h->GetY2()-30;
        }
        else if (h->GetActivedBlade()) {
            x = h->GetX1();
            y = h->GetY2()-30;
        }
    }
    else if (isShootLeft)
    {
        if(h->isNormalMode)
        {
            if (h->GetActivedBullet()) {
                x = h->GetX1();
                y = h->GetY1() + 10;
            }
            else if (h->GetActivedMagic()) {
                x = h->GetX1() - 40;
                y = h->GetY1() ;
            }
            else if (h->GetActivedBlade()) {
                x = h->GetX1() - 40;
                y = h->GetY1() -10;
            }
        }
        else
        {
            if (h->GetActivedBullet()) {
                x = h->GetX1();
                y = h->GetY1() + 40;
            }
            else if (h->GetActivedMagic()) {
                x = h->GetX1() - 40;
                y = h->GetY1() + 30;
            }
            else if (h->GetActivedBlade()) {
                x = h->GetX1() - 40;
                y = h->GetY1() + 20;
            }
        }
    }
    else if (isShootRight)
    {
        if (h->isNormalMode)
        {
            if (h->GetActivedBullet()) {
                x = h->GetX1() + 30;
                y = h->GetY1() + 10;
            }
            else if (h->GetActivedMagic()) {
                x = h->GetX1() + 40;
                y = h->GetY1() ;
            }
            else if (h->GetActivedBlade()) {
                x = h->GetX1() + 40;
                y = h->GetY1() - 10;
            }
        }
        else
        {
            if (h->GetActivedBullet()) {
                x = h->GetX1() + 30;
                y = h->GetY1() + 40;
            }
            else if (h->GetActivedMagic()) {
                x = h->GetX1() + 40;
                y = h->GetY1() + 30;
            }
            else if (h->GetActivedBlade()) {
                x = h->GetX1() + 40;
                y = h->GetY1() + 20;
            }
        }
    }
}

bool Bullet::GetShootUp()
{
    return isShootUp;
}

bool Bullet::GetShootDown()
{
    return isShootDown;
}

bool Bullet::GetShootRight()
{
    return isShootRight;
}

bool Bullet::GetShootLeft()
{
    return isShootLeft;
}

bool Bullet::HitBlock(Map* m,Hero* h)
{
    return (!(m->isEmpty(x, y) && m->isEmpty(GetX2(h), GetY2(h))));
}

bool Bullet::OverBoundary(Map* m)
{
    return (GetScreenX(m) < 0 || GetScreenX(m) > 800 || GetScreenY(m) < 0 || GetScreenY(m) > 600);
}

int Bullet::GetX1()
{
    return x+10;
}

int Bullet::GetY1()
{
    return y+10;
}

int Bullet::GetX2(Hero* h)
{
    int x1 = x + kongqipao[0].Width()-10;
    int x2= x + magic[0].Width()-10;
    int x3 = x + Blade.Width()-10;
    if (h->GetActivedBullet()) {
        return  x1;
    }
    else if (h->GetActivedMagic()) {
        return x2;
    }
    return x3;
}

int Bullet::GetY2(Hero* h)
{
    int y1 = y + kongqipao[0].Height()-10;
    int y2 = y + magic[0].Height()-10;
    int y3 = y + Blade.Height()-10;
    if (h->GetActivedBullet()) {
        return y1;
    }
    else if (h->GetActivedMagic()) {
        return y2;
    }
    return y3;
}

bool Bullet::HitRectangle(int tx1, int ty1, int tx2, int ty2,Hero* h)
{
    int x1 = x;
    int y1 = y;
    int x2=0, y2=0;
    if (h->GetActivedMagic())
    {
        x2 = x1 +magic[0].Width()-10;
        y2 = y1 +magic[0].Height()-10;
    }
    else if (h->GetActivedBullet())
    {
        x2 = x1 + bullet.Width();
        y2 = y1 + bullet.Height();
    }
    else if (h->GetActivedBlade())
    {
        x2 = x1 + Blade.Width() - 10;
        y2 = y1 + Blade.Height() - 10;
    }
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}
}
