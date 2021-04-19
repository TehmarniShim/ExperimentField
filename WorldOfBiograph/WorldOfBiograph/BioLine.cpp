#include "BioLine.h"

size_t BioLine::GetBranchValue() const
{
	if (this->pParent == this)
	{
		return 0;
	}
	else
	{
		BioLine* temp = this->pParent;
		size_t nBranchCount = 1;
		while (temp->pParent != temp)
		{
			nBranchCount++;
			temp = temp->pParent;
		}
		return nBranchCount;
	}
}

bool BioLine::IsRightKid() const
{
	assert(this != this->pParent);
	return this == this->pParent->pRight;
}

void BioLine::RenewPosition()
{
	const Vector2D& V2ParentDirection = pParent->V2Direction;
	const Vector2D& V2ParentPosition = pParent->V2Position;
	const float& fParentLineLength = pParent->fLength;
	Vector2D V2AddingVector = V2ParentDirection * fParentLineLength;
	V2Position = V2ParentPosition + V2AddingVector;
	return;

}
