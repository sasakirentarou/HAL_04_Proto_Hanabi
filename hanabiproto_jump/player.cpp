/*==============================================================================

   頂点管理 [polygon.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "camera.h"
#include "BENRIclass.h"
#include "object.h"
#include "inputx.h"
#include "input.h"
//#include <Xinput.h>

//#pragma comment(lib."Xinput.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_DISP_X (SCREEN_WIDTH/2)
#define PLAYER_DISP_Y (SCREEN_HEIGHT/2)
#define HISSATU_COOLTIME (280)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void pad_reset(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int g_TextureNo;
static int g_TextureNo2;

static PLAYER g_Player;

static float g_U, g_V;

CURSOR		g_cursor[PLAYER_CURSOR_NUM];	//カーソルの軌跡点
int			g_nownum = -1;//カーソルの現在個数
int			g_targetnum = -1;//攻撃先
int			g_curmax = 0;
Float2 cursorposf;
POINT cursorPos;

Float2 g_pad_curpos;
Float2 g_pad_inputprev;
bool g_cursol_prev;

int g_playerflame;
int g_Hissattu;

//ジャンプ
bool g_jflg = false;//ジャンプしてるか

//DWORD        dwUserIndex;
//XINPUT_STATE State;


//static int g_AnimeIndex;
//static int g_AnimeWait;

static unsigned int g_StartTime;
static unsigned int g_Second;
static unsigned int g_MiliSecond;

static float g_AnimeTable[4] =
{
	0.0f,
	0.333333f,
	0.666667f,
	1.0f,
};

//static float g_AnimeTable[4] =
//{
//	0.0f,
//	0.333333f,
//	0.666667f,
//	0.333333f,
//};
//
//static float g_MukiTable[4] =
//{
//	0.75f,	//上向き
//	0.5f,	//右向き
//	0.0f,	//下向き
//	0.25f,	//左向き
//};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/proto_Hanabi_character.png");
	g_TextureNo2 = LoadTexture((char*)"data/TEXTURE/proto_effect_akari.png");

	//初期化
	g_Player.pos.x = PLAYER_DISP_X;
	g_Player.pos.y = PLAYER_DISP_Y;

	g_Player.spjp.x = 8.0f;
	g_Player.spjp.y = 0.0f;

	g_Player.jp.x = -10.0f;
	g_Player.jp.y = 0.0f;
	//Xinput初期化
	//XInputEnable(true);
	//XInputGetState(0, &State);
	//State.Gamepad.
	pad_reset();
	g_cursol_prev = false;
	g_playerflame = 0;

	//g_AnimeIndex = 0;
	//g_AnimeWait = 0;
	
	g_U = 0.0f;
	g_V = 0.0f;

	//ゲーム開始時の時刻を取得する
	g_StartTime = timeGetTime();
	g_nownum = -1;
	g_Hissattu = 0;

	//カーソル初期化
	for (int i = 0; i < PLAYER_CURSOR_NUM; i++)
	{
		g_cursor[i].use = false;
		g_cursor[i].color = { 0.0f,0.0f,0.0f,0.0f };
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	//ベース座標を取得する
	D3DXVECTOR2 basePos = GetBase();
	Float2 BasePos(basePos.x, basePos.y);

	//入力処理≒＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	if (g_Hissattu <= 0)
	{
		//cursor処理＝＝＝＝＝＝＝＝＝
		if (GetKeyState(VK_LBUTTON) & 0x80)
		{
			bool flag = false;
			if (g_nownum != -1)
			{
				flag = true;
			}
			if (g_cursol_prev == false)
			{
				g_cursor[g_nownum].use = false;
			}
			g_cursol_prev = true;
			g_playerflame += 1;


			g_curmax++;
			g_nownum++;
			if (g_nownum >= PLAYER_CURSOR_NUM)
			{
				g_nownum = 0;
			}
			if (g_curmax >= PLAYER_CURSOR_NUM)
			{
				g_curmax = PLAYER_CURSOR_NUM;
			}

			GetCursorPos(&cursorPos);
			ScreenToClient(GethWnd(), &cursorPos);
			cursorposf.x = (float)cursorPos.x;
			cursorposf.y = (float)cursorPos.y;
			g_cursor[g_nownum].pos.x = (cursorposf.x);
			g_cursor[g_nownum].pos.y = (cursorposf.y);
			g_cursor[g_nownum].prev_pos.x = g_cursor[g_nownum].pos.x - 1;
			g_cursor[g_nownum].prev_pos.y = g_cursor[g_nownum].pos.y - 1;
			if (flag)
			{
				g_cursor[g_nownum].prev_pos = g_cursor[(g_nownum + PLAYER_CURSOR_NUM - 1) % PLAYER_CURSOR_NUM].pos;
			}
			g_cursor[g_nownum].use = true;
			//色づけ
			{
				float RGB[3];
				int saidai = 0;
				for (int j = 0; j < 3; j++)
				{
					RGB[j] = frand();
					if (RGB[saidai] <= RGB[j])
						saidai = j;
				}
				RGB[saidai] = 1.0f;
				g_cursor[g_nownum].color = { RGB[0],RGB[1],RGB[2],1.0f };
			}
			g_cursor[(g_nownum + PLAYER_CURSOR_NUM + 1) % PLAYER_CURSOR_NUM].use = false;

			Float2 maxvec = { 0,0 };
			float maxvecf;
			for (int i = 0; i < PLAYER_CURSOR_NUM; i++)
			{
				if (g_cursor[i].use == true)
				{
					maxvec = maxvec + Float2(fabsf(g_cursor[i].pos.x - g_cursor[i].prev_pos.x), fabsf(g_cursor[i].pos.y - g_cursor[i].prev_pos.y));
					maxvecf = maxvec.x + maxvec.y;
				}
			}

			for (int i = 1; i < PLAYER_CURSOR_NUM; i++)
			{
				int j = (i + g_nownum) % PLAYER_CURSOR_NUM;
				if (g_cursor[j].use == true)
				{
					if (maxvecf > 700)
					{
						if (HitCheckCross2nd(g_cursor[j].prev_pos, g_cursor[j].pos
							, g_cursor[g_nownum].prev_pos, g_cursor[g_nownum].pos) == true)
						{
							//ここにcross時の処理
							CompositionAkari(j, g_nownum);
							for (int i = 0; i < PLAYER_CURSOR_NUM; i++)
							{
								g_cursor[i].use = false;
							}
							g_nownum = -1;
							pad_reset();
						}
					}
					else
					{

					}
				}
			}
		}
		else if (0 < fabsf(GetThumbRightX(0) - g_pad_inputprev.x) + fabsf(GetThumbRightY(0) - g_pad_inputprev.y) || 0 < fabsf(GetThumbRightX(0)) + fabsf(GetThumbRightY(0)))
		{
			g_cursol_prev = false;

			bool flag = false;
			if (g_nownum != -1)
			{
				flag = true;
			}
			g_curmax++;
			g_nownum++;
			g_playerflame += 1;


			if (g_nownum >= PLAYER_CURSOR_NUM)
			{
				g_nownum = 0;
			}
			if (g_curmax >= PLAYER_CURSOR_NUM)
			{
				g_curmax = PLAYER_CURSOR_NUM;
			}

			g_pad_curpos.x += (GetThumbRightX(0) * 2) * (fabsf(GetThumbRightX(0)) * 7) * 2.5;
			g_pad_curpos.y += (-GetThumbRightY(0) * 2) * (fabsf(GetThumbRightY(0)) * 7) * 2.5;

			if (g_pad_curpos.x < 0)
			{
				g_pad_curpos.x = 0;
			}
			if (g_pad_curpos.x > SCREEN_WIDTH)
			{
				g_pad_curpos.x = SCREEN_WIDTH;
			}
			if (g_pad_curpos.y < 0)
			{
				g_pad_curpos.y = 0;
			}
			if (g_pad_curpos.y > SCREEN_HEIGHT)
			{
				g_pad_curpos.y = SCREEN_HEIGHT;
			}

			//重要！cursor位置＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝ー
			cursorposf.x = g_pad_curpos.x;
			cursorposf.y = g_pad_curpos.y;
			//cursorposf.x = (GetThumbRightX(0) * 0.6 + 1) * (SCREEN_WIDTH / 2);
			//cursorposf.y = SCREEN_HEIGHT - ((GetThumbRightY(0) * 0.6 + 1) * (SCREEN_HEIGHT / 2));
			//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
			g_cursor[g_nownum].pos.x = (cursorposf.x);
			g_cursor[g_nownum].pos.y = (cursorposf.y);
			g_cursor[g_nownum].prev_pos.x = g_cursor[g_nownum].pos.x - 1;
			g_cursor[g_nownum].prev_pos.y = g_cursor[g_nownum].pos.y - 1;
			if (flag)
			{
				g_cursor[g_nownum].prev_pos = g_cursor[(g_nownum + PLAYER_CURSOR_NUM - 1) % PLAYER_CURSOR_NUM].pos;
			}
			g_cursor[g_nownum].use = true;
			//色づけ
			{
				float RGB[3];
				int saidai = 0;
				for (int j = 0; j < 3; j++)
				{
					RGB[j] = frand();
					if (RGB[saidai] <= RGB[j])
						saidai = j;
				}
				RGB[saidai] = 1.0f;
				g_cursor[g_nownum].color = { RGB[0],RGB[1],RGB[2],1.0f };
			}
			g_cursor[(g_nownum + PLAYER_CURSOR_NUM + 1) % PLAYER_CURSOR_NUM].use = false;

			Float2 maxvec = { 0,0 };
			float maxvecf;
			for (int i = 0; i < PLAYER_CURSOR_NUM; i++)
			{
				if (g_cursor[i].use == true)
				{
					maxvec = maxvec + Float2(fabsf(g_cursor[i].pos.x - g_cursor[i].prev_pos.x), fabsf(g_cursor[i].pos.y - g_cursor[i].prev_pos.y));
					maxvecf = maxvec.x + maxvec.y;
				}
			}

			for (int i = 1; i < PLAYER_CURSOR_NUM; i++)
			{
				int j = (i + g_nownum) % PLAYER_CURSOR_NUM;
				if (g_cursor[j].use == true)
				{
					if (maxvecf > 500 && 0 < fabsf(GetThumbRightX(0) - g_pad_inputprev.x) + fabsf(GetThumbRightY(0) - g_pad_inputprev.y))
					{
						if (HitCheckCross2nd(g_cursor[j].prev_pos, g_cursor[j].pos
							, g_cursor[g_nownum].prev_pos, g_cursor[g_nownum].pos) == true)
						{
							//ここにcross時の処理
							CompositionAkari(j, g_nownum);
							for (int i = 0; i < PLAYER_CURSOR_NUM; i++)
							{
								g_cursor[i].use = false;
							}
							g_nownum = -1;
							pad_reset();
						}
					}
					else
					{

					}
				}
			}

			g_pad_inputprev.x = GetThumbRightX(0);
			g_pad_inputprev.y = GetThumbRightY(0);

		}
		else if (g_nownum != -1)
		{
			for (int i = 0; i < PLAYER_CURSOR_NUM; i++)
			{
				if (g_nownum == i)
				{
				}
				else
				{
					g_cursor[i].use = false;
				}
			}
			if (g_cursol_prev == true)
			{
				pad_reset();
				g_cursol_prev = false;
				g_cursor[g_nownum].use = false;
				g_playerflame = 0;
				g_targetnum = -1;
			}
			else
			{
				g_targetnum = g_nownum;
			}
			g_nownum = -1;


		}

		//==========================================

		//==========================================
		if (g_nownum != -1)
			g_targetnum = g_nownum;
		if (g_targetnum != -1)
			g_playerflame += 1;
		if (g_playerflame >= 50)
		{
			Normalizer(g_Player.pos, g_cursor[g_targetnum].pos - BasePos);
			g_playerflame = 0;
		}
		//==========================================


		//キーボードのAキーが押されたら左に移動する
		if (GetKeyboardPress(DIK_A) || GetThumbLeftX(0) < 0)
		{
			g_Player.pos.x -= g_Player.spjp.x;
			g_Player.vec.x = -2.0;
		}
		//キーボードのDキーが押されたら右に移動する
		if (GetKeyboardPress(DIK_D) || GetThumbLeftX(0) > 0)
		{
			g_Player.pos.x += g_Player.spjp.x;
			g_Player.vec.x = 2.0;
		}


		//ジャンプ処理
		//フラグがオンになった時ジャンプ処理を開始する
		if (g_jflg == true)
		{
			//Y方向の速度に加速度を加える
			g_Player.jp.y += g_Player.spjp.y;
			//Y座標の更新
			g_Player.pos.y += g_Player.jp.y;
			if (g_Player.pos.y >= PLAYER_DISP_Y) //yの360に到達したらフラグをオフにする
			{
				g_jflg = false;
				g_Player.pos.y = PLAYER_DISP_Y;
			}
		}

		//スペースが押されてる&ジャンプフラグがオフだったらジャンプする
		if (GetKeyboardTrigger(DIK_SPACE) && g_jflg == false)
		{
			g_jflg = true;
			g_Player.jp.y = -20.0f;
			g_Player.spjp.y = 0.8f;
		}

		//if (GetKeyboardPress(DIK_W))
		//{
		//	g_Player.pos.y -= 2.0f;
		//}
		//if (GetKeyboardPress(DIK_S))
		//{
		//	g_Player.pos.y += 2.0f;
		//}
	}
	else if (g_Hissattu > 0)
	{
		if (g_Hissattu > HISSATU_COOLTIME)
		{
			Float2 hissatuwaza(frand() * SCREEN_WIDTH, frand() * SCREEN_HEIGHT);
			Normalizer(g_Player.pos, hissatuwaza - BasePos);
		}
		g_Hissattu--;
	}
	//入力処理≒＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝


	//カメラ座標の更新
	//プレイヤー座標から表示座標分を引いた値をカメラ座標としてセットする
	D3DXVECTOR2 cameraPos;
	cameraPos.x = g_Player.pos.x - PLAYER_DISP_X;
	cameraPos.y = g_Player.pos.y - PLAYER_DISP_Y;
	if (cameraPos.x < 0) cameraPos.x = 0.0f;
	if (cameraPos.y < 0) cameraPos.y = 0.0f;
	if (cameraPos.x > 960) cameraPos.x = 960.0f;
	if (cameraPos.y > 540) cameraPos.y = 540.0f;
	SetCameraPos(cameraPos.x, cameraPos.y);



	unsigned int nowTime = timeGetTime();

	//	SetNumber(30000-(nowTime - g_StartTime));
	g_Second = (nowTime - g_StartTime) / 1000;
	g_MiliSecond = ((nowTime - g_StartTime) % 1000) / 10;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	//ベース座標を取得する
	D3DXVECTOR2 basePos = GetBase();

	DrawSprite(g_TextureNo, basePos.x + g_Player.pos.x, basePos.y + g_Player.pos.y, 
		120.0f, 120.0f,
		0.33333, 0.0, 
		0.33333f, 1.0f);

	for (int i = 0; i < PLAYER_CURSOR_NUM; i++)
	{
		if (g_cursor[i].use == true)
		{

			DrawSpriteColor(g_TextureNo2,g_cursor[i].pos.x,g_cursor[i].pos.y,
				40.0f, 40.0f,
				1.0f, 0.0,
				1.0f, 1.0f, {1.0f,0.6f,0.9f,1.0f});

			//DrawSprite(g_TextureNo, basePos.x + g_cursor[i].pos.x, basePos.y + g_cursor[i].pos.y,
			//	120.0f, 120.0f,
			//	0.33333, 0.0,
			//	0.33333f, 1.0f);
		}
	}

}

bool CompositionAkari(int clossStart,int clossGoal)
{
	bool useflag = false;
	int length = Float2_length_gather(clossStart, clossGoal);


	for (int h = 0; h < AKARI_NUM; h++)
	{
		useflag = false;
		if (GetAkariObject(h).use == true)
			useflag = true;
		Float2 apos = GetAkariObject(h).pos;

		if(HitCheckConcavePolygon(g_cursor, apos, clossStart, length) == true)
		{
			Akarigather(h);
		}
	}
	

	return true;
}

PLAYER* GetPlayer(void)
{
	return &g_Player;
}


void pad_reset(void)
{
	g_pad_curpos.x = (SCREEN_WIDTH / 4) * 3;
	g_pad_curpos.y = SCREEN_HEIGHT / 2;

	g_pad_inputprev.x = 0;
	g_pad_inputprev.y = 0;
}

void plus_hissatuwaza(int index)
{
	g_Hissattu += index;
	if (g_Hissattu > HISSATU_COOLTIME + 200)
		g_Hissattu = HISSATU_COOLTIME + 200;
	else if(g_Hissattu < HISSATU_COOLTIME + index)
	{
		g_Hissattu = HISSATU_COOLTIME + index;
	}
}


//bool CompositionAkari(int clossStart)
//{
//	bool flag = true;
//	bool useflag = false;
//
//
//	for (int h = 0; h < AKARI_NUM; h++)
//	{
//		useflag = false;
//		if (GetAkariObject(h).use == true)
//			useflag = true;
//
//		for (int i = clossStart; i < clossStart + g_curmax - 1 && useflag == true; i++)
//		{
//			int j = i % PLAYER_CURSOR_NUM;
//			//Float2 dis1 = g_cursor[j].pos - g_cursor[j].prev_pos;
//			//Float2 dis2 = g_cursor[(j + 1) % PLAYER_CURSOR_NUM].prev_pos - g_cursor[(j + 1) % PLAYER_CURSOR_NUM].pos;
//
//			//D3DXVec2Normalize(&dis1, &dis1);
//			//D3DXVec2Normalize(&dis2, &dis2);
//			//Float2 dis3 = (dis1 + dis2);
//			//D3DXVec2Normalize(&dis3, &dis3);
//
//
//
//			Float2 top(0, 0);
//			Float2 under(0, 0);
//			Float2 mid(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
//			under = g_cursor[clossStart].pos;
//			for (int f = clossStart + 1; f < clossStart + g_curmax - 1; f++)
//			{
//				int g = f % PLAYER_CURSOR_NUM;
//				if (top.y < g_cursor[f].pos.y)
//				{
//					top.y = g_cursor[f].pos.y;
//				}
//				if (top.x < g_cursor[f].pos.x)
//				{
//					top.x = g_cursor[f].pos.x;
//				}
//				if (under.y > g_cursor[f].pos.y)
//				{
//					under.y = g_cursor[f].pos.y;
//				}
//				if (under.x > g_cursor[f].pos.x)
//				{
//					under.x = g_cursor[f].pos.x;
//				}
//
//			}
//			mid = (top + under) / 2;
//			Float2 dis3 = mid - g_cursor[j].pos;
//			//D3DXVec2Normalize(&dis3, &dis3);
//
//			float biggestnow = 0;
//			float biggestnow2 = 0;
//			Float2 biggest1 = { 0,0 };
//			Float2 biggest2 = { 0,0 };
//
//			float sikai = 0;
//			for (int k = clossStart + 1; k < clossStart + g_curmax; k++)
//			{
//				int l = k % PLAYER_CURSOR_NUM;
//				Float2 ldir = g_cursor[l].pos - g_cursor[j].pos;
//
//				//if (k < clossStart + (g_curmax / 2))
//				//{
//				//	//自分（敵）の向きとプレイヤーとの位置関係のベクトルとで内積を計算する
//				//	sikai = D3DXVec2Dot(&dis3, &ldir);
//
//				//	if (sikai > biggestnow)
//				//	{
//				//		biggest1 = ldir;
//				//		biggestnow = sikai;
//				//	}
//				//}
//				//else
//				//{
//					//自分（敵）の向きとプレイヤーとの位置関係のベクトルとで内積を計算する
//				sikai = D3DXVec2Dot(&dis3, &ldir);
//
//				//内積の値が一定の値より大きい場合視界に入ったと判定しCHASE状態へ移行する
//				if (sikai > biggestnow2)
//				{
//					biggest2 = ldir;
//					biggestnow2 = sikai;
//				}
//				//}
//
//				//Float2 dis4 = (biggest1 + biggest2);
//				//D3DXVec2Normalize(&dis4, &dis4);
//				//float biggestend = (biggestnow + biggestnow2) / 2;
//
//				Float2 disAkari = GetAkariObject(h).pos - g_cursor[l].pos;
//
//				float kakudohantei = D3DXVec2Dot(&mid, &disAkari);
//
//				if (kakudohantei < biggestnow2)
//				{
//					Akarigather(h);
//				}
//			}
//
//		}
//	}
//
//	return true;
//}

//bool CompositionAkari(int clossStart)
//{
//	bool flag = true;
//	bool useflag = false;
//
//
//	for (int h = 0; h < AKARI_NUM; h++)
//	{
//		useflag = false;
//		if (GetAkariObject(h).use == true)
//			useflag = true;
//
//		for (int i = clossStart; i < clossStart + g_curmax - 1 && useflag == true; i++)
//		{
//			int j = i % PLAYER_CURSOR_NUM;
//
//			Float2 top(0, 0);
//			Float2 under(0, 0);
//			Float2 mid(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
//			under = g_cursor[clossStart].pos;
//			for (int f = clossStart + 1; f < clossStart + g_curmax - 1; f++)
//			{
//				int g = f % PLAYER_CURSOR_NUM;
//				if (top.y < g_cursor[f].pos.y)
//				{
//					top.y = g_cursor[f].pos.y;
//				}
//				if (top.x < g_cursor[f].pos.x)
//				{
//					top.x = g_cursor[f].pos.x;
//				}
//				if (under.y > g_cursor[f].pos.y)
//				{
//					under.y = g_cursor[f].pos.y;
//				}
//				if (under.x > g_cursor[f].pos.x)
//				{
//					under.x = g_cursor[f].pos.x;
//				}
//
//			}
//			mid = (top + under) / 2;
//			Float2 dis3 = mid - g_cursor[j].pos;
//			//D3DXVec2Normalize(&dis3, &dis3);
//
//			float biggestnow = 0;
//			float biggestnow2 = 0;
//			Float2 biggest1 = { 0,0 };
//			Float2 biggest2 = { 0,0 };
//
//			float sikai = 0;
//			for (int k = clossStart + 1; k < clossStart + g_curmax; k++)
//			{
//				int l = k % PLAYER_CURSOR_NUM;
//				Float2 ldir = g_cursor[l].pos - g_cursor[j].pos;
//
//
//				//自分（敵）の向きとプレイヤーとの位置関係のベクトルとで内積を計算する
//				sikai = D3DXVec2Dot(&dis3, &ldir);
//
//				//内積の値が一定の値より大きい場合視界に入ったと判定しCHASE状態へ移行する
//				if (sikai > biggestnow2)
//				{
//					biggest2 = ldir;
//					biggestnow2 = sikai;
//				}
//
//				Float2 disAkari = GetAkariObject(h).pos - g_cursor[l].pos;
//
//				float kakudohantei = D3DXVec2Dot(&dis3, &disAkari);
//
//
//				if (kakudohantei < 10000.0)
//				{
//					Akarigather(h);
//				}
//				//if (kakudohantei < biggestnow2)
//				//{
//				//	Akarigather(h);
//				//}
//			}
//
//		}
//	}
//
//	return true;
//}