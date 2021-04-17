#pragma once
#include <EASTL/vector.h>
#include <vector>
#include "Genes.h"
#include "TehmarniEAStlHeader.h"
#include "WorldOfBiograph.h"
#include "Vector2D.h"
#include "typedef.h"

//typedef eastl::vector<BioLine*> VecBio;
typedef std::vector<BioLine*> VecBio;

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
	void MakeChange(eGenes eGeneKind);

private:
	void recursiveMakeBioLine(BioLine** ppBranch, BioLine* pParent, eNodeDirection eDirection);
	void recursiveCollectBioLines(VecBio& VBioLines, BioLine* bio) const;
	void recursiveRelocateGraph(BioLine* pLine, const float Xcomponent, const float Ycomponent) const;
	void recursiveRenewGraph(BioLine* pLine, const float fEvenAngleValue, const float fOddAngleValue, const float fEvenLengthValue, const float fOddLengthValue) const;

private:
	BioLine* mpRootElement;
	float mfThetaInDegree;
	size_t mnBranchCount;
	int mnEvenAngleValue;
	int mnOddAngleValue;
	int mnEvenLengthValue;
	int mnOddLengthValue;

};
