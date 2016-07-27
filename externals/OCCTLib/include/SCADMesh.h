#pragma once

#ifdef	IMP_DLL
#define DLL_EXPORT	__declspec (dllexport)
#else
#define DLL_EXPORT
#endif

#include <vector>
#include <array>
#include <tuple>

class	SPntInfo
{
public:
	SPntInfo():X(0),Y(0),Z(0),nX(0),nY(0),nZ(0),U(0),V(0) {}

	bool	operator ==(const SPntInfo& rhs) const
	{
		if ( this == &rhs )
		{
			return true;
		}

		if ( X==rhs.X && Y==rhs.Y && Z==rhs.Z
			&& nX==rhs.nX && nY==rhs.nY && nZ==rhs.nZ
			&& U==rhs.U && V==rhs.V)
		{
			return true;
		}

		return false;
	}

	bool	operator!=(const SPntInfo& rhs) const
	{
		return !(*this == rhs);
	}

	double	X,	Y,	Z;
	double	nX,	nY,	nZ;
	double	U,	V;
};


typedef	std::tuple<double,double,double>	SVectorInfo;
typedef	std::tuple<int,int,int>				STriangleInfo;

typedef	std::vector<SPntInfo>			PntsVector;
typedef	std::vector<STriangleInfo>		TrianglesVector;
typedef	std::array<SPntInfo, 2>			SimpleSegment;
typedef	std::vector<SimpleSegment>		Segments;

class	SFaceInfo
{
public:

	TrianglesVector	theTriangles;
	PntsVector		thePnts;
	int				theIndex;
};

class	SIndexSegmentInfo
{
public:
	Segments	theSegments;
	int			theFaceIndex;
};

typedef	std::vector<SIndexSegmentInfo>		IndexSegments;

class	SCADMesh
{
public:

	typedef			std::vector<SFaceInfo>			Faces;

public:

	DLL_EXPORT void					GetSolidMesh(PntsVector& pntsVec, TrianglesVector& trisVec);

	DLL_EXPORT void					GetWireframeMesh(Segments& segVec);

	DLL_EXPORT const Faces&			GetFaces();

	DLL_EXPORT void					PushFace(const SFaceInfo& aFace);

	DLL_EXPORT const Segments&		GetSegments();

	DLL_EXPORT void					PushSegments(const Segments& segs);

	DLL_EXPORT const IndexSegments&	GetIndexSegments();

	DLL_EXPORT void					PushIndexSegmenst(const SIndexSegmentInfo& indexSeg);

private:

	DLL_EXPORT void					_CalcuFace(SFaceInfo& face, PntsVector& pntsVec, TrianglesVector& trisVec);

	DLL_EXPORT void					_UpdateIndex(STriangleInfo& theTriangle, TrianglesVector& trisVec, std::size_t pntNr);

	DLL_EXPORT void					_SortIndexSegment();

private:

	Faces			mTheFaces;
	Segments		mTheLines;
	IndexSegments	mTheIndexLines;
};