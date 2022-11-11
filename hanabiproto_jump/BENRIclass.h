#pragma once
#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
//GMのと同じように使える、XYのfloat型二つ――――――――――――――――――-
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

	//＋の場合は自分自身が左辺、引数が右辺、自分と同じクラスのメンバにはアクセスできる。ほどほどに。
	Float2 operator+(const Float2& v)const {
		return Float2(x + v.x, y + v.y);
	}
	//一項演算子
	Float2 operator-(void)const {
		return Float2(-x, -y);
		//一個だけ演算子が逆などは、よくやってしまう間違え
	}
	Float2 operator-(const Float2& v)const {
		return *this + -v;
		//一項演算子があればこれもできる
	}
	//スカラー倍
	Float2 operator*(float s)const {
		return Float2(x * s, y * s);
	}
	//分数
	Float2 operator/(float w)const {
		return Float2(x / w, y / w);
	}
};
//――――――――――――――――――――――――――――――――――――――

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float AngleOf2Vector(D3DXVECTOR2 A, D3DXVECTOR2 B);
int Float2_length_gather(int start, int goal);
