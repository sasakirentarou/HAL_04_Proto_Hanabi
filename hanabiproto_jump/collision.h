#pragma once

#include "main.h"
#include "player.h"
#include"BENRIclass.h"

bool HitCheckCircle(Float2 cir1pos, float cir1r,
	Float2 cir2pos, float cir2r);

bool HitCheckCircleSq(Float2 cir1pos, float cir1r,
	Float2 cir2pos, float cir2r);

bool HitCheckBox(Float2 box1pos, Float2 box1siz
	, Float2 box2pos, Float2 box2siz);

bool HitCheckBox(Float2 boxpos, Float2 boxsiz
	, Float2 pointpos);

bool HitCheckCross(Float2 start1pos, Float2 end1pos
	, Float2 start2pos, Float2 end2pos);

bool HitCheckCross2nd(Float2 start1pos, Float2 end1pos
	, Float2 start2pos, Float2 end2pos);

bool HitCheckConcavePolygon(CURSOR positions[], Float2 target, int start, int cursor_length);

bool HitCheckConcavePolygon(CURSOR positions[], Float2 target, int length);