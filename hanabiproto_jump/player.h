/*==============================================================================

   プレイヤー管理[player.h]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"
#include "BENRIclass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_CURSOR_NUM (56)


struct PLAYER
{
	Float2		pos;	//プレイヤーの現在位置
	Float2		vec;	//プレイヤーのベクトル
	Float2		spjp;	//プレイヤーの移動とジャンプ速度
	Float2		jp;		//プレイヤーのジャンプ処理
};

struct CURSOR
{
public:
	Float2		pos;
	Float2		prev_pos;
	D3DXCOLOR	color;

	//bool		csInp;	//カーソル入力判別用
	bool		use;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
bool CompositionAkari(int clossStart, int clossGoal);
PLAYER* GetPlayer(void);
void plus_hissatuwaza(int index);



//PLAYER* GetPlayer(void);
