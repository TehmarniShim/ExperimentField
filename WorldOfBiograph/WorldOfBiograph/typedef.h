#pragma once
#include "Vector2D.h"


class Vector2D;

struct BioLine
{
	Vector2D V2Position;
	Vector2D V2Direction;
	BioLine* pLeft;
	BioLine* pRight;
	BioLine* pParent;
	float fLength;
};


enum class eNodeDirection
{
	RIGHT,
	LEFT
};