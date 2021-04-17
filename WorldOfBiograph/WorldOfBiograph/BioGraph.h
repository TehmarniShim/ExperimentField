#pragma once
#include <EASTL/vector.h>
#include <vector>
#include "Genes.h"
#include "TehmarniEAStlHeader.h"
#include "WorldOfBiograph.h"
#include "Vector2D.h"
#include "typedef.h"

//struct BioLine;
typedef eastl::vector<BioLine*> VecBio;
//enum class NodeDirection;

//
//enum class NodeDirection
//{
//	RIGHT,
//	LEFT
//};


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
	
private:
	void recursiveMakeBioLine(BioLine ** ppBranch, BioLine* pParent, NodeDirection eDirection);
	void recursiveCollectBioLines(VecBio& VBioLines, BioLine* bio) const;
	void recursiveRelocateGraph(BioLine* pLine, const float Xcomponent, const float Ycomponent) const;

private:
	BioLine* mRootElement;
	float mfThetaInDegree;
	size_t mnBranchCount;
	
};
