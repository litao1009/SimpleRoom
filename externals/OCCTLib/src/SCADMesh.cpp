#include "SCADMesh.h"

#include <algorithm>
#include <iterator>
#include <functional>
#include <cassert>

#include <map>

bool		IsPntANearToLeftBottmAgainstPntB(const SPntInfo& pa, const SPntInfo& pb)
{
	if ( pa.Y < pb.Y )
	{
		return true;
	}
	else if ( pa.Y > pb.Y )
	{
		return false;
	}
	else
	{
		return ( pa.X <= pb.X ) ? true : false;
	}
}

bool		IsPntANearToLeftBottmAgainstPntB(SimpleSegment::const_iterator pa, SimpleSegment::const_iterator pb)
{
	return IsPntANearToLeftBottmAgainstPntB(*pa, *pb);
}

SimpleSegment::const_iterator	GetPntInSeg(const Segments& segs, bool isNearToLeftBottom)
{
	SimpleSegment::const_iterator		curBegSeg = segs.front().begin();
	SimpleSegment::const_iterator		curEndSeg = --segs.back().end();

	if ( IsPntANearToLeftBottmAgainstPntB(curBegSeg, curEndSeg) )
	{
		return isNearToLeftBottom ? curBegSeg : curEndSeg;
	}
	else
	{
		return isNearToLeftBottom ? curEndSeg : curBegSeg;
	}
}

bool		IsSegANearToLeftBottmAgainstSegB(const Segments& segA, const Segments& segB)
{
	const SPntInfo&	sa = *(GetPntInSeg(segA, true));
	const SPntInfo&	sb = *(GetPntInSeg(segB, true));

	if ( sa == sb )
	{
		const SPntInfo&	sc = *(GetPntInSeg(segA, false));
		const SPntInfo&	sd = *(GetPntInSeg(segB, false));

		return ( sc.X <= sd.X ) ? true : false;
	}
	else
	{
		return IsPntANearToLeftBottmAgainstPntB(sa, sb);
	}
}

IndexSegments::const_iterator	FindNextLine( SPntInfo& beginPnt, const IndexSegments& theIndexSegs )
{

	for (auto itor=theIndexSegs.begin(); itor!=theIndexSegs.end(); itor++)
	{
		SimpleSegment::const_iterator pntA = (itor->theSegments).front().begin();
		SimpleSegment::const_iterator pntB = --(itor->theSegments).back().end();

		if ( *pntA == beginPnt )
		{
			beginPnt = *pntB;
			return itor;
		}
		else if ( *pntB == beginPnt )
		{
			beginPnt = *pntA;
			return itor;
		}
	}

	return theIndexSegs.end();
}


void			SCADMesh::GetSolidMesh(PntsVector& pntsVec, TrianglesVector& trisVec)
{
	pntsVec.clear();
	trisVec.clear();

	for ( auto& curFace : mTheFaces )
	{
		_CalcuFace(curFace, pntsVec, trisVec);
	}
}

void			SCADMesh::GetWireframeMesh(Segments& segVec)
{
	segVec = mTheLines;
}

const SCADMesh::Faces&	SCADMesh::GetFaces()
{
	return mTheFaces;
}

void			SCADMesh::PushFace(const SFaceInfo& aFace)
{
	mTheFaces.push_back(aFace);
}


const Segments&	SCADMesh::GetSegments()
{
	return mTheLines;
}

void			SCADMesh::PushSegments(const Segments& segs)
{
	std::copy(segs.begin(), segs.end(), std::back_inserter(mTheLines));
}

const IndexSegments&	SCADMesh::GetIndexSegments()
{
	typedef	std::map<int, int>	FaceIndexType;

	_SortIndexSegment();

	FaceIndexType fit;
	for ( std::size_t i=0; i<mTheFaces.size(); ++i )
	{
		fit[mTheFaces[i].theIndex] = i;
	}

	for ( std::size_t i=0; i<mTheIndexLines.size(); ++i )
	{
		mTheIndexLines[i].theFaceIndex = fit[mTheIndexLines[i].theFaceIndex];
	}

	return mTheIndexLines;
}

void			SCADMesh::PushIndexSegmenst(const SIndexSegmentInfo& indexSeg)
{
	mTheIndexLines.push_back(indexSeg);
}


void			SCADMesh::_CalcuFace(SFaceInfo& face, PntsVector& pntsVec, TrianglesVector& trisVec)
{
	for ( auto& curTriangle : face.theTriangles )
	{
		_UpdateIndex(curTriangle, trisVec, pntsVec.size());
	}

	std::copy(face.thePnts.begin(), face.thePnts.end(), std::back_inserter(pntsVec));
}

void			SCADMesh::_UpdateIndex(STriangleInfo& theTriangle, TrianglesVector& trisVec, std::size_t pntNr)
{
	trisVec.emplace_back(std::get<0>(theTriangle)+pntNr, std::get<1>(theTriangle)+pntNr, std::get<2>(theTriangle)+pntNr);
}

void			SCADMesh::_SortIndexSegment()
{
	if ( mTheIndexLines.empty() )
	{
		return;
	}

	IndexSegments	SortedIndexSegs;

	IndexSegments::const_iterator	itorBegSeg = mTheIndexLines.begin();
	for (auto itor=mTheIndexLines.begin(); itor!=mTheIndexLines.end(); itor++)
	{
		std::size_t dis = std::distance(mTheIndexLines.begin(), itor);
		if ( !IsSegANearToLeftBottmAgainstSegB(itorBegSeg->theSegments, itor->theSegments) )
		{
			itorBegSeg = itor;
		}
	}

	SortedIndexSegs.push_back(*itorBegSeg);
	mTheIndexLines.erase(itorBegSeg);

	SPntInfo	begPnt	= *(GetPntInSeg(SortedIndexSegs.front().theSegments, false));
	do
	{
		IndexSegments::const_iterator fItor = FindNextLine(begPnt, mTheIndexLines);

		if ( fItor == mTheIndexLines.end() )
		{
			assert(mTheIndexLines.empty());
			break;
		}

		SortedIndexSegs.push_back(*fItor);
		mTheIndexLines.erase(fItor);
	}
	while(1);

	mTheIndexLines.swap(SortedIndexSegs);
}