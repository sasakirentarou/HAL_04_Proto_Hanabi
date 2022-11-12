/*==============================================================================

				   オブジェクト処理 [akariobject.cpp]
				Author : いでうら
				Date    : 2022/10/23(最終編集日)
------------------------------------------------------------------------------------------------------------------------------------------

==============================================================================*/
#include "object.h"	
#include "texture.h"
#include "collision.h"
#include "sprite.h"
#include "camera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************							
// マクロ定義							
//*****************************************************************************							
//*****************************************************************************							
// プロトタイプ宣言							
//*****************************************************************************							

//*****************************************************************************							
// グローバル変数							
//*****************************************************************************							
static int g_TextureNo;

static HanabiAkariObject g_AkariObject[AKARI_NUM];

static float g_U, g_V;

Float2 MovePos[AKARI_NUM];

Float2 Ppos;

bool g_composition;
//=============================================================================							
// 初期化処理							
//=============================================================================							
HRESULT InitAkariObject(void)
{
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/proto_effect_akari.png");

	//初期化
	for (int i = 0; i < AKARI_NUM; i++)
	{
		g_AkariObject[i].use = false;
		g_AkariObject[i].gather = false;
		g_AkariObject[i].setvec = false;
		g_AkariObject[i].dir.x = 0.0f;
		g_AkariObject[i].dir.y = 0.0f;
		g_AkariObject[i].pos.x = SCREEN_WIDTH;
		g_AkariObject[i].pos.y = SCREEN_HEIGHT;
		g_AkariObject[i].frame = 0;
		g_AkariObject[i].color = { 1.0f,1.0f,1.0f,1.0f };
		g_AkariObject[i].siz = { 32.0f,32.0f };
	}

	////お試し
	//for (int i = 0; i < 5; i++)
	//{
	//	g_AkariObject[i].gather = true;
	//	g_AkariObject[i].pos.x = frand() * SCREEN_WIDTH;
	//	g_AkariObject[i].pos.y = frand() * SCREEN_HEIGHT;
	//	g_AkariObject[i].use = true;
	//}

	for (int i = 0; i < AKARI_NUM; i++)
	{
		MovePos[i] = { 0.0f,0.0f };
	}

	g_U = 0.0f;
	g_V = 0.0f;

	g_composition = true;

	return S_OK;
}

//=============================================================================							
// 終了処理							
//=============================================================================							
void UninitAkariObject(void)
{

}

//=============================================================================							
// 更新処理							
//=============================================================================							
void UpdateAkariObject(void)
{
	//テスト
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		for (int i = 0; i < AKARI_NUM; i++)
		{
			if (g_AkariObject[i].use == false)
			{
				g_AkariObject[i].use = true;
				g_AkariObject[i].gather = false;
				g_AkariObject[i].pos.x = frand() * SCREEN_WIDTH;
				g_AkariObject[i].pos.y = frand() * SCREEN_HEIGHT;
				break;
			}
		}
	}//テスト終わり




	//囲った範囲内の「AKARI」が集まるように
	for (int i = 0; i < AKARI_NUM; i++)
	{
		if (g_AkariObject[i].gather && g_AkariObject[i].use == true && g_AkariObject[i].setvec == false && Ppos.x != GetPlayer()->pos.x && Ppos.y != GetPlayer()->pos.y)
		{
			//地点Aから地点Bの移動距離
			//MovePos=地点B - 地点A;
			MovePos[i].x = GetPlayer()->pos.x - g_AkariObject[i].pos.x;
			MovePos[i].y = GetPlayer()->pos.y - g_AkariObject[i].pos.y;
			//何フレームかけて集まるか
			MovePos[i].x /= (60 / (g_AkariObject[i].frame + 1 * 5));
			MovePos[i].y /= (60 / (g_AkariObject[i].frame + 1 * 5));

			//g_AkariObject[i].setvec = true;
		}


		if (g_AkariObject[i].gather && g_AkariObject[i].use)
		{
			g_AkariObject[i].pos.x += MovePos[i].x;
			g_AkariObject[i].pos.y += MovePos[i].y;

			if (g_AkariObject[i].pos.x < /*(SCREEN_WIDTH / 2)*/GetPlayer()->pos.x + 19 && g_AkariObject[i].pos.x >/*(SCREEN_WIDTH / 2)*/GetPlayer()->pos.x - 19
				&& g_AkariObject[i].pos.y </*(SCREEN_HEIGHT / 2)*/GetPlayer()->pos.y + 19 && g_AkariObject[i].pos.y > /*(SCREEN_HEIGHT / 2)*/GetPlayer()->pos.y - 19)
			{
				MovePos[i].x = 0.0f;
				MovePos[i].y = 0.0f;
				g_AkariObject[i].frame += 1;


				//合成後消滅ーーー
				if (g_AkariObject[i].frame > 40)
				{
					for (int j = 0; j < AKARI_NUM; j++)
					{
						if (g_AkariObject[j].gather == true && g_AkariObject[j].use == true)
						{
							g_AkariObject[j].use = false;
							g_AkariObject[j].frame = 0;
							plus_hissatuwaza(5);
						}
					}
				}
			}
		}
		else if (g_AkariObject[i].use == true)
		{
			g_AkariObject[i].pos.x += MovePos[i].x * 3;
			g_AkariObject[i].pos.y += MovePos[i].y * 3;
			g_AkariObject[i].frame += 1;
			//合成できず消滅ーーー
			if (g_AkariObject[i].frame > 400)
			{
				g_AkariObject[i].use = false;
				g_AkariObject[i].frame = 0;

			}
		}
	}
}

//=============================================================================							
// 描画処理							
//=============================================================================							
void DrawAkariObject(void)
{
	//ベース座標を取得する
	D3DXVECTOR2 basePos = GetBase();
	for (int i = 0; i < AKARI_NUM; i++)
	{
		if (g_AkariObject[i].use == true)
		{
			DrawSpriteColor(g_TextureNo, basePos.x + g_AkariObject[i].pos.x, basePos.y + g_AkariObject[i].pos.y, 60.0f, 60.0f,
				1.0f, 1.0f, 1.0f, 1.0f, g_AkariObject[i].color);
		}
	}

}


HanabiAkariObject GetAkariObject(int index)
{
	return g_AkariObject[index];
}

void Akarigather(int index)
{
	g_AkariObject[index].gather = true;
	g_AkariObject[index].frame = 0;
}

//明かりの位置をセット
void SetAkari(Float2 pos)
{
	int create_akari = 4;
	Float2 akarivec[4] =
	{
		{1.0f,0.0f},
		{-1.0f,0.0f},
		{0.0f,1.0f},
		{0.0f,-1.0f}
	};

	for (int i = 0; i < AKARI_NUM; i++)
	{
		if (g_AkariObject[i].use == false)
		{
			g_AkariObject[i].use = true;
			g_AkariObject[i].pos = pos;
			g_AkariObject[i].setvec = false;
			g_AkariObject[i].gather = false;
			MovePos[i] = akarivec[create_akari - 1];
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
				g_AkariObject[i].color = { RGB[0],RGB[1],RGB[2],1.0f };
			}

			create_akari -= 1;
			if (create_akari <= 0)
			{
				break;
			}
		}
	}
}

//明かりの位置と最大個数をセット
void SetAkari(Float2 pos, int saidai)
{
	int create_akari = 4;
	Float2 akarivec[4] =
	{
		{1.0f,0.0f},
		{-1.0f,0.0f},
		{0.0f,1.0f},
		{0.0f,-1.0f}
	};

	for (int i = 0; i < AKARI_NUM; i++)
	{
		if (g_AkariObject[i].use == false)
		{
			g_AkariObject[i].use = true;
			g_AkariObject[i].pos = pos;
			g_AkariObject[i].setvec = false;
			g_AkariObject[i].gather = false;
			MovePos[i] = akarivec[create_akari - 1];
			//色づけ
			{
				float RGB[3];
				for (int j = 0; j < 3; j++)
				{
					RGB[j] = frand();
				}
				RGB[saidai] = 1.0f;
				g_AkariObject[i].color = { RGB[0],RGB[1],RGB[2],1.0f };
			}

			create_akari -= 1;
			if (create_akari <= 0)
			{
				break;
			}
		}
	}
}