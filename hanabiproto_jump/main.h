/*==============================================================================

   テクスチャの描画 [main.h]
                                                         Author : 
                                                         Date   : 
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include <stdio.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(720)				// ウインドウの高さ

//ゲームを構成するシーンの定義
enum SCENE
{
	SCENE_NONE,		//シーン無し
	SCENE_TITLE,	//タイトル画面
	SCENE_GAME,		//ゲーム本編
	SCENE_RESULT,	//リザルト画面

	SCENE_NUM,		//終端コード
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void SetScene(SCENE nextScene);
float frand(void);
HWND GethWnd();