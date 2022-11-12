#pragma once
#include "main.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
//GM�̂Ɠ����悤�Ɏg����AXY��float�^��\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\-
class Float2:public D3DXVECTOR2
{
public:
	float x;
	float y;

	Float2() :x(0), y(0) {}
	Float2(float ix, float iy)
		:x(ix)
		, y(iy) {
	}

	//�{�̏ꍇ�͎������g�����ӁA�������E�ӁA�����Ɠ����N���X�̃����o�ɂ̓A�N�Z�X�ł���B�قǂقǂɁB
	Float2 operator+(const Float2& v)const {
		return Float2(x + v.x, y + v.y);
	}
	//�ꍀ���Z�q
	Float2 operator-(void)const {
		return Float2(-x, -y);
		//��������Z�q���t�Ȃǂ́A�悭����Ă��܂��ԈႦ
	}
	Float2 operator-(const Float2& v)const {
		return *this + -v;
		//�ꍀ���Z�q������΂�����ł���
	}
	//�X�J���[�{
	Float2 operator*(float s)const {
		return Float2(x * s, y * s);
	}
	//����
	Float2 operator/(float w)const {
		return Float2(x / w, y / w);
	}
};
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
float AngleOf2Vector(D3DXVECTOR2 A, D3DXVECTOR2 B);
int Float2_length_gather(int start, int goal);
