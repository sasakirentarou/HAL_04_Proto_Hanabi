/*==============================================================================

				   �I�u�W�F�N�g���� [akariobject.cpp]
				Author : ���ł���
				Date    : 2022/10/23(�ŏI�ҏW��)
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
// �}�N����`							
//*****************************************************************************							
//*****************************************************************************							
// �v���g�^�C�v�錾							
//*****************************************************************************							

//*****************************************************************************							
// �O���[�o���ϐ�							
//*****************************************************************************							
static int g_TextureNo;

static HanabiAkariObject g_AkariObject[AKARI_NUM];

static float g_U, g_V;

Float2 MovePos[AKARI_NUM];

Float2 Ppos;

bool g_composition;
//=============================================================================							
// ����������							
//=============================================================================							
HRESULT InitAkariObject(void)
{
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/proto_effect_akari.png");

	//������
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

	////������
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
// �I������							
//=============================================================================							
void UninitAkariObject(void)
{

}

//=============================================================================							
// �X�V����							
//=============================================================================							
void UpdateAkariObject(void)
{
	//�e�X�g
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
	}//�e�X�g�I���




	//�͂����͈͓��́uAKARI�v���W�܂�悤��
	for (int i = 0; i < AKARI_NUM; i++)
	{
		if (g_AkariObject[i].gather && g_AkariObject[i].use == true && g_AkariObject[i].setvec == false && Ppos.x != GetPlayer()->pos.x && Ppos.y != GetPlayer()->pos.y)
		{
			//�n�_A����n�_B�̈ړ�����
			//MovePos=�n�_B - �n�_A;
			MovePos[i].x = GetPlayer()->pos.x - g_AkariObject[i].pos.x;
			MovePos[i].y = GetPlayer()->pos.y - g_AkariObject[i].pos.y;
			//���t���[�������ďW�܂邩
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


				//��������Ł[�[�[
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
			//�����ł������Ł[�[�[
			if (g_AkariObject[i].frame > 400)
			{
				g_AkariObject[i].use = false;
				g_AkariObject[i].frame = 0;

			}
		}
	}
}

//=============================================================================							
// �`�揈��							
//=============================================================================							
void DrawAkariObject(void)
{
	//�x�[�X���W���擾����
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

//������̈ʒu���Z�b�g
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
			//�F�Â�
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

//������̈ʒu�ƍő�����Z�b�g
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
			//�F�Â�
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