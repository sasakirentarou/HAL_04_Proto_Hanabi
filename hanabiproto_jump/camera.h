#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct CAMERA_2D
{
	D3DXVECTOR2 pos;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void DrawCamera(void);

D3DXVECTOR2 GetBase(void);
void SetCameraPos(float x, float y);
