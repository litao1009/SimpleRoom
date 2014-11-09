#ifndef WallODL_h__
#define WallODL_h__

#pragma once

#include "BaseODL.h"

#include "CornerODLFwd.h"
#include "GraphODLFwd.h"

#include "TopoDS_Shape.hxx"

class WallODL : public CBaseODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	typedef	Graph::edge_descriptor	EdgeIndex;
	typedef	std::array<gp_Pnt, 6>	MeshPoints;

public:

	WallODL(const GraphODLWPtr graphODL, const CornerODLSPtr& firstCorner, const CornerODLSPtr& secondCorner,  float wallThickness = 200.f, float height = 2400.f);
	~WallODL(void);

public:
	
	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_WALL; }

	virtual	void	Init();
	
protected:

	virtual	void	UpdateSweeping();

	virtual	void	UpdatePicking();

public:

	bool				IsBezierCurve() const;

	TopoDS_Edge			GetEdge(const CornerODLSPtr& fromCorner = nullptr) const;

	const EdgeIndex&	GetIndex() const { return GraphIndex_; }

	void				SetIndex(const EdgeIndex& val) { GraphIndex_ = val; }

	CornerODLWPtr		GetFirstCorner() const { return FirstCorner_; }

	CornerODLWPtr		GetSecondCorner() const { return SecondCorner_; }

	CornerODLWPtr		GetOtherCorner(const CornerODLSPtr& corner ) const
	{
		assert(FirstCorner_.lock()==corner || SecondCorner_.lock()==corner);
		return FirstCorner_.lock() == corner ? SecondCorner_ : FirstCorner_;
	}

	void				SetThickness(float val) { Thickness_ = val; }

	float				GetThickness() const { return Thickness_; }

	void				SetHeight(float val) { Height_ = val; }

	float				GetHeight() const { return Height_; }

	void				UpdateMesh();

	const MeshPoints&	GetMeshPoints() { return MeshPoints_; }

	void				SetDefaultTexture();

private:

	EdgeIndex		GraphIndex_;

	CornerODLWPtr	FirstCorner_;
	CornerODLWPtr	SecondCorner_;

	float			Thickness_;
	float			Height_;
	MeshPoints		MeshPoints_;

	GraphODLWPtr	GraphODL_;
};

typedef	std::shared_ptr<WallODL>	CWallODLSPtr;
typedef	std::weak_ptr<WallODL>		CWallODLWPtr;

#endif // WallODL_h__
