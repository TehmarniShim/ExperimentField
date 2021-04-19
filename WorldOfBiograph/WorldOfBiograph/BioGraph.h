#pragma once
//#include <EASTL/vector.h>
//#include <vector>
#include "framework.h"
//#include "TehmarniEAStlHeader.h"
//#include "Genes.h"
//#include "WorldOfBiograph.h"
//#include "Vector2D.h"
//#include "typedef.h"


class BioLine;
enum class eGenes;
enum class eNodeDirection;
enum class eODDEVEN;
//class Vector2D;
class BioGraph;

typedef eastl::vector<BioLine*> VecBio;
//typedef std::vector<BioLine*> VecBio;
//class BioLine
//{
//	friend class BioGraph;
//
//	//public:
//private:
//
//	Vector2D V2Position;
//	Vector2D V2Direction;
//	BioLine* pLeft;
//	BioLine* pRight;
//	BioLine* pParent;
//	float fLength;
//};
//

class BioGraph
{
public:
	BioGraph(const float fRootX, const float fRootY, const float fLength);
	BioGraph() = delete;
	virtual ~BioGraph();
	BioGraph(const BioGraph& rhs) = delete;
	BioGraph operator=(const BioGraph& rhs) = delete;


	void MakeGraph();
	VecBio GetBioGraph() const;
	void RelocateGraph(const size_t width, const size_t height) const;
	void MakeChange(eGenes eMutation);

private:
	void recursiveMakeBioLine(BioLine** ppBranch, BioLine* pParent, eNodeDirection eDirection);
	void recursiveCollectBioLines(VecBio& VBioLines, BioLine* bio) const;
	void recursiveRelocateGraph(BioLine* pLine, const float Xcomponent, const float Ycomponent) const;
	//void recursiveRenewGraph(BioLine* pLine, const float fEvenAngleValue, const float fOddAngleValue, const float fEvenLengthValue, const float fOddLengthValue) const;
	//void recursiveRenewGraph(const float fChangeValue, const eGenes eMutation);

	void recursiveRotateAngle(BioLine* pLine, const float fDegree, const eODDEVEN eOddEvenFlag);
	void recursiveExtendLine(BioLine* pLine, const float fLengthValue, const eODDEVEN eOddEvenFlag);

	
	
	
private:
	BioLine* mpRootElement;
	float mfThetaInDegree;
	size_t mnBranchCount;
	int mnEvenAngleValue;
	int mnOddAngleValue;
	int mnEvenLengthValue;
	int mnOddLengthValue;

};
