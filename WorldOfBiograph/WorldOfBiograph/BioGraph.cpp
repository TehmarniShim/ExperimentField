//#include <cassert>
#include "framework.h"





//size_t GetBranchValue(BioLine* bioLine)
//{
//	if (bioLine->pParent == bioLine)
//	{
//		return 0;
//	}
//	else
//	{
//		BioLine* temp = bioLine->pParent;
//		size_t nBranchCount = 1;
//		while (temp->pParent != temp)
//		{
//			nBranchCount++;
//			temp = temp->pParent;
//		}
//		return nBranchCount;
//	}
//}
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

void BioGraph::MakeChange(eGenes eMutation)
{
	constexpr float fAngleCoefficient = 5.0f;
	constexpr float fLengthCoefficient = 10.0f;
	eODDEVEN eOddEvenFlag;
	float fChangeValue;
	bool bIsAngleRotation;
	switch (eMutation)
	{
	case eGenes::ADD_ANGLE_EVEN_BRANCH:
		//mnEvenAngleValue++;
		fChangeValue = fAngleCoefficient;
		eOddEvenFlag = eODDEVEN::EVEN;
		bIsAngleRotation = true;
		break;
	case eGenes::ADD_ANGLE_ODD_BRANCH:
		//mnOddAngleValue++;
		//fChangeValue = f
		fChangeValue = fAngleCoefficient;
		eOddEvenFlag = eODDEVEN::ODD;
		bIsAngleRotation = true;
		break;
	case eGenes::SUBTRACT_ANGLE_EVEN_BRANCH:
		//mnEvenAngleValue--;
		fChangeValue = -fAngleCoefficient;
		eOddEvenFlag = eODDEVEN::EVEN;
		bIsAngleRotation = true;
		break;
	case eGenes::SUBTRACT_ANGLE_ODD_BRANCH:
		//mnOddAngleValue--;
		fChangeValue = -fAngleCoefficient;
		eOddEvenFlag = eODDEVEN::ODD;
		bIsAngleRotation = true;
		break;
	case eGenes::ADD_LENGTH_EVEN_BIO:
		//mnEvenLengthValue++;
		fChangeValue = fLengthCoefficient;
		eOddEvenFlag = eODDEVEN::EVEN;
		bIsAngleRotation = false;

		break;
	case eGenes::ADD_LENGTH_ODD_BIO:
		//mnOddLengthValue++;
		fChangeValue = fLengthCoefficient;
		eOddEvenFlag = eODDEVEN::ODD;
		bIsAngleRotation = false;


		break;
	case eGenes::SUBTRACT_LENGTH_EVEN_BIO:
		//mnEvenLengthValue--;
		fChangeValue = -fLengthCoefficient;
		eOddEvenFlag = eODDEVEN::EVEN;
		bIsAngleRotation = false;

		break;
	case eGenes::SUBTRACT_LENGTH_ODD_BIO:
		//mnOddLengthValue--;
		fChangeValue = -fLengthCoefficient;
		eOddEvenFlag = eODDEVEN::ODD;
		bIsAngleRotation = false;

		break;
	
	default:
		return;
	}
	if (bIsAngleRotation)
	{
		recursiveRotateAngle(mpRootElement, fChangeValue, eOddEvenFlag);
	}
	else
	{
		recursiveExtendLine(mpRootElement, fChangeValue, eOddEvenFlag);
	}
	return;
	/*constexpr float fAngleCoefficient = 5.0f;
	constexpr float fLengthCoefficient = 10.0f;*/
	//float fEvenAngleValue = fAngleCoefficient * static_cast<float>(mnEvenAngleValue);
	//float fOddAngleValue = fAngleCoefficient * static_cast<float>(mnOddAngleValue);
	//float fEvenLengthValue = fLengthCoefficient * static_cast<float>(mnEvenLengthValue);
	//float fOddLengthValue = fLengthCoefficient * static_cast<float>(mnOddLengthValue);
	////eGenes eIndex = eMutation;
	//switch (eMutation)
	//{
	//case eGenes::ADD_ANGLE_EVEN_BRANCH:
	//	if (!mnEvenAngleValue)
	//	{
	//		fEvenAngleValue += fAngleCoefficient;
	//	}
	//	break;
	//case eGenes::ADD_ANGLE_ODD_BRANCH:
	//	if (!mnOddAngleValue)
	//	{
	//		fOddAngleValue += fAngleCoefficient;
	//	}
	//	break;
	//case eGenes::SUBTRACT_ANGLE_EVEN_BRANCH:
	//	if (!mnEvenAngleValue)
	//	{
	//		fEvenAngleValue -= fAngleCoefficient;
	//	}
	//	break;
	//case eGenes::SUBTRACT_ANGLE_ODD_BRANCH:
	//	if (!mnOddAngleValue)
	//	{
	//		fOddAngleValue -= fAngleCoefficient;
	//	}
	//	break;
	//case eGenes::ADD_LENGTH_EVEN_BIO:
	//	if (!mnEvenLengthValue)
	//	{
	//		fEvenLengthValue += fLengthCoefficient;
	//	}
	//	break;
	//case eGenes::ADD_LENGTH_ODD_BIO:
	//	if (!mnOddLengthValue)
	//	{
	//		fOddLengthValue += fLengthCoefficient;
	//	}
	//	break;
	//case eGenes::SUBTRACT_LENGTH_EVEN_BIO:
	//	if (!mnEvenLengthValue)
	//	{
	//		fEvenLengthValue -= fLengthCoefficient;
	//	}
	//	break;
	//case eGenes::SUBTRACT_LENGTH_ODD_BIO:
	//	if (!mnOddLengthValue)
	//	{
	//		fOddLengthValue -= fLengthCoefficient;
	//	}
	//	break;
	//default:
	//	break;
	//}

	//recursiveRenewGraph(mpRootElement, fEvenAngleValue, fOddAngleValue, fEvenLengthValue, fOddLengthValue);
}


void BioGraph::recursiveMakeBioLine(BioLine** ppBranch, BioLine* pNewParent, eNodeDirection eDirection)
{
	assert(pNewParent != nullptr);
	assert(*ppBranch == nullptr);
	size_t nCurrentBranchCount = pNewParent->GetBranchValue();
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

void BioGraph::recursiveRotateAngle(BioLine* pLine, const float fDegree, const eODDEVEN eOddEvenFlag) const
{
	size_t nBranchCount = pLine->GetBranchValue();
	if (nBranchCount)
	{
		BioLine* pParent = pLine->pParent;
		size_t nParentBranchCount = pParent->GetBranchValue();
		if (nParentBranchCount)
		{
			pLine->RenewPosition();
		}
		if (eOddEvenFlag == eODDEVEN::ODD && nBranchCount % 2)
		{
			Vector2D& V2LineDirection = pLine->V2Direction;
			if (pLine->IsRightKid())
			{
				V2LineDirection = V2LineDirection.RotateVector(-fDegree);
			}
			else
			{
				V2LineDirection = V2LineDirection.RotateVector(fDegree);
			}
		}
		else if (eOddEvenFlag == eODDEVEN::EVEN && !(nBranchCount % 2))
		{
			Vector2D& V2LineDirection = pLine->V2Direction;
			if (pLine->IsRightKid())
			{
				V2LineDirection = V2LineDirection.RotateVector(-fDegree);
			}
			else
			{
				V2LineDirection = V2LineDirection.RotateVector(fDegree);
			}
		}
	}
	if (nullptr != pLine->pLeft)
	{
		recursiveRotateAngle(pLine->pLeft, fDegree, eOddEvenFlag);
	}
	if (nullptr != pLine->pRight)
	{
		recursiveRotateAngle(pLine->pRight, fDegree, eOddEvenFlag);
	}
	return;
}

void BioGraph::recursiveExtendLine(BioLine* pLine, const float fLengthValue, const eODDEVEN eOddEvenFlag) const
{
	size_t nBranchCount = pLine->GetBranchValue();
	if (nBranchCount)
	{
		pLine->RenewPosition();
	}
	if (eOddEvenFlag == eODDEVEN::EVEN && !(nBranchCount % 2))
	{
		pLine->fLength += fLengthValue;
	}
	
	else if (eOddEvenFlag == eODDEVEN::ODD && nBranchCount % 2)
	{
		pLine->fLength += fLengthValue;
	}
	if (nullptr != pLine->pLeft)
	{
		recursiveExtendLine(pLine->pLeft, fLengthValue, eOddEvenFlag);
	}
	if (nullptr != pLine->pRight)
	{
		recursiveExtendLine(pLine->pRight, fLengthValue, eOddEvenFlag);
	}

	//if(eODDEVEN::EVEN == eOddEvenFlag && nBranchCount)
	return;
}





//void BioGraph::recursiveRenewGraph(const float fChangeValue, const eGenes eMutation)
//{
//	switch (eMutation)
//	{
//	case eGenes::ADD_ANGLE_EVEN_BRANCH:
//
//		break;
//	case eGenes::ADD_ANGLE_ODD_BRANCH:
//		break;
//	case eGenes::SUBTRACT_ANGLE_EVEN_BRANCH:
//		break;
//	case eGenes::SUBTRACT_ANGLE_ODD_BRANCH:
//		break;
//	case eGenes::ADD_LENGTH_EVEN_BIO:
//		break;
//	case eGenes::ADD_LENGTH_ODD_BIO:
//		break;
//	case eGenes::SUBTRACT_LENGTH_EVEN_BIO:
//		break;
//	case eGenes::SUBTRACT_LENGTH_ODD_BIO:
//		break;
//	case eGenes::DEFAULT:
//		break;
//	default:
//		break;
//	}
//}
//
//void BioGraph::recursiveRenewGraph(BioLine* pLine, const float fEvenAngleValue, const float fOddAngleValue, const float fEvenLengthValue, const float fOddLengthValue) const
//{
//	assert(nullptr != pLine);
//	size_t nBranchValue = GetBranchValue(pLine);
//	Vector2D& V2Direction = pLine->V2Direction;
//	BioLine*& pParent = pLine->pParent;
//	//BioLine*& pRightKid = pParent->pRight;
//	Vector2D& V2RenewdParentDirection = pParent->V2Direction;
//	if (nBranchValue)
//	{
//		assert(pLine->pParent != pLine);
//		const float fRenewedParentLength = pParent->fLength;
//		Vector2D V2RenewedAddingVector = V2RenewdParentDirection * fRenewedParentLength;
//		pLine->V2Position = pParent->V2Position + V2RenewedAddingVector;
//	}
//
//	if (nBranchValue % 2)  //It means it is odd branch number 
//	{
//		
//		if (fOddLengthValue != 0.0f)  //If the value is zero, there does not have to any changes
//		{
//			pLine->fLength += fOddLengthValue;
//		}
//
//		if (fOddAngleValue != 0.0f)
//		{
//			if (pLine == pParent->pLeft)  //If the target is the left node of the parent node, mathematically we need to 'add' angle in order rotate left
//			{
//				V2Direction = V2Direction.RotateVector(fOddAngleValue);
//			}
//			else
//			{
//				V2Direction = V2Direction.RotateVector(-fOddAngleValue);
//			}
//		}
//	}
//	else
//	{
//		
//		if (fEvenLengthValue != 0.0f)
//		{
//			pLine->fLength += fEvenLengthValue;
//		}
//		if (fEvenAngleValue != 0.0f)
//		{
//			if (nBranchValue)
//			{
//				if (pLine == pParent->pLeft)
//				{
//					V2Direction = V2Direction.RotateVector(fEvenAngleValue);
//				}
//				else
//				{
//					V2Direction = V2Direction.RotateVector(-fEvenAngleValue);
//				}
//			}
//
//		}
//	}
//	if (nullptr != pLine->pLeft)
//	{
//		recursiveRenewGraph(pLine->pLeft, fEvenAngleValue, fOddAngleValue, fEvenLengthValue, fOddLengthValue);
//	}
//	if (nullptr != pLine->pRight)
//	{
//		recursiveRenewGraph(pLine->pRight, fEvenAngleValue, fOddAngleValue, fEvenLengthValue, fOddLengthValue);
//	}
//}
