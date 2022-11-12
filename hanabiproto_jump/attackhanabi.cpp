/*==============================================================================

   攻撃花火の処理 [attackhanabi.cpp]
														 Author : 横田郁弥
														 Date   : 2022/10/26
--------------------------------------------------------------------------------

==============================================================================*/
#include "object.h"
#include "player.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_HANABI 100

//弾のデータを管理する構造体
struct AtHANABI
{
	Float2 pos;	//表示座標
	Float2 dir;	//移動方向
	float       speed;	//移動速度
	float		frame;	//持続時間
	Float2		vec;
	D3DXCOLOR		color;
	int			saidaiing;

	bool			use;	//可視フラグ
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void HitHanabi(bool isHit);
void CreateHanabi(Float2 plpos, Float2 cspos);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static GameObject g_Hanabi0bj;
static EnemyObject g_Enemy0bj;

//static GameObject g_Objects[256];
//static PLAYER g_Player;	//プレイヤー用

static int g_AtHanabi;	//攻撃花火用のテクスチャの識別子
static AtHANABI g_HANABI[NUM_HANABI];	//弾バッファ
Float2 MovePosHanabi[NUM_HANABI];
D3DXVECTOR2 g_Test;
Float2 AtHanabiDeadPos[NUM_HANABI];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAtHanabi(void)
{
	//------------仮--------------------

	//テクスチャを読み込んで識別子を受け取る
	g_AtHanabi = LoadTexture((char*)"data/TEXTURE/proto_effect_attackhanabi.png");
	g_Test = { 3.0f,3.0f };

	//花火バッファの初期化
	for (int i = 0; i < NUM_HANABI; i++)
	{
		g_HANABI[i].pos = Float2(0.0f, 0.0f);	//表示座標
		g_HANABI[i].dir = Float2(1.0f, 1.0f);	//移動方向
		g_HANABI[i].speed = 3.0f;					//移動速度
		g_HANABI[i].frame = 0.0f;
		g_HANABI[i].color = { 0.0f,0.0f,0.0f,1.0f };
		g_HANABI[i].saidaiing = 0;

		g_HANABI[i].use = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAtHanabi(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateAtHanabi(void)
{
	//------------仮--------------------

		//弾バッファ全体を走査する
	for (int i = 0; i < NUM_HANABI; i++)
	{
		//可視フラグがオンの弾だけ座標を更新する
		if (g_HANABI[i].use == true)
		{
			//弾の座標更新
			//g_HANABI[i].pos += g_HANABI[i].dir * g_HANABI[i].speed;
			g_HANABI[i].pos.x += g_HANABI[i].dir.x * g_HANABI[i].speed;
			g_HANABI[i].pos.y += g_HANABI[i].dir.y * g_HANABI[i].speed;


			g_HANABI[i].frame += 1.0f;

			if (g_HANABI[i].frame > 80.0f)
			{
				g_HANABI[i].frame = 0.0f;
				AtHanabiDeadPos[i] = g_HANABI[i].pos;
				g_HANABI[i].use = false;

				SetAkari(g_HANABI[i].pos, g_HANABI[i].saidaiing);
			}
		}
	}




	

	//花火バッファ全体を走査する
	//for (int i = 0; i < NUM_HANABI; i++)
	//{
	//	//可視フラグがオンの花火だけ座標を更新する
	//	if (g_HANABI[i].use == true)
	//	{
	//		//花火の座標更新
	//		g_HANABI[i].pos += g_HANABI[i].dir * g_HANABI[i].speed;

	//		g_HANABI[i].frame += 1.0f;

	//		//180フレーム経過したら
	//		if (g_HANABI[i].frame > 180.0f)
	//		{
	//			//花火を画面から削除する
	//			g_HANABI[i].use = false;
	//		}
	//	}
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAtHanabi(void)
{

	//------------仮--------------------

	//ベース座標を取得する
	D3DXVECTOR2 basePos = GetBase();


	//花火バッファ全体を走査する
	for (int i = 0; i < NUM_HANABI; i++)
	{
		//可視フラグがオンの花火だけ描画する
		if (g_HANABI[i].use == true)
		{
			float rot;

			//移動方向から回転角度を求める
			rot = atan2f(g_HANABI[i].dir.y, g_HANABI[i].dir.x) + (D3DX_PI / 2);

			DrawSpriteColorRotate(g_AtHanabi, basePos.x + g_HANABI[i].pos.x, basePos.y + g_HANABI[i].pos.y,
				32.0f, 32.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				g_HANABI[i].color,
				rot);
		}
	}
}


//=============================================================================
// 攻撃花火の移動処理
//=============================================================================
//void MoveHanabi(void)
//{
//	MovePosHanabi[0].x = SCREEN_WIDTH / 2 - GetPlayer()->pos.x;
//	MovePosHanabi[0].y = SCREEN_HEIGHT / 2 - GetPlayer()->pos.y;
//	//何フレームかけて集まるか
//	MovePosHanabi[0].x /= 60;
//	MovePosHanabi[0].y /= 60;
//
//}

void MoveHanabiAI(void)
{

}

//=============================================================================
// 攻撃花火の当たり判定処理
//=============================================================================
void HitHanabi(bool isHit)
{
	//if (g_Hanabi0bj.pos.x == g_Enemy0bj.pos.x || g_Hanabi0bj.pos.y == g_Enemy0bj.pos.y)
	//{
	//	isHit = true;
	//}
	
}
void CreateHanabi(Float2 plpos, Float2 cspos)
{
	plpos - cspos;


	//------------仮--------------------

	//花火バッファ全体を走査する
	//for (int i = 0; i < NUM_HANABI; i++)
	//{
		//可視フラグがオフの花火を探す
	//	if (g_HANABI[i].use == false)
	//	{
	//		g_HANABI[i].pos = plpos;

	//		switch ()
	//		{
	//		case 0://上向き
	//			g_HANABI[i].dir = D3DXVECTOR2(0.0f, -1.0f);
	//			break;
	//		case 1://右向き
	//			g_HANABI[i].dir = D3DXVECTOR2(1.0f, 0.0f);
	//			break;
	//		case 2://下向き
	//			g_HANABI[i].dir = D3DXVECTOR2(0.0f, 1.0f);
	//			break;
	//		case 3://左向き
	//			g_HANABI[i].dir = D3DXVECTOR2(-1.0f, 0.0f);
	//			break;
	//		}

	//		g_HANABI[i].speed = 6.0f;

	//		//可視フラグをオンにする
	//		g_HANABI[i].use = true;

	//		//持続時間をリセットする
	//		g_HANABI[i].frame = 0.0f;

	//		//花火を一つセットしたら終了させる
	//		break;
	//	}
	//}
}

Float2 GetAtHanabiPos(Float2 PosA, Float2 PosB)
{
	//座標取得
	Float2 HanabiVec;

	//地点Aから地点Bの移動距離
		//MovePos=地点B - 地点A;
	HanabiVec.x = PosB.x - PosA.x;
	HanabiVec.y = PosB.y - PosA.y;
	
	//条件　下の一定数によって変えなきゃ動かない
	if (PosB.x < PosB.x + /*条件→*/1 && PosB.x >PosB.x - /*条件→*/1 && PosB.y <PosB.y + /*条件→*/1 && PosB.y >PosB.y - /*条件→*/1){}
	else
	{
		//何(float型の一定数)ずつ進むか
		HanabiVec.x -= 1;
		HanabiVec.y -= 1;
	}

	return Float2(HanabiVec);
}

void Normalizer(Float2 Player, Float2 Cursor)
{
	D3DXVECTOR2 Vec;
	Float2 Vecf2;
	Vecf2 = Cursor - Player;
	Vec.x = Vecf2.x;
	Vec.y = Vecf2.y;
	D3DXVec2Normalize(&Vec, &Vec);
	Vecf2.x = Vec.x;
	Vecf2.y = Vec.y;

	for (int i = 0; i < NUM_HANABI; i++)
	{
		if (g_HANABI[i].use == false)
		{
			g_HANABI[i].use = true;
			g_HANABI[i].pos = Player;
			g_HANABI[i].dir = Vecf2;
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
				g_HANABI[i].color = { RGB[0],RGB[1],RGB[2],1.0f };
				g_HANABI[i].saidaiing = saidai;
			}
			//ｂレーク
			break;
		}
	}
}

Float2 GetAtHanabiDeadPos(int index)
{
	return AtHanabiDeadPos[index];
}