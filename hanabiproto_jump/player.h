/*==============================================================================

   �v���C���[�Ǘ�[player.h]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"
#include "BENRIclass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_CURSOR_NUM (56)


struct PLAYER
{
	Float2		pos;	//�v���C���[�̌��݈ʒu
	Float2		vec;	//�v���C���[�̃x�N�g��
	Float2		spjp;	//�v���C���[�̈ړ��ƃW�����v���x
	Float2		jp;		//�v���C���[�̃W�����v����
};

struct CURSOR
{
public:
	Float2		pos;
	Float2		prev_pos;
	D3DXCOLOR	color;

	//bool		csInp;	//�J�[�\�����͔��ʗp
	bool		use;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
bool CompositionAkari(int clossStart, int clossGoal);
PLAYER* GetPlayer(void);
void plus_hissatuwaza(int index);



//PLAYER* GetPlayer(void);
