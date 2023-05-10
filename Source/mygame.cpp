#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include <string>


namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g)
{
    pressCount = counter = music_counter = 0;
    playCG = isShowHelp = isShowAbout = false;
}

void CGameStateInit::OnInit()
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(0);	// 一開始的loading進度為0%
    //
    // 開始載入資料
    //
    background.LoadBitmap(".\\bitmaps\\start.bmp");
    start.LoadBitmap(".\\bitmaps\\rest.bmp");
    const int a = 2;
    char* pic0[a] = { ".\\bitmaps\\select0.bmp",".\\bitmaps\\select1.bmp" };
    char* pic1[a] = { ".\\bitmaps\\select2.bmp",".\\bitmaps\\select3.bmp" };
    char* pic2[a] = { ".\\bitmaps\\select4.bmp",".\\bitmaps\\select5.bmp" };
    char* cg_1[5] = { ".\\bitmaps\\cg (1).bmp",  ".\\bitmaps\\cg (2).bmp",  ".\\bitmaps\\cg (3).bmp",  ".\\bitmaps\\cg (4).bmp",  ".\\bitmaps\\cg (5).bmp"  };
    char* cg_3[5] = { ".\\bitmaps\\cg (10).bmp", ".\\bitmaps\\cg (11).bmp", ".\\bitmaps\\cg (12).bmp", ".\\bitmaps\\cg (13).bmp", ".\\bitmaps\\cg (14).bmp" };
    for (int i = 0; i < a; i++) {
        button0.AddBitmap(pic0[i], RGB(255, 255, 255));
        button1.AddBitmap(pic1[i], RGB(255, 255, 255));
        button2.AddBitmap(pic2[i], RGB(255, 255, 255));
    }
    for (int i = 0; i < 5; i++) {
        cg1.AddBitmap(cg_1[i]);
        cg3.AddBitmap(cg_3[i]);
    }
    cg2_1.LoadBitmapA(".\\bitmaps\\cg (6).bmp");
    cg2_2.LoadBitmapA(".\\bitmaps\\cg (8).bmp");
    help.LoadBitmapA(".\\bitmaps\\help.bmp");
    about.LoadBitmapA(".\\bitmaps\\about.bmp");
    //
    // 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
    //
    CAudio::Instance()->Load(AUDIO_Doraemon, "sounds\\doraemon.mp3");
    CAudio::Instance()->Load(AUDIO_START, "sounds\\start.mp3");
    CAudio::Instance()->Load(AUDIO_select, "sounds\\select.wav");
    CAudio::Instance()->Load(AUDIO_startCG, "sounds\\start_cg.mp3");
}

void CGameStateInit::OnBeginState()
{
    music_counter = 0;
    if (score > highestScore) {
        highestScore = score;
    }
    score = 0;
    playCG = false;
    counter = 0;
    left = 3;
}

void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (playCG) {
        return;
    }
    const char KEY_ESC = 27;
    const char KEY_ENTER = 13;
    const char KEY_UP = 0x26;
    const char KEY_DOWN = 0x28;
    if (nChar == KEY_ESC && (isShowHelp || isShowAbout)) {
        isShowHelp = false;
        isShowAbout = false;
        CAudio::Instance()->Play(AUDIO_letter, false);
    }
    if (!isShowHelp && !isShowAbout)
    {
        if (nChar == KEY_UP) {
            if (pressCount >= 1 && pressCount <= 3) {
                pressCount--;
                CAudio::Instance()->Play(AUDIO_select, false);
            }
        }
        if (nChar == KEY_DOWN) {
            if (pressCount >= 0 && pressCount <= 2) {
                pressCount++;
                CAudio::Instance()->Play(AUDIO_select, false);
            }
        }
        if (nChar == KEY_ENTER)
        {
            if (pressCount == 0) {
                playCG = true;
                CAudio::Instance()->Play(AUDIO_startCG, false);
                CAudio::Instance()->Stop(AUDIO_Doraemon);
            }
            else if (pressCount == 1)
            {
                isShowHelp = true;
                CAudio::Instance()->Play(AUDIO_letter, false);
            }
            else if (pressCount == 2)
            {
                isShowAbout = true;
                CAudio::Instance()->Play(AUDIO_letter, false);
            }
            else {
                PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
            }
        }
    }
}

void CGameStateInit::OnShow()
{
    //
    // 貼上logo
    //
    // 切換至GAME_STATE_RUN
    background.ShowBitmap();
    if (pressCount == 0) {
        button0.SetTopLeft(230, 262);
        button0.SetDelayCount(6);
        button0.OnShow();
    }
    else if (pressCount == 1) {
        button0.SetTopLeft(230, 335);
        button0.SetDelayCount(6);
        button0.OnShow();
    }
    else if (pressCount == 2) {
        button1.SetTopLeft(232, 406);
        button1.SetDelayCount(6);
        button1.OnShow();
    }
    else {
        button2.SetTopLeft(273,485);
        button2.SetDelayCount(6);
        button2.OnShow();
    }
    //
    // Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
    //
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f,*fp;
    pDC->SetBkMode(TRANSPARENT);		// 設定顯示文字為透明背景
    f.CreatePointFont(140,"Times New Roman");	// 產生 font f; 160表示16 point的字
    fp=pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0,0,0));
    pDC->SetTextColor(RGB(0,0,0));
    char str[30];
    sprintf(str, "HighestScore: %d", highestScore);
    pDC->TextOut(550,40,str);
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    if (counter > 0 && counter <= 260) {
        cg1.OnShow();
    }
    if (counter > 260 && counter <= 275) {
        cg2_1.ShowBitmap();
    }
    if (counter > 275 && counter <= 290) {
        cg2_2.ShowBitmap();
    }
    if (counter > 290 && counter <= 330) {
        cg3.OnShow();
    }
    if (counter==331) {
        CAudio::Instance()->Play(AUDIO_START, false);
    }
    if (counter > 330 && counter <= 400)
    {
        start.ShowBitmap();
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
        CFont f, *fp;
        f.CreatePointFont(200, "Times New Roman");	// 產生 font f; 160表示16 point的字
        fp = pDC->SelectObject(&f);					// 選用 font f
        pDC->SetBkMode(TRANSPARENT);		// 設定顯示文字為透明背景
        pDC->SetBkColor(RGB(0, 0, 0));
        pDC->SetTextColor(RGB(0, 0, 0));
        char str[80], str2[80];								// Demo 數字對字串的轉換
        sprintf(str, "Score : %d", score);
        sprintf(str2, "Left   : %d", left);
        pDC->TextOut(400, 400, str);
        pDC->TextOut(400, 450, str2);
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();
    }
    if (isShowHelp) {
        help.ShowBitmap();
    }
    if (isShowAbout) {
        about.ShowBitmap();
    }
}

void CGameStateInit::OnMove()
{
    music_counter++;
    if (music_counter == 1) {
        CAudio::Instance()->Play(AUDIO_Doraemon, true);
    }
    button0.OnMove();
    button1.OnMove();
    button2.OnMove();
    if (counter == 400) {
        GotoGameState(GAME_STATE_RUN);
    }
    if(counter>0 && counter<=90) {
        cg1.SetDelayCount(4);
    }
    if(counter>90 && counter<=160) {
        cg1.SetDelayCount(3);
    }
    if (counter > 160 && counter <= 230) {
        cg1.SetDelayCount(2);
    }
    cg3.SetDelayCount(1);
    cg1.OnMove();
    cg3.OnMove();
    if (playCG) {
        counter++;
    }
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g)
{
    for (int i = 0; i < 10; i++) {
        dialogueText.push_back(bmp);
    }
    map = new Map(&hero);
    heroBullet = new Bullet(map);
    weaponFire[0] = new WeaponFire(map, &hero);
    for (int i = 0; i < 2; i++) {
        itemLetter[i] = new ItemLetter(map, &hero);    //new掉落的水果
    }
    for (int i = 0; i < 3; i++) {
        heroWeapon[i] = new ItemKongQiPao(map, &hero);
    }
    for (int i = 0; i < 16; i++) {
        item_1[i] = new Item(map, &hero, heroBullet);
    }
    for (int i = 0; i < 23; i++) {
        movingBlock[i] = new MovingBlock(map, &hero, heroBullet);
    }
    for (int i = 0; i < 7; i++) {
        starDoor[i] = new ItemDoor(map, &hero);
    }
    for (int i = 0; i < 2; i++) {
        itemKey[i] = new ItemKey(map, &hero);
    }
    castle = new CastleRed(map, &hero, heroBullet);
    for (int i = 0; i < 17; i++) {
        fireBall[i] = new EnemyFireBall(map, &hero);
    }
    for (int i = 0; i <= 5; i++) {
        enemies_1[i] = new EnemyFire(map, &hero, heroBullet);
    }
    for (int i = 6; i <= 8; i++) {
        enemies_1[i] = new EnemyBird(map, &hero, heroBullet);
    }
    for (int i = 9; i <= 10; i++) {
        enemies_1[i] = new EnemyPig(map, &hero, heroBullet);
    }
    for (int i = 11; i <= 12; i++) {
        enemies_1[i] = new EnemyTree(map, &hero, heroBullet);
    }
    for (int i = 13; i <= 20; i++) {
        enemies_1[i] = new EnemyMucus(map, &hero, heroBullet);
    }
    for (int i = 21; i <= 23; i++) {
        enemies_1[i] = new EnemyMummy(map, &hero, heroBullet);
    }
    for (int i = 24; i <= 25; i++) {
        enemies_1[i] = new EnemyShin(map, &hero, heroBullet);
    }
    for (int i = 26; i <= 27; i++) {
        enemies_1[i] = new EnemyBigFire(map, &hero, heroBullet);
    }
    for (int i = 28; i <= 29; i++) {
        enemies_1[i] = new EnemyBirdVertical(map, &hero, heroBullet);
    }
    for (int i = 30; i <= 31; i++) {
        enemies_1[i] = new EnemyFirePig(map, &hero, heroBullet);
    }
    for (int i = 0; i <= 2; i++) {
        enemies_3[i] = new EnemyUFO(map, &hero, heroBullet);
    }
    for (int i = 3; i <= 5; i++) {
        enemies_3[i] = new EnemyBird(map, &hero, heroBullet);
    }
    for (int i = 6; i <= 11; i++) {
        enemies_3[i] = new EnemyBat(map, &hero, heroBullet);
    }
    for (int i = 12; i <= 15; i++) {
        enemies_3[i] = new EnemyBooger(map, &hero, heroBullet);
    }
    bossMan = new EnemyBossMan(map, &hero, heroBullet);
    skullSoul = new EnemySkullSoul(map, &hero, heroBullet);
}

CGameStateRun::~CGameStateRun()
{
    delete map;
    delete heroBullet;
    delete castle;
    delete bossMan;
    delete skullSoul;
    delete weaponFire[0];
    for (int i = 0; i < 2; i++) {
        delete itemLetter[i];
        delete itemKey[i];
    }
    for (int i = 0; i < 3; i++) {
        delete heroWeapon[i];
    }
    for (int i = 0; i < 7; i++) {
        delete starDoor[i];
    }
    for (int i = 0; i < 16; i++) {
        delete item_1[i];
    }
    for (int i = 0; i < 23; i++) {
        delete movingBlock[i];
    }
    for (int i = 0; i < 17; i++) {
        delete fireBall[i];
    }
    for (int i = 0; i < enemyNumber_3; i++) {
        delete enemies_3[i];
    }
    for (int i = 0; i < enemyNumber_1; i++) {
        delete enemies_1[i];
    }
}

void CGameStateRun::OnBeginState()
{
    for (int i = 0; i < enemyNumber_1; i++) {
        enemies_1[i]->ResetBlood();
    }
    for (int i = 0; i < enemyNumber_3; i++) {
        enemies_3[i]->ResetBlood();
    }
    for (int i = 0; i < 16; i++) {
        item_1[i]->ResetBlood();
    }
    weaponFire[0]->SetIsAlive(true);
    black_counter = dead_counter = 0;
    isShowBlack = false;
    bossMan->ResetBlood();
    hero.RefreshAnimation();
    hero.ResetBloodAmount();
    if (MapLevel >= 31 && MapLevel <= 34) {
        ResetMovingBlock();
    }
    if (reset) {
        ResetEverything();
    }
    if(MapLevel==10)
    {
        if (bossMan->IsAlive()) {
            CAudio::Instance()->Play(AUDIO_boss1, true);
        }
        else {
            CAudio::Instance()->Play(AUDIO_map1, true);
        }
    }
    else if (MapLevel >= 11 && MapLevel <= 15)
    {
        switch (MapLevel)
        {
        case 11:
            hero.SetXY(250, 80);
            map->SetXY(64, 64);
            break;
        case 12:
            hero.SetXY(70, 90);
            map->SetXY(64, 64);
            break;
        case 13:
            hero.SetXY(777, 170);
            map->SetXY(415, 64);
            break;
        case 14:
            hero.SetXY(775, 95);
            map->SetXY(420, 64);
            break;
        case 15:
            hero.SetXY(630, 246);
            map->SetXY(200, 64);
            break;
        }
        CAudio::Instance()->Play(AUDIO_map1_, true);
    }
    else if (MapLevel == 30)
    {
        if (skullSoul->IsAlive()) {
            CAudio::Instance()->Play(AUDIO_horror, true);
        }
        else {
            CAudio::Instance()->Play(AUDIO_map3, true);
        }
    }
    else {
        CAudio::Instance()->Play(AUDIO_map3_, true);
    }
}

void CGameStateRun::ResetMovingBlock()
{
    movingBlock[0]->SetXY(384,384); //6,6
    movingBlock[1]->SetXY(576, 384); //9,6
    movingBlock[2]->SetXY(768, 576); //12,9
    movingBlock[3]->SetXY(832, 320); //13,5
    movingBlock[4]->SetXY(256, 512); //4,8
    movingBlock[5]->SetXY(256, 576); //4,9
    movingBlock[6]->SetXY(384, 384); //6,6
    movingBlock[7]->SetXY(512, 384); //8,6
    movingBlock[8]->SetXY(640, 448); //10,7
    movingBlock[9]->SetXY(704, 320); //11,5
    movingBlock[10]->SetXY(128, 512); //2,8
    movingBlock[11]->SetXY(320, 512); //5,8
    movingBlock[12]->SetXY(384, 448); //6,7
    movingBlock[13]->SetXY(640, 576); //10,9
    movingBlock[14]->SetXY(256, 256); //4,4
    movingBlock[15]->SetXY(448, 192); //7,3
    movingBlock[16]->SetXY(512, 192); //8,3
    movingBlock[17]->SetXY(704, 640); //11,10
    movingBlock[18]->SetXY(384, 448); //6,7
    movingBlock[19]->SetXY(384, 576); //6,9
    movingBlock[20]->SetXY(384, 640); //6,10
    movingBlock[21]->SetXY(640, 320); //10,5
    movingBlock[22]->SetXY(640, 512); //10,8
}

void CGameStateRun::MapChangingEffect()
{
    hero.RefreshAnimation();
    isShowBlack = true;
    CAudio::Instance()->Play(AUDIO_intohole);
    if (MapLevel >= 11 && MapLevel <= 15)
    {
        CAudio::Instance()->Play(AUDIO_map1, true);
        CAudio::Instance()->Stop(AUDIO_map1_);
        hero.ChangeToNormalMode();
    }
    else if (MapLevel == 10)
    {
        CAudio::Instance()->Play(AUDIO_map1_, true);
        CAudio::Instance()->Stop(AUDIO_map1);
        hero.ChangeToJumpMode();
    }
    else if (MapLevel >= 31 && MapLevel <= 35)
    {
        CAudio::Instance()->Play(AUDIO_map3, true);
        CAudio::Instance()->Stop(AUDIO_map3_);
        hero.ChangeToJumpMode();
    }
    else if (MapLevel == 30)
    {
        CAudio::Instance()->Play(AUDIO_map3_, true);
        CAudio::Instance()->Stop(AUDIO_map3);
        hero.ChangeToNormalMode();
    }
}

void CGameStateRun::ResetEverything()		//重置所有
{
    dialogue_counter = black_counter = dead_counter = space_counter = weaponFire_distance = 0;
    isPressSpace = isTalking = isPlayEnding = false;
    enemyCount = 39;
    endingMoveX = -6700;
    map->Initialize();
    map->MapLevel = MapLevel = 10;	//重置關卡
    hero.Initialize();
    hero.SetXY(2725, 2380);
    map->SetXY(2325, 1960);
    ResetMovingBlock();
    for (int i = 0; i < 16; i++) {
        item_1[i]->Initialize();
    }
    for (int i = 0; i < enemyNumber_1; i++) {
        enemies_1[i]->Initialize();
    }
    for (int i = 0; i < enemyNumber_3; i++) {
        enemies_3[i]->Initialize();
    }
    for (int i = 0; i < 17; i++) {
        fireBall[i]->Initialize();
    }
    for (int i = 0; i < 2; i++) {
        itemKey[i]->Initialize();
        isDoorOpen[i] = false;
    }
    enemies_1[0]->SetXY(880, 860);
    enemies_1[1]->SetXY(110, 1600);
    enemies_1[2]->SetXY(1500, 1900);
    enemies_1[3]->SetXY(750, 500);
    enemies_1[4]->SetXY(125, 320);
    enemies_1[5]->SetXY(670, 380);
    enemies_1[6]->SetXY(2800, 1325);
    enemies_1[7]->SetXY(3050, 2100);
    enemies_1[8]->SetXY(1800, 2300);
    enemies_1[9]->SetXY(1800, 1700);
    enemies_1[10]->SetXY(500, 2400);
    enemies_1[11]->SetXY(2650, 130);
    enemies_1[12]->SetXY(1450, 360);
    enemies_1[13]->SetXY(2500, 1150);
    enemies_1[14]->SetXY(1750, 600);
    enemies_1[15]->SetXY(1450, 1000);
    enemies_1[16]->SetXY(2500, 800);
    enemies_1[17]->SetXY(300, 1400);
    enemies_1[18]->SetXY(2700, 970);
    enemies_1[19]->SetXY(2400, 270);
    enemies_1[20]->SetXY(700, 2200);
    enemies_1[21]->SetXY(2050, 450);
    enemies_1[22]->SetXY(2800, 2300);
    enemies_1[23]->SetXY(1200, 1180);
    enemies_1[24]->SetXY(2400, 1800);
    enemies_1[25]->SetXY(400, 1800);
    enemies_1[26]->SetXY(2180,2400);
    enemies_1[27]->SetXY(1370, 1370);
    enemies_1[28]->SetXY(1130, 1900);
    enemies_1[29]->SetXY(3050, 1150);
    enemies_1[30]->SetXY(3023, 498);
    enemies_1[31]->SetXY(1485, 370);
    enemies_3[0]->SetXY(10400, 570);
    enemies_3[1]->SetXY(6350, 500);
    enemies_3[2]->SetXY(2900, 80);
    enemies_3[3]->SetXY(1150, 160);
    enemies_3[4]->SetXY(5000, 140);
    enemies_3[5]->SetXY(5300, 160);
    enemies_3[6]->SetXY(2080, 380);
    enemies_3[7]->SetXY(4300, 480);
    enemies_3[8]->SetXY(7800, 200);
    enemies_3[9]->SetXY(8700, 400);
    enemies_3[10]->SetXY(8200, 85);
    enemies_3[11]->SetXY(6800, 200);
    enemies_3[12]->SetXY(10650, 598);
    enemies_3[13]->SetXY(9500, 151);
    enemies_3[14]->SetXY(10200, 598);
    enemies_3[15]->SetXY(3140, 279);
    itemLetter[0]->SetXY(2890, 2375);
    itemLetter[1]->SetXY(485, 1190);
    starDoor[0]->SetXY(448,1088);
    starDoor[1]->SetXY(448, 1310);
    starDoor[2]->SetXY(448, 192);
    starDoor[3]->SetXY(448, 64);
    starDoor[4]->SetXY(448, 128);
    starDoor[5]->SetXY(448, 64);
    starDoor[6]->SetXY(448, 64);
    heroWeapon[0]->SetXY(140, 1100);	//air cannon
    heroWeapon[1]->SetXY(290, 140);
    heroWeapon[1]->SetType(2);		//dragon magic
    heroWeapon[2]->SetXY(6420, 530);
    heroWeapon[2]->SetType(1);		//rotating dart
    itemKey[0]->SetXY(700, 1200);
    itemKey[1]->SetXY(10440, 610);
    for (int i = 0; i <= 5; i++) {
        item_1[i]->SetType("BloodUp");
    }
    for (int i = 6; i <= 11; i++) {
        item_1[i]->SetType("Bell");
    }
    for (int i = 12; i <= 13; i++) {
        item_1[i]->SetType("CureBottle");
    }
    for (int i = 14; i <= 15; i++) {
        item_1[i]->SetType("Guard");
    }
    for (int i = 2; i < 14; i++)
    {
        if (i >= 6 && i <= 7) {
            continue;
        }
        item_1[i]->SetHidden(true);
    }
    for (int i = 0; i <enemyNumber_3; i++) {
        enemies_3[i]->SetIsAlive(true);
    }
    item_1[0]->SetXY(780,1400);	//1
    item_1[1]->SetXY(2100,1006);	//1
    item_1[2]->SetXY(1330,1560);	//1i
    item_1[3]->SetXY(2900,278);	//1i
    item_1[4]->SetXY(710,260);		//1-1 i
    item_1[5]->SetXY(250,180);		//1-3 i
    item_1[6]->SetXY(110,1760);		//1
    item_1[7]->SetXY(1970,2240);		//1
    item_1[8]->SetXY(1910,1000);		//1i
    item_1[9]->SetXY(1350,150);		//1-4i
    item_1[10]->SetXY(290,130);		//1-2i
    item_1[11]->SetXY(550,405);		//1-1i
    item_1[12]->SetXY(3140,500);
    item_1[13]->SetXY(1540,350);
    item_1[14]->SetXY(2015, 380);
    item_1[15]->SetXY(3060, 1670);
    fireBall[0]->SetXY(100, 50);	//down
    fireBall[0]->SetType("down");
    fireBall[1]->SetXY(300, 600);
    fireBall[2]->SetXY(495, 50);	//down
    fireBall[2]->SetType("down");
    fireBall[3]->SetXY(690, 600);
    fireBall[4]->SetXY(875, 600);
    fireBall[5]->SetXY(1070, 50);	//down
    fireBall[5]->SetType("down");
    fireBall[6]->SetXY(1260, 600);
    fireBall[7]->SetXY(1455, 50);	//down
    fireBall[7]->SetType("down");
    fireBall[8]->SetXY(400, 600);
    fireBall[9]->SetXY(715, 600);
    fireBall[10]->SetXY(1135, 50);	//down
    fireBall[10]->SetType("down");
    fireBall[11]->SetXY(1458, 50);	//down
    fireBall[11]->SetType("down");
    fireBall[12]->SetXY(2125, 600);
    fireBall[13]->SetXY(560, 600);
    fireBall[14]->SetXY(845, 50);
    fireBall[14]->SetType("down");
    fireBall[15]->SetXY(1035, 600);
    fireBall[16]->SetXY(1200, 50);
    fireBall[16]->SetType("down");
    weaponFire[0]->SetXY(80,200);  //1-3
    enemies_1[30]->SetIsAlive(true);
    enemies_1[31]->SetIsAlive(true);
    itemLetter[0]->SetIsAlive(true);
    itemLetter[1]->SetIsAlive(false);
    bossMan->Initialize();
    bossMan->SetXY(65, 400);
    skullSoul->Initialize();
    skullSoul->SetXY(12000, 150);
    castle->Initialize();
    reset = false;
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
    //
    // 開始載入資料
    //
    hero.LoadBitmap();
    //
    // 完成部分Loading動作，提高進度
    //
    // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 繼續載入其他資料
    //
    nobi.AddBitmap(".\\bitmaps\\nobi1.bmp", RGB(255, 255, 255));
    nobi.AddBitmap(".\\bitmaps\\nobi2.bmp", RGB(255, 255, 255));
    dialogueText[0].LoadBitmap(".\\bitmaps\\text0.bmp", RGB(255, 255, 255));
    dialogueText[1].LoadBitmap(".\\bitmaps\\text1.bmp", RGB(255, 255, 255));
    dialogueText[2].LoadBitmap(".\\bitmaps\\text2.bmp", RGB(255, 255, 255));
    dialogueText[3].LoadBitmap(".\\bitmaps\\text3.bmp", RGB(255, 255, 255));
    dialogueText[4].LoadBitmap(".\\bitmaps\\text4.bmp", RGB(255, 255, 255));
    dialogueText[5].LoadBitmap(".\\bitmaps\\text5.bmp", RGB(255, 255, 255));
    dialogueText[6].LoadBitmap(".\\bitmaps\\text6.bmp", RGB(255, 255, 255));
    dialogueText[7].LoadBitmap(".\\bitmaps\\text7.bmp", RGB(255, 255, 255));
    dialogueText[8].LoadBitmap(".\\bitmaps\\text8.bmp", RGB(255, 255, 255));
    dialogueText[9].LoadBitmap(".\\bitmaps\\text9.bmp", RGB(255, 255, 255));
    door.LoadBitmap("Bitmaps/item/door.bmp", RGB(255, 255, 255));
    door2.LoadBitmap("Bitmaps/item/door2.bmp", RGB(255, 255, 255));
    black.LoadBitmap(".\\bitmaps\\black.bmp");
    endingBackground.LoadBitmap(".\\bitmaps\\ending.bmp");
    end.LoadBitmap(".\\bitmaps\\end.bmp");
    char* walking[4] = { ".\\bitmaps\\walking00.bmp",".\\bitmaps\\walking01.bmp",".\\bitmaps\\walking02.bmp",".\\bitmaps\\walking03.bmp" };
    char* fireWork[17] = { ".\\bitmaps\\0.bmp", ".\\bitmaps\\1.bmp", ".\\bitmaps\\2.bmp", ".\\bitmaps\\3.bmp", ".\\bitmaps\\4.bmp", ".\\bitmaps\\5.bmp",
                           ".\\bitmaps\\6.bmp", ".\\bitmaps\\7.bmp", ".\\bitmaps\\8.bmp", ".\\bitmaps\\9.bmp", ".\\bitmaps\\10.bmp", ".\\bitmaps\\11.bmp",
                           ".\\bitmaps\\12.bmp", ".\\bitmaps\\13.bmp", ".\\bitmaps\\14.bmp", ".\\bitmaps\\15.bmp",".\\bitmaps\\16.bmp"
                         };
    for (int i = 0; i < 4; i++) {
        allWalking.AddBitmap(walking[i], RGB(255, 255, 255));
    }
    for (int i = 0; i < 17; i++) {
        firework.AddBitmap(fireWork[i], RGB(255, 255, 255));
    }
    map->LoadBitmap();
    bossMan->LoadBitmap();
    skullSoul->LoadBitmap();
    heroBullet[0].LoadBitmap();
    castle->LoadBitmap();
    weaponFire[0]->LoadBitmap();
    for (int i = 0; i < 16; i++) {
        item_1[i]->LoadBitmap();
    }
    for (int i = 0; i < 17; i++) {
        fireBall[i]->LoadBitmap();
    }
    for (int i = 0; i < 3; i++) {
        heroWeapon[i]->LoadBitmap();
    }
    for (int i = 0; i < 23; i++) {
        movingBlock[i]->LoadBitmap();
    }
    for (int i = 0; i < 2; i++) {
        itemKey[i]->LoadBitmap();
    }
    for (int i = 0; i < 7; i++) {
        starDoor[i]->LoadBitmap();
    }
    for (int i = 0; i < enemyNumber_1; i++) {
        enemies_1[i]->LoadBitmap();
    }
    for (int i = 0; i < enemyNumber_3; i++) {
        enemies_3[i]->LoadBitmap();
    }
    for (int i = 0; i < 2; i++) {
        itemLetter[i]->LoadBitmap();
    }
    CAudio::Instance()->Load(AUDIO_pick01, "sounds\\pick01.wav");
    CAudio::Instance()->Load(AUDIO_hit01, "sounds\\hit01.wav");
    CAudio::Instance()->Load(AUDIO_hit02, "sounds\\hit02.mp3");
    CAudio::Instance()->Load(AUDIO_jump, "sounds\\jump.mp3");
    CAudio::Instance()->Load(AUDIO_pick02, "sounds\\pick02.wav");
    CAudio::Instance()->Load(AUDIO_shoot2, "sounds\\shoot2.wav");
    CAudio::Instance()->Load(AUDIO_shoot3, "sounds\\shoot3.mp3");
    CAudio::Instance()->Load(AUDIO_shoot5, "sounds\\shoot5.mp3");
    CAudio::Instance()->Load(AUDIO_map1, "sounds\\map1.mp3");
    CAudio::Instance()->Load(AUDIO_map1_, "sounds\\map1_.mp3");
    CAudio::Instance()->Load(AUDIO_map3, "sounds\\map3.mp3");
    CAudio::Instance()->Load(AUDIO_map3_, "sounds\\map3_.mp3");
    CAudio::Instance()->Load(AUDIO_OVER, "sounds\\over.wav");
    CAudio::Instance()->Load(AUDIO_DEAD, "sounds\\dead.mp3");
    CAudio::Instance()->Load(AUDIO_PASS, "sounds\\pass.mp3");
    CAudio::Instance()->Load(AUDIO_win, "sounds\\win.mp3");
    CAudio::Instance()->Load(AUDIO_injure, "sounds\\injure3.mp3");
    CAudio::Instance()->Load(AUDIO_intohole, "sounds\\intoHole.mp3");
    CAudio::Instance()->Load(AUDIO_bird, "sounds\\bird.wav");
    CAudio::Instance()->Load(AUDIO_pig, "sounds\\pig.wav");
    CAudio::Instance()->Load(AUDIO_castlered, "sounds\\castleRed.mp3");
    CAudio::Instance()->Load(AUDIO_getweapon, "sounds\\getweapon.mp3");
    CAudio::Instance()->Load(AUDIO_manhurt, "sounds\\man_uha.wav");
    CAudio::Instance()->Load(AUDIO_manshout, "sounds\\man_ha.wav");
    CAudio::Instance()->Load(AUDIO_boss1, "sounds\\boss.mp3");
    CAudio::Instance()->Load(AUDIO_key, "sounds\\pickkey.wav");
    CAudio::Instance()->Load(AUDIO_letter, "sounds\\pickletter.wav");
    CAudio::Instance()->Load(AUDIO_hittree, "sounds\\hittree.wav");
    CAudio::Instance()->Load(AUDIO_hitfire, "sounds\\hitfire.wav");
    CAudio::Instance()->Load(AUDIO_hitfrog, "sounds\\hitfrog.wav");
    CAudio::Instance()->Load(AUDIO_soul, "sounds\\soul.mp3");
    CAudio::Instance()->Load(AUDIO_horror, "sounds\\horror.mp3");
    CAudio::Instance()->Load(AUDIO_hit05, "sounds\\hit05.wav");
    CAudio::Instance()->Load(AUDIO_hit07, "sounds\\hit07.wav");
    CAudio::Instance()->Load(AUDIO_hit11, "sounds\\hit11.wav");
    CAudio::Instance()->Load(AUDIO_hit12, "sounds\\hit12.mp3");
    CAudio::Instance()->Load(AUDIO_hit13, "sounds\\hit13.mp3");
    CAudio::Instance()->Load(AUDIO_firepig, "sounds\\firepig.wav");
    ShowInitProgress(50);
    //
    // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
    //
}


void CGameStateRun::OnMove()							// 移動遊戲元素
{
    //
    // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
    //
    //SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    if (!isTalking) {
        hero.OnMove(map);
    }
    map->OnMove();
    heroBullet->OnMove(&hero, map);
    if (isPressSpace == true) {
        space_counter++;
    }
    if (space_counter > 10) {
        isPressSpace = false;    //控制射擊的間隔
        space_counter = 0;
    }
    ////////////////////////清空小怪後的動作///////////////////////////////////
    if (MapLevel >= 10 && MapLevel <= 14)
    {
        if (enemyCount == 0 && dialogue_counter == 300) {
            isTalking = true;
        }
        if (dialogue_counter == 301) {
            CAudio::Instance()->Play(AUDIO_letter, false);
        }
        if (dialogue_counter == 449) {
            enemyCount--;
            itemKey[0]->SetIsAlive(true);
        }
    }
    if (hero.Dead())
    {
        if (dead_counter == 0)
        {
            CAudio::Instance()->Play(AUDIO_DEAD, false);
            CAudio::Instance()->Stop(AUDIO_map1);
            CAudio::Instance()->Stop(AUDIO_map1_);
            CAudio::Instance()->Stop(AUDIO_map3);
            CAudio::Instance()->Stop(AUDIO_map3_);
            CAudio::Instance()->Stop(AUDIO_boss1);
            CAudio::Instance()->Stop(AUDIO_horror);
            hero.ReduceLife();
            left = hero.GetLifeAmount();
            hero.SetInjured(false);
        }
        dead_counter++;
    }
    if (isShowBlack) {
        black_counter++;
    }
    if (black_counter == 10) {
        black_counter = 0;    //顯示黑畫面的時間
        isShowBlack=false;
    }
    if (dead_counter == 120 && (!hero.Over())) {
        isShowBlack=true;
        GotoGameState(GAME_STATE_REST);
    }
    if (hero.Over() && dead_counter == 120) {
        isShowBlack = true;
        GotoGameState(GAME_STATE_OVER);
    }
    if (isTalking) {
        dialogue_counter++;
    }
    if (dialogue_counter == 150 || dialogue_counter == 300 || dialogue_counter == 450 ||
            dialogue_counter == 600 || dialogue_counter == 750 || dialogue_counter == 1300 ||
            dialogue_counter == 1450 || dialogue_counter == 1600) {
        isTalking = false;
    }
///////////////////////////////////////////////////////第一個關卡的code//////////////////////////////////////////////////////////////////////////
    if (MapLevel == 10)
    {
        for (int i = 0; i < enemyNumber_1 - 2; i++)
        {
            if (i >= 3 && i <= 5) {
                continue;
            }
            if (enemies_1[i]->HitHero() || bossMan->HitHero()) {
                hero.ExecuteInjureEffect();    //英雄碰到敵人的動作
            }
        }
        nobi.SetDelayCount(6);
        nobi.OnMove();
        bossMan->OnMove();
        item_1[15]->OnMove();
        itemKey[0]->OnMove();
        for (int i = 0; i <= 3; i++) {
            item_1[i]->OnMove();
        }
        for (int i = 6; i <= 8; i++) {
            item_1[i]->OnMove();
        }
        for (int i = 0; i < enemyNumber_1-2; i++)
        {
            if (i >= 3 && i <= 5) {
                continue;
            }
            enemies_1[i]->OnMove();
        }
        /////////////////////////////////////////進入黑洞口的動作////////////////////////////////////////////////////////
        if (hero.GetFootX() > 3080 && hero.GetFootX() < 3130 && hero.GetFootY() > 210 && hero.GetFootY() < 240)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 12;
            hero.SetXY(1400, 180);
            map->SetXY(800, 64);
        }
        if (hero.GetFootX() > 1285 && hero.GetFootX() < 1340 && hero.GetFootY() > 1045 && hero.GetFootY() < 1070)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 11;
            hero.SetXY(250, 80);
            map->SetXY(64, 64);
        }
        if (hero.GetFootX() > 520 && hero.GetFootX() < 570 && hero.GetFootY() > 1040 && hero.GetFootY() < 1070)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 11;
            hero.SetXY(1420, 90);
            map->SetXY(800, 64);
        }
        if (hero.GetFootX() > 965 && hero.GetFootX() < 1015 && hero.GetFootY() > 1230 && hero.GetFootY() < 1260)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 14;
            hero.SetXY(1540, 100);
            map->SetXY(800, 64);
        }
        if (hero.GetFootX() > 1925 && hero.GetFootX() < 1980 && hero.GetFootY() > 1440 && hero.GetFootY() < 1465)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 14;
            hero.SetXY(0, 100);
            map->SetXY(0, 64);
        }
        if (hero.GetFootX() > 135 && hero.GetFootX() < 180 && hero.GetFootY() > 2450 && hero.GetFootY() < 2480)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 13;
            hero.SetXY(1535, 350);
            map->SetXY(800, 64);
        }
        if (hero.GetFootX() > 2310 && hero.GetFootX() < 2360 && hero.GetFootY() > 2065 && hero.GetFootY() < 2098)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 12;
            hero.SetXY(70, 90);
            map->SetXY(64, 64);
        }
        if (hero.GetFootX() > 3080 && hero.GetFootX() < 3130 && hero.GetFootY() > 1170 && hero.GetFootY() < 1200)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 13;
            hero.SetXY(20, 350);
            map->SetXY(0, 64);
        }
        if (hero.GetFootX() > 140 && hero.GetFootX() < 175 && hero.GetFootY() > 150 &&
                hero.GetFootY() < 165 && isDoorOpen[0])	//走進任意門的動作
        {
            CAudio::Instance()->Play(AUDIO_START);
            castle->SetIsAlive(false);
            map->MapLevel = MapLevel = 30;
            enemyCount = enemyNumber_3;
            hero.ChangeToJumpMode();
            map->SetXY(64, 50);
            hero.SetXY(150, 370);
            hero.ResetBloodAmount();
            GotoGameState(GAME_STATE_REST);
        }
        if (castle->GetBloodAmount() == 1)			// enter castle and fight boss
        {
            map->ChangeMapValue(1, 32, 1);
            map->ChangeMapValue(1, 33, 1);
            if (hero.GetFootX() > 2090 && hero.GetFootX() < 2130 && hero.GetFootY() > 190 && hero.GetFootY() < 210)
            {
                MapChangingEffect();
                MapLevel = map->MapLevel = 15;
                hero.SetXY(30, 480);
                map->SetXY(0, 64);
            }
        }
////////////////////////////////////////////////////進入黑洞口的動作////////////////////////////////////////////////////////////
//////////////////////////////////////////// 觸發劇情 //////////////////////////////////////////////////////////
        if (itemLetter[0]->HitHero()) {
            heroWeapon[0]->SetIsAlive(true);    //撿到第一封信後出現空氣炮武器
            isTalking = true;
        }
        if (heroWeapon[0]->HitHero())			//撿到空氣炮後的動作
        {
            isTalking = true;
            CAudio::Instance()->Play(AUDIO_getweapon);
            hero.GetedBullet(true);
            hero.ActivedBullet(true);
            for (int i = 0; i < enemyNumber_1 - 2; i++)
            {
                enemies_1[i]->SetIsAlive(true);
            }
            heroWeapon[0]->SetIsAlive(false);
        }
        ////////////////////////撿到藥匙之後出現第二封信件///////////////////////////////////
        if (itemKey[0]->HitHero())
        {
            map->ChangeMapValue(17, 7, 0);
            map->ChangeMapValue(17, 8, 0);
            map->ChangeMapValue(20, 7, 0);
            map->ChangeMapValue(20, 8, 0);
            starDoor[0]->SetIsAlive(false);
            starDoor[1]->SetIsAlive(false);
            itemLetter[1]->SetIsAlive(true);
            itemKey[0]->SetIsAlive(false);
        }
        if (itemLetter[1]->HitHero()) {
            castle->SetIsAlive(true);    //撿到第二封信後城堡開啟
            isTalking = true;
        }
        ////////////////////////城堡出現後顯示提示畫面///////////////////////////////////
        if (castle->GetBloodAmount() == 2 && castle->HitByBullet()) {
            CAudio::Instance()->Play(AUDIO_castlered);
            isTalking = true;
        }
        if (castle->GetBloodAmount() > 1) {
            castle->BeHit();
        }
        /////////////////////////////////////////////////////////////////////////////////
        for (int i = 0; i < 2; i++)		//撿到信封的動作
        {
            if (itemLetter[i]->HitHero())
            {
                isTalking = true;
                itemLetter[i]->SetIsAlive(false);
                CAudio::Instance()->Play(AUDIO_letter);
            }
        }
        ////////////////////////打倒第一關boss後顯示提示畫面///////////////////////////////////
        if (bossMan->GetBloodAmount() == 0 && dialogue_counter == 750) {
            isTalking = true;
        }
        if (dialogue_counter == 751) {
            CAudio::Instance()->Stop(AUDIO_boss1);
        }
        if (dialogue_counter == 851 || dialogue_counter == 1001 || dialogue_counter == 1151) {
            CAudio::Instance()->Play(AUDIO_letter, false);
        }
        if (dialogue_counter == 1299) {
            heroWeapon[1]->SetIsAlive(true);
        }
        if (heroWeapon[1]->HitHero())			//撿到旋轉飛鏢後的動作
        {
            isTalking = true;
            CAudio::Instance()->Play(AUDIO_getweapon);
            hero.GetedMagic(true);
            hero.GetedBlade(true);
            isDoorOpen[0] = true;
            heroWeapon[1]->SetIsAlive(false);
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (MapLevel == 11)
    {
        enemies_1[3]->OnMove();
        item_1[11]->OnMove();
        if (enemies_1[3]->HitHero()) {
            hero.ExecuteInjureEffect();    //英雄碰到敵人的動作
        }
        for (int i = 13; i < 17; i++) {
            fireBall[i]->OnMove();
        }
        item_1[4]->OnMove();
        if (hero.GetX1() > 190 && hero.GetX1() < 300 && hero.GetY1() < 50)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 10;
            hero.SetXY(1280, 900);
            map->SetXY(960, 630);
        }
        if (hero.GetX1() > 1380 && hero.GetX1() < 1470 && hero.GetY1() < 30)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 10;
            hero.SetXY(390, 990);
            map->SetXY(0, 720);
        }
    }
    else if (MapLevel == 12)
    {
        enemies_1[4]->OnMove();
        item_1[10]->OnMove();
        item_1[13]->OnMove();
        enemies_1[31]->OnMove();
        if (enemies_1[31]->HitHero() || enemies_1[4]->HitHero())		//英雄碰到敵人的動作
        {
            hero.ExecuteInjureEffect();
        }
        if (hero.GetX1() > 50 && hero.GetX1() < 120 && hero.GetY1() < 30)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 10;
            hero.SetXY(2380, 2020);
            map->SetXY(2100, 1850);
        }
        if (hero.GetX1() > 1460 && hero.GetX1() < 1600 && hero.GetY1() < 30)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 10;
            hero.SetXY(3080, 240);
            map->SetXY(2400, 0);
        }
    }
    else if (MapLevel == 13)
    {
        enemies_1[5]->OnMove();
        item_1[5]->OnMove();
        weaponFire[0]->OnMove();
        if (enemies_1[5]->HitHero()) {
            hero.ExecuteInjureEffect();    //英雄碰到敵人的動作
        }
        if (weaponFire_distance >= 0 && weaponFire_distance < 1410) {
            weaponFire[0]->MoveX(15);
            weaponFire_distance+=15;
        }
        if (weaponFire_distance >= 1410 && weaponFire_distance < 2820) {
            weaponFire[0]->MoveX(-15);
            weaponFire_distance+=15;
        }
        if (weaponFire_distance == 2820) {
            weaponFire_distance = 0;
        }
        if (hero.GetX1() > 1580 && hero.GetY1() > 320 )
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 10;
            hero.SetXY(130, 2350);
            map->SetXY(0, 1960);
        }
        if (hero.GetX1() < -30 && hero.GetY1() > 320)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 10;
            hero.SetXY(3075, 1215);
            map->SetXY(2400, 1020);
        }
    }
    else if (MapLevel == 14)
    {
        for (int i = 0; i < 8; i++)
        {
            fireBall[i]->OnMove();
        }
        item_1[9]->OnMove();
        if (hero.GetX1() > 1580 && hero.GetY1() > 70)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 10;
            hero.SetXY(965, 1260);
            map->SetXY(820, 900);
        }
        if (hero.GetX1() < -30 && hero.GetY1() > 70)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 10;
            hero.SetXY(1860, 1380);
            map->SetXY(1330, 1200);
        }
    }
    else if (MapLevel == 15)
    {
        if (enemies_1[30]->HitHero()) {
            hero.ExecuteInjureEffect();    //英雄碰到敵人的動作
        }
        for (int i = 8; i < 13; i++) {
            fireBall[i]->OnMove();
        }
        item_1[14]->OnMove();
        item_1[12]->OnMove();
        enemies_1[30]->OnMove();
        if (hero.GetX1() > 3180 && hero.GetY1() > 400)
        {
            MapChangingEffect();
            CAudio::Instance()->Stop(AUDIO_map1);
            CAudio::Instance()->Play(AUDIO_boss1, true);
            bossMan->SetIsAlive(true);
            MapLevel = map->MapLevel = 10;
            hero.SetXY(900, 100);
            map->SetXY(350, 0);
        }
        if (hero.GetX1() < -30 && hero.GetY1() > 70)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 10;
            hero.SetXY(2090, 200);
            map->SetXY(1700, 0);
        }
    }
    else if (MapLevel == 30)
    {
        itemKey[1]->OnMove();
        for (int i = 0; i < enemyNumber_3; i++)
        {
            enemies_3[i]->OnMove();
            if (enemies_3[i]->HitHero()) {
                hero.ExecuteInjureEffect();    //英雄碰到敵人的動作
            }
        }
        if (itemKey[1]->HitHero() && !itemKey[1]->FollowedHero()) {
            itemKey[1]->SetFollowHero(true);
        }
        if (skullSoul->GetBloodAmount()==0 && dialogue_counter == 1450)
        {
            isTalking = true;
            CAudio::Instance()->Stop(AUDIO_horror);
            CAudio::Instance()->Play(AUDIO_win);
        }
        if (skullSoul->GetBloodAmount() == 0 && hero.GetX2() > 12770)
        {
            hero.SetXY(0, 0);
            isShowBlack = true;
            isTalking = true;
            isPlayEnding = true;
            CAudio::Instance()->Play(AUDIO_intohole);
            CAudio::Instance()->Play(AUDIO_PASS);
        }
        skullSoul->OnMove();
        if (dialogue_counter == 2960) {
            reset = true;
            GotoGameState(GAME_STATE_INIT);
        }
        if (itemKey[1]->GetX2() > 10814 && itemKey[1]->IsAlive())
        {
            itemKey[1]->SetIsAlive(false);
            isDoorOpen[1] = true;
            skullSoul->SetIsAlive(true);
            CAudio::Instance()->Play(AUDIO_key);
            CAudio::Instance()->Stop(AUDIO_map3);
            CAudio::Instance()->Play(AUDIO_horror,true);
            map->ChangeMapValue(8, 169, 0);
            map->ChangeMapValue(9, 169, 0);
        }
        if (hero.GetX2() > 2760 && hero.GetX2() < 2815 && hero.GetY1() > 128 && hero.GetY1() < 192)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 31;
            hero.SetXY(480, 750);
            map->SetXY(80, 0);
        }
        if (hero.GetX2() > 5000 && hero.GetX2() < 5050 && hero.GetY1() > 64 && hero.GetY1() < 128)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 32;
            hero.SetXY(480, 750);
            map->SetXY(80, 0);
        }
        if (hero.GetX2() > 7040 && hero.GetX2() < 7100 && hero.GetY1() > 64 && hero.GetY1() < 128)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 33;
            hero.SetXY(480, 750);
            map->SetXY(80, 0);
        }
        if (hero.GetX2() > 8770 && hero.GetX2() < 8830 && hero.GetY1() > 128 && hero.GetY1() < 192)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 34;
            hero.SetXY(480, 750);
            map->SetXY(80, 0);
        }
        if (hero.GetX2() > 10685 && hero.GetX2() < 10750 && hero.GetY1() > 320 && hero.GetY1() < 384)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 35;
            hero.SetXY(480, 750);
            map->SetXY(80, 0);
        }
    }
    else if (MapLevel == 31)
    {
        for (int i = 0; i < 4; i++) {
            movingBlock[i]->OnMove();
        }
        if (hero.GetY1()<1)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 30;
            hero.SetXY(3400, 500);
            map->SetXY(2900, 50);
        }
        if (map->GetMapValue(2, 2) == 3 && map->GetMapValue(2, 5) == 3 &&
                map->GetMapValue(2, 10) == 3 && map->GetMapValue(2, 13) == 3)
        {
            starDoor[2]->SetIsAlive(false);
            map->ChangeMapValue(3, 7, 0);
            map->ChangeMapValue(3, 8, 0) ;
        }
    }
    else if (MapLevel == 32)
    {
        for (int i = 4; i < 10; i++) {
            movingBlock[i]->OnMove();
        }
        if (hero.GetY1() < 1)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 30;
            hero.SetXY(5250, 300);
            map->SetXY(4850, 50);
        }
        if (map->GetMapValue(2, 3) == 3 && map->GetMapValue(2, 4) == 3 && map->GetMapValue(2, 11) == 3 &&
                map->GetMapValue(2, 12) == 3 && map->GetMapValue(7, 7) == 3 && map->GetMapValue(7, 8) == 3)
        {
            starDoor[3]->SetIsAlive(false);
            map->ChangeMapValue(1, 7, 0);
            map->ChangeMapValue(1, 8, 0);
        }
    }
    else if (MapLevel == 33)
    {
        for (int i = 10; i < 14; i++) {
            movingBlock[i]->OnMove();
        }
        if (hero.GetY1() < 1)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 30;
            hero.SetXY(7300, 500);
            map->SetXY(6900, 50);
        }
        if (map->GetMapValue(3, 3) == 3 && map->GetMapValue(3, 6) == 3 &&
                map->GetMapValue(3, 9) == 3 && map->GetMapValue(3, 12) == 3)
        {
            starDoor[4]->SetIsAlive(false);
            map->ChangeMapValue(2, 7, 0);
            map->ChangeMapValue(2, 8, 0);
        }
    }
    else if (MapLevel == 34)
    {
        for (int i = 14; i < 18; i++) {
            movingBlock[i]->OnMove();
        }
        if (hero.GetY1() < 1)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 30;
            hero.SetXY(9050, 400);
            map->SetXY(8650, 50);
        }
        if (map->GetMapValue(1, 1) == 3 && map->GetMapValue(1, 14) == 3 &&
                map->GetMapValue(12, 1) == 3 && map->GetMapValue(12, 14) == 3)
        {
            starDoor[5]->SetIsAlive(false);
            map->ChangeMapValue(1, 7, 0);
            map->ChangeMapValue(1, 8, 0);
        }
    }
    else if (MapLevel == 35)
    {
        for (int i = 18; i < 23; i++) {
            movingBlock[i]->OnMove();
        }
        if (hero.GetY1() < 1)
        {
            MapChangingEffect();
            MapLevel = map->MapLevel = 30;
            hero.SetXY(10700, 550);
            map->SetXY(10300, 50);
        }
        if (map->GetMapValue(1, 2) == 3 && map->GetMapValue(1, 13) == 3)
        {
            starDoor[6]->SetIsAlive(false);
            map->ChangeMapValue(1, 7, 0);
            map->ChangeMapValue(1, 8, 0);
            itemKey[1]->SetIsAlive(true);
        }
    }
    if (MapLevel >= 31 && MapLevel <= 35)			//在解謎地圖上重置方塊位置
    {
        if (heroBullet->GetX2(&hero) > 450 && heroBullet->GetX1() < 575 && heroBullet->GetY2(&hero) > 835)
        {
            ResetMovingBlock();
            map->InitializeMovingBlockMap();
            for (int i = 0; i < 7; i++) {
                starDoor[i]->SetIsAlive(true);
            }
        }
    }
    if (MapLevel >= 11 && MapLevel <= 15)			//在橫向卷軸的地圖掉落會死亡
    {
        if (hero.GetY2() > 680 && hero.GetLifeAmount()<=6) {
            hero.ReduceBlood(6);
        }
    }
    if (isPlayEnding)
    {
        allWalking.SetDelayCount(9);
        allWalking.OnMove();
        endingMoveX += 4;
        firework.SetDelayCount(2);
        firework.OnMove();
    }
}

void CGameStateRun::OnShow()
{
    //
    //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
    //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
    //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
    //
    //
    //
    map->OnShow();
///////////////////////////////////////////////////////第一個關卡的code//////////////////////////////////////////////////////////////////////////
    if (MapLevel == 10)
    {
        for (int i = 0; i < 4; i++) {
            item_1[i]->OnShow();
        }
        item_1[15]->OnShow();
        if (isDoorOpen[0])
        {
            door.SetTopLeft(map->ScreenX(130), map->ScreenY(65));
            door.ShowBitmap();
        }
        for (int i = 0; i < 2; i++) {
            starDoor[i]->OnShow();
        }
        itemKey[0]->OnShow();
        for (int i = 0; i < 2; i++) {
            heroWeapon[i]->OnShow();
        }
        bossMan->OnShow();
        castle->OnShow();
        for (int i = 6; i <= 8; i++) {
            item_1[i]->OnShow();
        }
        for (int i = 0; i < enemyNumber_1-2; i++)
        {
            if (i >= 3 && i <= 5) {
                continue;
            }
            enemies_1[i]->OnShow();
        }
        for (int i = 0; i < 2; i++) {
            itemLetter[i]->OnShow();
        }
        if(!bossMan->IsAlive())
        {
            nobi.SetTopLeft(map->ScreenX(220), map->ScreenY(100));
            nobi.OnShow();
        }
    }
///////////////////////////////////////////////////////第一個關卡的code//////////////////////////////////////////////////////////////////////////
    else if (MapLevel>=11 && MapLevel <= 15)
    {
        if(MapLevel == 11)
        {
            for (int i = 13; i < 17; i++) {
                fireBall[i]->OnShow();
            }
            enemies_1[3]->OnShow();
            item_1[11]->OnShow();
            item_1[4]->OnShow();
        }
        if (MapLevel == 12)
        {
            enemies_1[4]->OnShow();
            item_1[13]->OnShow();
            item_1[10]->OnShow();
            enemies_1[31]->OnShow();
        }
        if (MapLevel == 13)
        {
            weaponFire[0]->OnShow();
            enemies_1[5]->OnShow();
            item_1[5]->OnShow();
        }
        if (MapLevel == 14)
        {
            for (int i = 0; i < 8; i++) {
                fireBall[i]->OnShow();
            }
            item_1[9]->OnShow();
        }
        if (MapLevel == 15)
        {
            for (int i = 8; i < 13; i++) {
                fireBall[i]->OnShow();
            }
            item_1[12]->OnShow();
            item_1[14]->OnShow();
            enemies_1[30]->OnShow();
        }
        heroBullet[0].OnShow(map, &hero);
        hero.OnShow(map, heroBullet);
    }
///////////////////////////////////////////////////////第三個關卡的code//////////////////////////////////////////////////////////////////////////
    else if (MapLevel >=30 && MapLevel <=35)
    {
        if (MapLevel == 30)
        {
            if (!isDoorOpen[1])
            {
                door2.SetTopLeft(map->ScreenX(10816), map->ScreenY(512));
                door2.ShowBitmap();
            }
            if(skullSoul->GetBloodAmount()==0)
            {
                door.SetTopLeft(map->ScreenX(12742), map->ScreenY(393));
                door.ShowBitmap();
            }
            for(int i=0; i<enemyNumber_3; i++) {
                enemies_3[i]->OnShow();
            }
            skullSoul->OnShow();
            itemKey[1]->OnShow();
        }
        else if (MapLevel == 31)
        {
            for (int i = 0; i < 4; i++) {
                movingBlock[i]->OnShow();
            }
            starDoor[2]->OnShow();
        }
        else if (MapLevel == 32)
        {
            for (int i = 4; i < 10; i++) {
                movingBlock[i]->OnShow();
            }
            starDoor[3]->OnShow();
        }
        else if (MapLevel == 33)
        {
            for (int i = 10; i < 14; i++) {
                movingBlock[i]->OnShow();
            }
            starDoor[4]->OnShow();
        }
        else if (MapLevel == 34)
        {
            for (int i = 14; i < 18; i++) {
                movingBlock[i]->OnShow();
            }
            starDoor[5]->OnShow();
        }
        else if (MapLevel == 35)
        {
            for (int i = 18; i < 23; i++) {
                movingBlock[i]->OnShow();
            }
            starDoor[6]->OnShow();
        }
    }
///////////////////////////////////////////////////////第三個關卡的code//////////////////////////////////////////////////////////////////////////
    heroBullet[0].OnShow(map, &hero);
    hero.OnShow(map, heroBullet);
    if (dialogue_counter > 0 && dialogue_counter < 150) {
        dialogueText[0].SetTopLeft(0, 0);
        dialogueText[0].ShowBitmap();
    }
    if (dialogue_counter > 150 && dialogue_counter < 300) {
        dialogueText[1].SetTopLeft(0, 0);
        dialogueText[1].ShowBitmap();
    }
    if (dialogue_counter > 450 && dialogue_counter < 600) {
        dialogueText[3].SetTopLeft(0, 0);
        dialogueText[3].ShowBitmap();
    }
    if (dialogue_counter > 600 && dialogue_counter < 750) {
        dialogueText[4].SetTopLeft(0, 0);
        dialogueText[4].ShowBitmap();
    }
    if (dialogue_counter > 850 && dialogue_counter < 1000) {
        dialogueText[5].SetTopLeft(0, 0);
        dialogueText[5].ShowBitmap();
    }
    if (dialogue_counter > 1000 && dialogue_counter < 1150) {
        dialogueText[6].SetTopLeft(0, 0);
        dialogueText[6].ShowBitmap();
    }
    if (dialogue_counter > 1150 && dialogue_counter < 1300) {
        dialogueText[7].SetTopLeft(0, 0);
        dialogueText[7].ShowBitmap();
    }
    if (dialogue_counter > 1300 && dialogue_counter < 1450) {
        dialogueText[8].SetTopLeft(0, 0);
        dialogueText[8].ShowBitmap();
    }
    if (dialogue_counter > 1450 && dialogue_counter < 1600) {
        dialogueText[9].SetTopLeft(0, 0);
        dialogueText[9].ShowBitmap();
    }
    if (dialogue_counter>300 && dialogue_counter<450) {
        dialogueText[2].SetTopLeft(0, 0);    //清空小怪后的提示畫面
        dialogueText[2].ShowBitmap();
    }
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    pDC->SetBkMode(TRANSPARENT);		// 設定顯示文字為透明背景
    f.CreatePointFont(140, "Times New Roman");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(0, 0, 0));
    char str[80], enemyNumber[20];
    int showEnemyCount = enemyCount;
    if (enemyCount < 0) {
        showEnemyCount = 0;
    }
    sprintf(str, "Score: %d\n",score);
    sprintf(enemyNumber, "Enemies:%d", showEnemyCount);
    pDC->TextOut(350, 10, str);
    pDC->TextOut(670, 10, enemyNumber);
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();				// 放掉 Back Plain 的 CDC
    if (isPlayEnding)
    {
        endingBackground.SetTopLeft(endingMoveX, 0);
        endingBackground.ShowBitmap();
        allWalking.SetTopLeft(0, -60);
        allWalking.OnShow();
        firework.SetTopLeft(200, 0);
        firework.OnShow();
    }
    if (dialogue_counter > 2850 && dialogue_counter <= 2960) {
        end.SetTopLeft(0, 0);
        end.ShowBitmap();
    }
    if (isShowBlack) {
        black.SetTopLeft(0, 0);
        black.ShowBitmap();
    }
}



void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT  = 0x25; // keyboard左箭頭
    const char KEY_UP    = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN  = 0x28; // keyboard下箭頭
    const char KEY_SPACE = 0x20;
    const char KEY_TAB = 9;
    if (nChar == KEY_LEFT || nChar == 65)
    {
        hero.SetMovingLeft(true);
        hero.SetMovingRight(false);
        hero.SetMovingUp(false);
        hero.SetMovingDown(false);
        hero.SetPressedLeft(false);
        hero.SetPressedRight(false);
        hero.SetPressedUp(false);
        hero.SetPressedDown(false);
    }
    if (nChar == KEY_RIGHT || nChar == 68)
    {
        hero.SetMovingRight(true);
        hero.SetMovingLeft(false);
        hero.SetMovingUp(false);
        hero.SetMovingDown(false);
        hero.SetPressedLeft(false);
        hero.SetPressedRight(false);
        hero.SetPressedUp(false);
        hero.SetPressedDown(false);
    }
    if (nChar == KEY_UP || nChar == 87)
    {
        if (hero.Dead()) {
            return;
        }
        if (hero.isJumpMode)
        {
            if (hero.isReadyJump) {
                hero.SetJumping(true);
                CAudio::Instance()->Play(AUDIO_jump);
                hero.isReadyJump = false;
            }
        }
        else
        {
            hero.SetMovingUp(true);
            hero.SetMovingLeft(false);
            hero.SetMovingRight(false);
            hero.SetMovingDown(false);
            hero.SetPressedLeft(false);
            hero.SetPressedRight(false);
            hero.SetPressedUp(false);
            hero.SetPressedDown(false);
        }
    }
    if (nChar == KEY_DOWN || nChar == 83)
    {
        if (!hero.isJumpMode)
        {
            hero.SetMovingDown(true);
            hero.SetMovingLeft(false);
            hero.SetMovingRight(false);
            hero.SetMovingUp(false);
            hero.SetPressedLeft(false);
            hero.SetPressedRight(false);
            hero.SetPressedUp(false);
            hero.SetPressedDown(false);
        }
    }
    if (nChar == KEY_SPACE && isPressSpace==false)
    {
        isPressSpace = true;
        hero.SetPressedSpace(true);
        if (!hero.Dead() && !heroBullet[0].IsAlive())
        {
            heroBullet->OnKeyDown(&hero);
            if(hero.GetActivedBullet()) {
                CAudio::Instance()->Play(AUDIO_shoot5);
            }
            else if(hero.GetActivedBlade()) {
                CAudio::Instance()->Play(AUDIO_shoot2);
            }
            else if(hero.GetActivedMagic()) {
                CAudio::Instance()->Play(AUDIO_shoot3);
            }
            hero.SetShootAnimation(true);
        }
    }
    if (hero.GetMagic())
    {
        if (nChar == KEY_TAB && !heroBullet->IsAlive() && !hero.Dead() && !hero.IsTransAnimation())
        {
            hero.ChangeWeapon();    //切換武器
            CAudio::Instance()->Play(AUDIO_pick02);
        }
    }
    if (nChar == 112) {
        hero.AddBlood(999);    //英雄開啟無敵
    }
    if (nChar == 46)		//殺死全部小怪
    {
        if (MapLevel >= 10 && MapLevel <= 15)
        {
            for (int i = 0; i < enemyNumber_1 - 2; i++) {
                enemies_1[i]->SetIsAlive(false);
            }
            if (enemyCount > 0)
            {
                dialogue_counter = 300;
                isTalking = true;
            }
        }
        if (MapLevel >= 30 && MapLevel <= 35)
        {
            for (int i = 0; i < enemyNumber_1 - 2; i++) {
                enemies_3[i]->SetIsAlive(false);
            }
        }
        enemyCount = 0;
        bossMan->ReduceBlood(bossMan->GetBloodAmount()-4);
        skullSoul->ReduceBlood(skullSoul->GetBloodAmount()-5);
    }
    if (nChar >= 96 && nChar <=100)		// 第一關的按鍵
    {
        CAudio::Instance()->Play(AUDIO_map1, true);
        CAudio::Instance()->Stop(AUDIO_map1_);
        CAudio::Instance()->Stop(AUDIO_map3);
        CAudio::Instance()->Stop(AUDIO_map3_);
        map->MapLevel = MapLevel = 10;
        hero.ChangeToNormalMode();
        isShowBlack = true;
        switch (nChar)
        {
        case 96:
            hero.SetXY(2725, 2380);
            map->SetXY(2325, 1960);
            break;	// 到第一關初始位置
        case 97:
            hero.SetXY(300, 1015);
            map->SetXY(0, 715);
            break;	// 撿武器 跟 撿信封 的地方
        case 98:
            hero.SetXY(700, 1050);
            map->SetXY(300, 750);
            break;	// 撿鑰匙的地方
        case 99:
            hero.SetXY(2020, 300);
            map->SetXY(1620, 0);
            break;	// 城堡附近
        }
    }
    if(nChar >= 100 && nChar <=102)		// 第二關的按鍵
    {
        CAudio::Instance()->Play(AUDIO_map3, true);
        CAudio::Instance()->Stop(AUDIO_map3_);
        CAudio::Instance()->Stop(AUDIO_map1);
        CAudio::Instance()->Stop(AUDIO_map1_);
        map->MapLevel = MapLevel = 30;
        dialogue_counter = 1450;
        hero.ChangeToJumpMode();
        isShowBlack = true;
        switch (nChar)
        {
        case 100:
            hero.SetXY(300, 350);
            map->SetXY(64, 50);
            break;	// 到第二關初始位置
        case 101:
            hero.SetXY(5450, 430);
            map->SetXY(5050, 50);
            break;	// 到第二關地圖中段位置
        case 102:
            hero.SetXY(9300, 300);
            map->SetXY(8900, 50);
            break;	// 到第二關地圖後段位置
        }
    }
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT  = 0x25; // keyboard左箭頭
    const char KEY_UP    = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN  = 0x28; // keyboard下箭頭
    const char KEY_SPACE = 0x20;
    if (nChar == KEY_LEFT || nChar == 65)
    {
        hero.SetPressedLeft(true);
        hero.SetMovingLeft(false);
        hero.SetPressedRight(false);
        hero.SetPressedUp(false);
        hero.SetPressedDown(false);
    }
    if (nChar == KEY_RIGHT || nChar == 68)
    {
        hero.SetPressedRight(true);
        hero.SetMovingRight(false);
        hero.SetPressedLeft(false);
        hero.SetPressedUp(false);
        hero.SetPressedDown(false);
    }
    if (nChar == KEY_UP || nChar == 87)
    {
        if (hero.isJumpMode) {}
        else
        {
            hero.SetPressedUp(true);
            hero.SetPressedLeft(false);
            hero.SetPressedRight(false);
            hero.SetMovingUp(false);
            hero.SetPressedDown(false);
        }
    }
    if (nChar == KEY_DOWN || nChar == 83)
    {
        if (hero.isJumpMode) {}
        else
        {
            hero.SetPressedDown(true);
            hero.SetPressedLeft(false);
            hero.SetPressedRight(false);
            hero.SetMovingDown(false);
            hero.SetPressedUp(false);
        }
    }
    if (nChar == KEY_SPACE)
    {
        hero.SetPressedSpace(false);
        hero.SetShootAnimation(false);
    }
}

Map::Map(Hero* ptrhero):hero(ptrhero),mW(3200),mH(2560)	//膘?赽
{
    Initialize();
}

void Map::Initialize()	//場宎趙華
{
    int map_init[40][50] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1},
        {1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,0,0,1,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,1,1,1,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    int map3_init[11][200] = { {1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,0,0,1,1,1,0,1,1,1,0,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,2,2,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,1,2,2,1,2,2,1,1,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,2,1,1,1,2,1,1,1,2,1,1,2,2,1,1,1,2,1,1,1,2,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}
    };
    int map__init[4][11][26] = { { {1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0},
            {1,2,0,0,0,0,2,1,0,0,1,0,2,0,0,0,0,0,0,0,0,0,0,0,1,0},
            {1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
            {1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0},
            {1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0},
            {1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0},
            {1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0},
            {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
            {1,1,1,0,0,1,1,1,1,1,1,0,1,0,1,0,0,0,0,0,1,1,1,1,1,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        },

        {   {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1},
            {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
            {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
        },

        {   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
            {1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
            {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        },

        {   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
            {0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0},
            {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0},
            {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0},
            {1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0},
            {1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0},
            {1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0},
            {1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        }
    };
    int map_castle_init[11][51] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {1,2,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,2,0,0,0,2,0,0,2,0,0,2,0,0,0,0},
        {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    for (int i = 0; i < 40; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            map[i][j] = map_init[i][j];
        }
    }
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            map3[i][j] = map3_init[i][j];
        }
    }
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 51; j++)
        {
            map_castle[i][j] = map_castle_init[i][j];
        }
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            for (int k = 0; k < 26; k++)
            {
                map_[i][j][k] = map__init[i][j][k];
            }
        }
    }
    InitializeMovingBlockMap();
    sx = mW - 800;
    sy = mH - 600;
    MapLevel = 10;
    for (int i = 0; i < 10; i++) {
        v_background.push_back(background);
    }
}

void Map::InitializeMovingBlockMap()	//場宎趙痄源腔華?蹈
{
    int map3__init[5][14][16] = { { {1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1},
            {1,1,0,0,0,0,1,0,0,1,0,0,0,0,1,1},
            {1,1,0,1,1,0,1,1,1,1,0,0,0,0,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,1,0,0,1,0,0,0,3,0,1},
            {1,0,0,1,0,0,3,0,0,3,0,0,0,1,0,1},
            {1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
            {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        },

        {   {1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
            {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,1,0,0,1,0,3,0,1,0,1},
            {1,0,0,0,0,0,3,0,3,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,1,0,0,1,3,0,0,0,0,1},
            {1,0,0,0,3,0,1,1,1,1,0,0,0,0,0,1},
            {1,0,1,0,3,0,0,0,0,0,0,0,0,1,0,1},
            {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        },

        {   {1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1},
            {1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1},
            {1,0,0,1,0,0,1,1,1,1,1,0,1,0,0,1},
            {1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
            {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
            {1,0,0,0,0,1,3,0,0,0,1,0,0,0,0,1},
            {1,0,3,0,0,3,0,0,0,0,0,0,1,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        },

        {   {1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,1,1,1,1,3,3,0,1,0,0,0,0,1},
            {1,0,0,0,3,0,0,0,0,0,1,0,0,0,1,1},
            {1,0,0,1,1,1,1,0,0,0,1,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
            {1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
            {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,1,0,0,0,3,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        },

        {   {1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1},
            {1,1,0,1,0,0,1,1,1,1,0,0,1,0,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,1},
            {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,3,1,1,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,1},
            {1,0,0,0,0,0,3,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,3,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        }
    };
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 14; j++)
        {
            for (int k = 0; k < 16; k++)
            {
                map3_[i][j][k] = map3__init[i][j][k];
            }
        }
    }
}

void Map::LoadBitmap()		//徭倛
{
    background.LoadBitmap(".\\bitmaps\\map.bmp");
    background3.LoadBitmap(".\\bitmaps\\map3.bmp");
    v_background[0].LoadBitmap(".\\bitmaps\\map1-1.bmp");
    v_background[1].LoadBitmap(".\\bitmaps\\map1-2.bmp");
    v_background[2].LoadBitmap(".\\bitmaps\\map1-3.bmp");
    v_background[3].LoadBitmap(".\\bitmaps\\map1-4.bmp");
    v_background[4].LoadBitmap(".\\bitmaps\\map1-5.bmp");
    v_background[5].LoadBitmap(".\\bitmaps\\map3-1.bmp");
    v_background[6].LoadBitmap(".\\bitmaps\\map3-2.bmp");
    v_background[7].LoadBitmap(".\\bitmaps\\map3-3.bmp");
    v_background[8].LoadBitmap(".\\bitmaps\\map3-4.bmp");
    v_background[9].LoadBitmap(".\\bitmaps\\map3-5.bmp");
}

void Map::OnShow()		//鞞尨倛
{
    switch (MapLevel)
    {
    case 10:
        background.SetTopLeft(-sx, -sy);
        background.ShowBitmap();
        break;
    case 11:
        v_background[0].SetTopLeft(-sx, -sy);
        v_background[0].ShowBitmap();
        break;
    case 12:
        v_background[1].SetTopLeft(-sx, -sy);
        v_background[1].ShowBitmap();
        break;
    case 13:
        v_background[2].SetTopLeft(-sx, -sy);
        v_background[2].ShowBitmap();
        break;
    case 14:
        v_background[3].SetTopLeft(-sx, -sy);
        v_background[3].ShowBitmap();
        break;
    case 15:
        v_background[4].SetTopLeft(-sx, -sy);
        v_background[4].ShowBitmap();
        break;
    case 30:
        background3.SetTopLeft(-sx, -sy);
        background3.ShowBitmap();
        break;
    case 31:
        v_background[5].SetTopLeft(-sx, -sy);
        v_background[5].ShowBitmap();
        break;
    case 32:
        v_background[6].SetTopLeft(-sx, -sy);
        v_background[6].ShowBitmap();
        break;
    case 33:
        v_background[7].SetTopLeft(-sx, -sy);
        v_background[7].ShowBitmap();
        break;
    case 34:
        v_background[8].SetTopLeft(-sx, -sy);
        v_background[8].ShowBitmap();
        break;
    case 35:
        v_background[9].SetTopLeft(-sx, -sy);
        v_background[9].ShowBitmap();
        break;
    }
}

void Map::OnMove()	//華腔痄摯髡夔
{
    int STEP_SIZE = hero->GetStepSize();
    if (MapLevel==30 || MapLevel>=11 && MapLevel<=15) {
        if (MapLevel >= 13 && MapLevel <= 15)
        {
            if (ScreenX(hero->GetX1()) < 400 && sx > 0)
                sx -= STEP_SIZE;
        }
        else
        {
            if (ScreenX(hero->GetX1()) < 400 && sx > 64)
                sx -= STEP_SIZE;
        }
        if (MapLevel == 30)
        {
            if (ScreenX(hero->GetX1()) > 400 && sx < 12000)
                sx += STEP_SIZE;
        }
        else if (MapLevel == 15)
        {
            if (ScreenX(hero->GetX1()) > 400 && sx < 2400)
                sx += STEP_SIZE;
        }
        else
        {
            if (ScreenX(hero->GetX1()) > 400 && sx < 800)
                sx += STEP_SIZE;
        }
    }
    else {
        int largestWidth = 2400;
        int largestHeight = 1960;
        if (MapLevel >= 31 && MapLevel <= 35)
        {
            largestWidth = 224;
            largestHeight = 296;
        }
        if (ScreenX(hero->GetX1()) < 400 && sx > 0)
            sx -= STEP_SIZE;
        if (ScreenX(hero->GetX1()) > 400 && sx < largestWidth)
            sx += STEP_SIZE;
        if (ScreenY(hero->GetY1()) < 300 && sy > 0)
            sy -= STEP_SIZE;
        if (ScreenY(hero->GetY1()) > 300 && sy < largestHeight)
            sy += STEP_SIZE;
    }
    if (!hero->GetInvincible() && !hero->GetGuard())
    {
        if (isTrap(hero->GetX1(),hero->GetY1()) || isTrap(hero->GetX1(), hero->GetY2())
                || isTrap(hero->GetX2(), hero->GetY1()) || isTrap(hero->GetX2(), hero->GetY2()))
        {
            hero->ExecuteInjureEffect();
        }
    }
}


void Map::SetXY(int X, int Y)	//偞隅XY釴
{
    sx = X;
    sy = Y;
}

int Map::GetX()	//腕酘奻褒X釴
{
    return sx;
}

int Map::GetY()	//腕酘奻褒Y釴
{
    return sy;
}

int Map::ScreenX(int x)	//腕躉X釴
{
    return x-sx;
}

int Map::ScreenY(int y)	//腕躉Y釴
{
    return y-sy;
}

bool Map::isEmpty(int x, int y)	//瓚岆瘁諾
{
    int xx, yy;
    xx = x / 64;
    yy = y / 64;
    switch (MapLevel)
    {
    case 10:
        return (map[yy][xx] == 0);
        break;
    case 11:
        return (map_[0][yy][xx] == 0 || map_[0][yy][xx] == 2);
        break;
    case 12:
        return (map_[1][yy][xx] == 0);
        break;
    case 13:
        return (map_[2][yy][xx] == 0);
        break;
    case 14:
        return (map_[3][yy][xx] == 0 || map_[3][yy][xx] == 2);
        break;
    case 15:
        return (map_castle [yy][xx] == 0 || map_castle[yy][xx] == 2);
        break;
    case 30:
        return (map3[yy][xx] == 0 || map3[yy][xx] == 2);
        break;
    case 31:
        return (map3_[0][yy][xx] == 0 || map3_[0][yy][xx] == 3);
        break;
    case 32:
        return (map3_[1][yy][xx] == 0 || map3_[1][yy][xx] == 3);
        break;
    case 33:
        return (map3_[2][yy][xx] == 0 || map3_[2][yy][xx] == 3);
        break;
    case 34:
        return (map3_[3][yy][xx] == 0 || map3_[3][yy][xx] == 3);
        break;
    case 35:
        return (map3_[4][yy][xx] == 0 || map3_[4][yy][xx] == 3);
        break;
    }
    return true;
}

bool Map::isTrap(int x, int y)	//瓚岆瘁疪筘
{
    int xx, yy;
    xx = x / 64;
    yy = y / 64;
    switch (MapLevel)
    {
    case 30:
        return (map3[yy][xx] == 2);
        break;
    case 11:
        return (map_[0][yy][xx] == 2);
        break;
    case 14:
        return (map_[3][yy][xx] == 2);
        break;
    case 15:
        return (map_castle[yy][xx] == 2);
        break;
    default:
        return false;
    }
}

bool Map::isNotBlock(int x, int y)	//瓚岆瘁源
{
    int xx, yy;
    xx = x / 64;
    yy = y / 64;
    switch (MapLevel)
    {
    case 31:
        return !(map3_[0][yy][xx] == 3);
        break;
    case 32:
        return !(map3_[1][yy][xx] == 3);
        break;
    case 33:
        return !(map3_[2][yy][xx] == 3);
        break;
    case 34:
        return !(map3_[3][yy][xx] == 3);
        break;
    case 35:
        return !(map3_[4][yy][xx] == 3);
        break;
    }
    return true;
}

void Map::ChangeMapValue(int i, int j, int value)	//蜊?華?蹈腔硉
{
    switch (MapLevel)
    {
    case 10:
        map[i][j] = value;
        break;
    case 30:
        map3[i][j] = value;
        break;
    case 31:
        map3_[0][i][j] = value;
        break;
    case 32:
        map3_[1][i][j] = value;
        break;
    case 33:
        map3_[2][i][j] = value;
        break;
    case 34:
        map3_[3][i][j] = value;
        break;
    case 35:
        map3_[4][i][j] = value;
        break;
    }
}

int Map::GetMapValue(int i, int j)	//腕華?蹈腔硉
{
    switch (MapLevel)
    {
    case 31:
        return map3_[0][i][j];
        break;
    case 32:
        return map3_[1][i][j];
        break;
    case 33:
        return map3_[2][i][j];
        break;
    case 34:
        return map3_[3][i][j];
        break;
    case 35:
        return map3_[4][i][j];
        break;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g) {}

void CGameStateOver::OnMove()
{
    counter--;
    if (counter < 0)
        GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
    CAudio::Instance()->Stop(AUDIO_map1);
    CAudio::Instance()->Stop(AUDIO_map1_);
    CAudio::Instance()->Stop(AUDIO_boss1);
    CAudio::Instance()->Play(AUDIO_OVER, false);
    counter = 30 * 9; // 4 seconds
    reset = true;
}

void CGameStateOver::OnInit()
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
    //
    // 開始載入資料
    //
    over.LoadBitmap(".\\bitmaps\\over.bmp");
    //
    // 最終進度為100%
    //
    ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
    over.SetTopLeft(0, 0);
    over.ShowBitmap();
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(200, "Times New Roman");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkMode(TRANSPARENT);		// 設定顯示文字為透明背景
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(0, 0, 0));
    char str[80];								// Demo 數字對字串的轉換
    sprintf(str, "Score: %d", score);
    pDC->TextOut(550, 520, str);
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

CGameStateRest::CGameStateRest(CGame* g):CGameState(g) {}

void CGameStateRest::OnBeginState()
{
    counter = 30 * 4; // 4 seconds
}

void CGameStateRest::OnInit()
{
    rest.LoadBitmap(".\\bitmaps\\rest.bmp");
    rest2.LoadBitmap(".\\bitmaps\\rest2.bmp");
}

void CGameStateRest::OnMove()
{
    counter--;
    if (counter < 0)
        GotoGameState(GAME_STATE_RUN);
}

void CGameStateRest::OnShow()
{
    if (MapLevel >= 10 && MapLevel <= 15)
    {
        rest.SetTopLeft(0, 0);
        rest.ShowBitmap();
    }
    else if (MapLevel >= 30 && MapLevel <=35)
    {
        rest2.SetTopLeft(0, 0);
        rest2.ShowBitmap();
    }
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(200, "Times New Roman");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkMode(TRANSPARENT);		// 設定顯示文字為透明背景
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(0, 0, 0));
    char str[80],str2[80];								// Demo 數字對字串的轉換
    sprintf(str, "Score : %d", score);
    sprintf(str2, "Left   : %d", left);
    pDC->TextOut(400, 400, str);
    pDC->TextOut(400, 450, str2);
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

}