
#include "collision.h"
#include "main.h"
#include "sprite.h"
#include "camera.h"



//�~�Ɖ~�̓����蔻��
bool HitCheckCircle(Float2 cir1pos, float cir1r,
	                Float2 cir2pos, float cir2r)
{
	//cir1����cir2�Ɍ������x�N�g�������
	Float2 dist;
	dist = cir2pos - cir1pos;

	//dist�x�N�g���̑傫�����v�Z����
	float length;
	//�x�N�g���̑傫���́u�e������2�悵�đS�������B�S�������I������畽�������Ƃ�v�Ōv�Z
	length = sqrt((dist.x*dist.x) + (dist.y*dist.y));

	//circle1�̔��a��circle2�̔��a�𑫂����l�����
	float R;
	R = (cir1r + cir2r);

	if (length < R)
		return true;
	else
		return false;
}

bool HitCheckCircleSq(Float2 cir1pos, float cir1r,
	Float2 cir2pos, float cir2r)
{
	//cir1����cir2�Ɍ������x�N�g�������
	Float2 dist;
	dist = cir2pos - cir1pos;

	//dist�x�N�g���̑傫�����v�Z����
	float length;
	//�x�N�g���̑傫���́u�e������2�悵�đS�������B�S�������I������畽�������Ƃ�v�Ōv�Z
	length = (dist.x*dist.x) + (dist.y*dist.y);

	//circle1�̔��a��circle2�̔��a�𑫂����l�����
	float R;
	R = (cir1r + cir2r) * (cir1r + cir2r);

	if (length < R)
		return true;
	else
		return false;
}

//���Ɣ��̓����蔻��
bool HitCheckBox(Float2 box1pos, Float2 box1siz
				,Float2 box2pos, Float2 box2siz)
{
	//���Ɣ��̒��S���m�̋���
	Float2 boxdis(box1pos - box2pos);
	//���̑傫���̍��v
	Float2 boxsiz(box1siz + box2siz);
	//���̑傫���̍��v�̔���
	Float2 boxsiz_half = boxsiz / 2;

	if (boxsiz_half.x > fabsf(boxdis.x) &&
		boxsiz_half.y > fabsf(boxdis.y))
	{
		return true;
	}

	return false;
}

//���Ɠ_�̓����蔻��
bool HitCheckBox(Float2 boxpos, Float2 boxsiz
				, Float2 pointpos)
{
	//���Ɣ��̒��S���m�̋���
	Float2 boxdis(boxpos - pointpos);
	//���̑傫���̍��v�̔���
	Float2 boxsiz_half = boxsiz / 2;

	if (boxsiz_half.x > fabsf(boxdis.x) &&
		boxsiz_half.y > fabsf(boxdis.y))
	{
		return true;
	}

	return false;
}


bool HitCheckCross(Float2 start1pos, Float2 end1pos
				 , Float2 start2pos, Float2 end2pos)
{
	if (start2pos.x - start1pos.x == 0)
	{
		start2pos.x++;
	}
	if (end2pos.x - start1pos.x == 0)
	{
		end2pos.x++;
	}
	if (end1pos.x - start1pos.x == 0)
	{
		end1pos.x++;
	}

	Float2 s1s2vec(1,(start2pos.y - start1pos.y) / (start2pos.x - start1pos.x));
	Float2 s1e2vec(1,(end2pos.y - start1pos.y) / (end2pos.x - start1pos.x));
	Float2 s1e1vec(1,(end1pos.y - start1pos.y) / (end1pos.x - start1pos.x));



	if (s1e1vec.y < s1s2vec.y && s1e1vec.y > s1e2vec.y)
	{
		if (fabsf(start2pos.y - start1pos.y) + fabsf(start2pos.x - start1pos.x) < fabsf(end1pos.y - start1pos.y) + fabsf(end1pos.x - start1pos.x) && 
			fabsf(end2pos.y - start1pos.y) + fabsf(end2pos.x - start1pos.x) < fabsf(end1pos.y - start1pos.y) + fabsf(end1pos.x - start1pos.x))
		{
			return true;
		}
	}
	if (s1e1vec.y > s1s2vec.y && s1e1vec.y < s1e2vec.y)
	{
		if (fabsf(start2pos.y - start1pos.y) + fabsf(start2pos.x - start1pos.x) < fabsf(end1pos.y - start1pos.y) + fabsf(end1pos.x - start1pos.x) &&
			fabsf(end2pos.y - start1pos.y) + fabsf(end2pos.x - start1pos.x) < fabsf(end1pos.y - start1pos.y) + fabsf(end1pos.x - start1pos.x))
		{
			return true;
		}
	}
	return false;
}

bool HitCheckCross2nd(Float2 start1pos, Float2 end1pos
	, Float2 start2pos, Float2 end2pos)
{
	//Float2 minus(-10, -10);
	Float2 box1pos = (start1pos + end1pos);
	box1pos = box1pos / 2;
	Float2 box2pos = (start2pos + end2pos);
	box2pos = box2pos / 2;
	Float2 box1size(fabsf(start1pos.x - end1pos.x), fabsf(start1pos.y - end1pos.y));
	Float2 box2size(fabsf(start2pos.x - end2pos.x), fabsf(start2pos.y - end2pos.y));

	//box1size + minus;
	//box2size + minus;

	return HitCheckBox(box1pos,box1size,box2pos,box2size);
	//return true;

}

bool HitCheckConcavePolygon(CURSOR positions[], Float2 target,int length)
{
	float result = 0;
	//�x�[�X���W���擾����
	D3DXVECTOR2 basePos = GetBase();
	Float2 BasePos(basePos.x, basePos.y);

	target = target + BasePos;

	for (int i = 0; i < length; i++)
	{
			Float2 l1 = positions[i].pos - target;
			Float2 l2 = positions[i+ 1 % length].pos - target;

			D3DXVECTOR2 l1v2 = { l1.x,l1.y };
			D3DXVECTOR2 l2v2 = { l2.x,l2.y };

			float angle = AngleOf2Vector(l1v2, l2v2);

			if (D3DXVec2CCW(&l1v2, &l2v2) > 0)
			{
				angle *= -1;
			}

			result += angle;
	}

	float unit = 1.0f / 360.0f;
	result *= unit;

	// ���v���E�����v���ǂ�������肦�邽�ߐ�Βl�Ŕ��肷��
	if (fabs(result) >= 0.01f)
	{
		return true;
	}
	return false;
	//return Mathf.Abs(result) >= 0.01f;
}

bool HitCheckConcavePolygon(CURSOR positions[], Float2 target,int start,int cursor_length)
{
	float result = 0;
	//�x�[�X���W���擾����
	D3DXVECTOR2 basePos = GetBase();
	Float2 BasePos(basePos.x, basePos.y);

	target = target + BasePos;

	for (int i = start; i < start + cursor_length; i++)
	{
		if (i == start + cursor_length - 1)
		{
			Float2 l1 = positions[i% PLAYER_CURSOR_NUM].pos - target;
			Float2 l2 = positions[start].pos - target;

			D3DXVECTOR2 l1v2 = { l1.x,l1.y };
			D3DXVECTOR2 l2v2 = { l2.x,l2.y };

			float angle = AngleOf2Vector(l1v2, l2v2);

			if (D3DXVec2CCW(&l1v2, &l2v2) > 0)
			{
				angle *= -1;
			}

			result += angle;
		}
		else
		{
			Float2 l1 = positions[i% PLAYER_CURSOR_NUM].pos - target;
			Float2 l2 = positions[(i + 1) % PLAYER_CURSOR_NUM].pos - target;

			D3DXVECTOR2 l1v2 = { l1.x,l1.y };
			D3DXVECTOR2 l2v2 = { l2.x,l2.y };

			float angle = AngleOf2Vector(l1v2, l2v2);

			if (D3DXVec2CCW(&l1v2, &l2v2) > 0)
			{
				angle *= -1;
			}

			result += angle;
		}
	}

	float unit  = 1.0f / 360.0f;
	result *= unit;

	// ���v���E�����v���ǂ�������肦�邽�ߐ�Βl�Ŕ��肷��
	if (fabs(result) >= 0.01f)
	{
		return true;
	}
	return false;
	//return Mathf.Abs(result) >= 0.01f;
}