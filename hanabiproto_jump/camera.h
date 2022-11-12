#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct CAMERA_2D
{
	D3DXVECTOR2 pos;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void DrawCamera(void);

D3DXVECTOR2 GetBase(void);
void SetCameraPos(float x, float y);
