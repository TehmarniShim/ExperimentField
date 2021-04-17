#include <cassert>
#include "BioGraph.h"



size_t GetBranchValue(BioLine* bioLine)
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
	: mpRootElement(nullptr)
	, mfThetaInDegree(30.0f)
	, mnBranchCount(4)
	, mnEvenAngleValue(0)
	, mnOddAngleValue(0)
	, mnEvenLengthValue(0)
	, mnOddLengthValue(0)
{
	mpRootElement = new BioLine();
	mpRootElement->V2Position.fX = fRootPosX;
	mpRootElement->V2Position.fY = fRootPosY;
	mpRootElement->fLength = fLength;
	mpRootElement->V2Direction.fX = 0.0f;
	mpRootElement->V2Direction.fY = -1.0f;
	mpRootElement->pLeft = nullptr;
	mpRootElement->pRight = nullptr;
	mpRootElement->pParent = mpRootElement;
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
	if (nullptr != mpRootElement)
	{
		RecursiveRelease(mpRootElement);
	}
}



void BioGraph::MakeGraph()
{
	assert(nullptr != mpRootElement);
	if (nullptr == mpRootElement->pLeft)
	{
		recursiveMakeBioLine(&mpRootElement->pLeft, mpRootElement, eNodeDirection::LEFT);
	}
	if (nullptr == mpRootElement->pRight)
	{
		recursiveMakeBioLine(&mpRootElement->pRight, mpRootElement, eNodeDirection::RIGHT);
	}
}

VecBio BioGraph::GetBioGraph() const
{
	assert(nullptr != mpRootElement);
	VecBio VBioLines;
	recursiveCollectBioLines(VBioLines, mpRootElement);
	return VBioLines;
}

void BioGraph::RelocateGraph(const size_t XPos, const size_t YPos) const
{
	assert(nullptr != mpRootElement);
	const float fCurrentXPos = mpRootElement->V2Position.fX;
	const float fCurrentYPos = mpRootElement->V2Position.fY;
	const float fRelocateX = static_cast<float>(XPos);
	const float fRelocateY = static_cast<float>(YPos);
	const float fSettleValueX = fRelocateX - fCurrentXPos;
	const float fSettleValueY = fRelocateY - fCurrentYPos;
	recursiveRelocateGraph(mpRootElement, fSettleValueX, fSettleValueY);
}

void BioGraph::MakeChange(eGenes eGeneKind)
{
	switch (eGeneKind)
	{
	case eGenes::ADD_ANGLE_EVEN_BRANCH:
		mnEvenAngleValue++;

		break;
	case eGenes::ADD_ANGLE_ODD_BRANCH:
		mnOddAngleValue++;
		break;
	case eGenes::SUBTRACT_ANGLE_EVEN_BRANCH:
		mnEvenAngleValue--;

		break;
	case eGenes::SUBTRACT_ANGLE_ODD_BRANCH:
		mnOddAngleValue--;

		break;
	case eGenes::ADD_LENGTH_EVEN_BIO:
		mnEvenLengthValue++;

		break;
	case eGenes::ADD_LENGTH_ODD_BIO:
		mnOddLengthValue++;

		break;
	case eGenes::SUBTRACT_LENGTH_EVEN_BIO:
		mnEvenLengthValue--;
		break;
	case eGenes::SUBTRACT_LENGTH_ODD_BIO:
		mnOddLengthValue--;

		break;
	default:
		break;
	}
	constexpr float fAngleCoefficient = 5.0f;
	constexpr float fLengthCoefficient = 10.0f;
	const float fEvenAngleValue = fAngleCoefficient * static_cast<float>(mnEvenAngleValue);
	const float fOddAngleValue =  fAngleCoefficient * static_cast<float>(mnOddAngleValue);
	const float fEvenLengthValue = fLengthCoefficient * static_cast<float>(mnEvenLengthValue);
	const float fOddLengthValue = fLengthCoefficient * static_cast<float>(mnOddLengthValue);
	recursiveRenewGraph(mpRootElement, fEvenAngleValue, fOddAngleValue, fEvenLengthValue, fOddLengthValue);
}


void BioGraph::recursiveMakeBioLine(BioLine** ppBranch, BioLine* pNewParent, eNodeDirection eDirection)
{
	assert(pNewParent != nullptr);
	assert(*ppBranch == nullptr);
	size_t nCurrentBranchCount = GetBranchValue(pNewParent);
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
		if (eDirection == eNodeDirection::LEFT)
		{
			(*ppBranch)->V2Direction = VParentDirection.RotateVector(mfThetaInDegree);
		}
		else
		{
			(*ppBranch)->V2Direction = VParentDirection.RotateVector(-mfThetaInDegree);
		}
		(*ppBranch)->fLength = pNewParent->fLength * 0.8f;
		recursiveMakeBioLine(&(*ppBranch)->pLeft, *ppBranch, eNodeDirection::LEFT);
		recursiveMakeBioLine(&(*ppBranch)->pRight, *ppBranch, eNodeDirection::RIGHT);
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

void BioGraph::recursiveRenewGraph(BioLine* pLine, const float fEvenAngleValue, const float fOddAngleValue, const float fEvenLengthValue, const float fOddLengthValue) const
{
	assert(nullptr != pLine);
	size_t nBranchValue = GetBranchValue(pLine);
	Vector2D& V2Direction = pLine->V2Direction;
	BioLine*& pParent = pLine->pParent;
	Vector2D& V2RenewdParentDirection = pParent->V2Direction;
	if (nBranchValue)
	{
		assert(pLine->pParent != pLine);
		const float fRenewedParentLength = pParent->fLength;
		Vector2D V2RenewedAddingVector = V2RenewdParentDirection * fRenewedParentLength;
		pLine->V2Position = pParent->V2Position + V2RenewedAddingVector;
	}

	if (nBranchValue % 2)  //It means it is odd branch number 
	{
		pLine->fLength += fOddLengthValue;
		if (pLine == pParent->pLeft)  //If the target is the left node of the parent node, mathematically we need to 'add' angle in order rotate left
		{
			V2Direction.RotateVector(fOddAngleValue);
		}
		else
		{
			V2Direction.RotateVector(-fOddAngleValue);
		}
	}
	else
	{
		pLine->fLength += fEvenLengthValue;
		if (nBranchValue)  
		{
			if (pLine == pParent->pLeft)
			{
				V2Direction.RotateVector(fEvenAngleValue);
			}
			else
			{
				V2Direction.RotateVector(-fEvenAngleValue);
			}
		}
	}
	if (nullptr != pLine->pLeft)
	{
		recursiveRenewGraph(pLine->pLeft, fEvenAngleValue, fOddAngleValue, fEvenLengthValue, fOddLengthValue);
	}
	if (nullptr != pLine->pRight)
	{
		recursiveRenewGraph(pLine->pLeft, fEvenAngleValue, fOddAngleValue, fEvenLengthValue, fOddLengthValue);
	}
}
