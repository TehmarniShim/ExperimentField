#include <cassert>
#include "BioGraph.h"



size_t GetBranchCount(BioLine* bioLine)
{
	if (bioLine->pParent == bioLine)
	{
		return 0;
	}
	else
	{
		BioLine* temp = bioLine->pParent;
		size_t nBranchCount = 1;
		while (temp->pParent != temp)
		{
			nBranchCount++;
			temp = temp->pParent;
		}
		return nBranchCount;
	}
}
BioGraph::BioGraph(const float fRootPosX, const float fRootPosY, const float fLength)
	: mRootElement(nullptr)
	, mfThetaInDegree(5.0f)
	, mnBranchCount(8)
{
	mRootElement = new BioLine();
	mRootElement->V2Position.fX = fRootPosX;
	mRootElement->V2Position.fY = fRootPosY;
	mRootElement->fLength = fLength;
	mRootElement->V2Direction.fX = 0.0f;
	mRootElement->V2Direction.fY = -1.0f;
	mRootElement->pLeft = nullptr;
	mRootElement->pRight = nullptr;
	mRootElement->pParent = mRootElement;
}

void RecursiveRelease(BioLine* target)
{
	if (nullptr != target->pLeft)
	{
		RecursiveRelease(target->pLeft);
	}
	if (nullptr != target->pRight)
	{
		RecursiveRelease(target->pRight);
	}
	delete target;
	target = nullptr;
}

BioGraph::~BioGraph()
{
	if (nullptr != mRootElement)
	{
		RecursiveRelease(mRootElement);
	}
}



void BioGraph::MakeGraph()
{
	assert(nullptr != mRootElement);
	if (nullptr == mRootElement->pLeft)
	{
		recursiveMakeBioLine(&mRootElement->pLeft, mRootElement, NodeDirection::LEFT);
	}
	if (nullptr == mRootElement->pRight)
	{
		recursiveMakeBioLine(&mRootElement->pRight, mRootElement, NodeDirection::RIGHT);
	}
}

VecBio BioGraph::GetBioGraph() const
{
	VecBio VBioLines;
	recursiveCollectBioLines(VBioLines, mRootElement);
	return VBioLines;
}

void BioGraph::RelocateGraph(const size_t XPos, const size_t YPos) const
{
	assert(nullptr != mRootElement);
	const float fCurrentXPos = mRootElement->V2Position.fX;
	const float fCurrentYPos = mRootElement->V2Position.fY;
	const float fRelocateX = static_cast<float>(XPos);
	const float fRelocateY = static_cast<float>(YPos);
	const float fSettleValueX = fRelocateX - fCurrentXPos;
	const float fSettleValueY = fRelocateY - fCurrentYPos;
	recursiveRelocateGraph(mRootElement, fSettleValueX, fSettleValueY);
}




void BioGraph::recursiveMakeBioLine(BioLine** ppBranch, BioLine* pNewParent, NodeDirection eDirection)
{
	assert(pNewParent != nullptr);
	assert(*ppBranch == nullptr);
	size_t nCurrentBranchCount = GetBranchCount(pNewParent);
	if (nCurrentBranchCount < mnBranchCount)
	{
		*ppBranch = new BioLine();
		(*ppBranch)->pParent = pNewParent;
		(*ppBranch)->pLeft = nullptr;
		(*ppBranch)->pRight = nullptr;
		const float fParentLength = pNewParent->fLength;
		Vector2D VAddingVector = pNewParent->V2Direction * fParentLength;
		(*ppBranch)->V2Position = pNewParent->V2Position + VAddingVector;
		auto& VParentDirection = pNewParent->V2Direction;
		DirectX::XMVECTOR VXaxis{ 1.0f, 0.0f };
		DirectX::XMVECTOR VTargetVector{ VParentDirection.fX, -VParentDirection.fY };
		DirectX::XMVECTOR VAngle = DirectX::XMVector2AngleBetweenNormals(VXaxis, VTargetVector);
		DirectX::XMFLOAT2 F2Angle{};
		DirectX::XMStoreFloat2(&F2Angle, VAngle);
		float fAngleInDegree = DirectX::XMConvertToDegrees(F2Angle.x);
		VParentDirection.Normalize();
		if (eDirection == NodeDirection::LEFT)
		{
			(*ppBranch)->V2Direction = VParentDirection.RotateVector(mfThetaInDegree);
		}
		else
		{
			(*ppBranch)->V2Direction = VParentDirection.RotateVector(-mfThetaInDegree);
		}
		(*ppBranch)->fLength = pNewParent->fLength * 0.8f;
		recursiveMakeBioLine(&(*ppBranch)->pLeft, *ppBranch, NodeDirection::LEFT);
		recursiveMakeBioLine(&(*ppBranch)->pRight, *ppBranch, NodeDirection::RIGHT);
	}
}

void BioGraph::recursiveCollectBioLines(VecBio& VBioLines, BioLine* bio) const
{
	assert(nullptr != bio);
	VBioLines.push_back(bio);
	if (bio->pLeft != nullptr)
	{
		recursiveCollectBioLines(VBioLines, bio->pLeft);
	}
	if (bio->pRight != nullptr)
	{
		recursiveCollectBioLines(VBioLines, bio->pRight);
	}
}

void BioGraph::recursiveRelocateGraph(BioLine* pLine, const float fSettleXValue, const float fSettleYValue) const
{
	assert(nullptr != pLine);
	pLine->V2Position.fX += fSettleXValue;
	pLine->V2Position.fY += fSettleYValue;
	if (nullptr != pLine->pLeft)
	{
		recursiveRelocateGraph(pLine->pLeft, fSettleXValue, fSettleYValue);
	}
	if (nullptr != pLine->pRight)
	{
		recursiveRelocateGraph(pLine->pRight, fSettleXValue, fSettleYValue);
	}
}

