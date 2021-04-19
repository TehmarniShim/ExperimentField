#pragma once
//#include "Vector2D.h"
//#include "BioGraph.h"
#include "framework.h"
class Vector2D;
class BioGraph;

class BioLine
{
public:
	friend class BioGraph;

	size_t GetBranchValue() const;
	bool IsRightKid() const;

	void RenewPosition();
	//private:

	Vector2D V2Position;
	Vector2D V2Direction;
	BioLine* pLeft;
	BioLine* pRight;
	BioLine* pParent;
	float fLength;
};

