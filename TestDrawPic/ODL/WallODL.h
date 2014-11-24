#ifndef WallODL_h__
#define WallODL_h__

#pragma once

#include "BaseODL.h"
#include "HoleODLFwd.h"

#include "CornerODLFwd.h"
#include "RoomODLFwd.h"
#include "GraphODLFwd.h"

#include "TopoDS_Shape.hxx"

class WallODL : public BaseODL
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

	gp_Dir				GetDirection(const CornerODLSPtr& fromCorner = nullptr) const;

	CornerODLWPtr		GetOtherCorner(const CornerODLSPtr& corner ) const
	{
		assert(FirstCorner_.lock()==corner || SecondCorner_.lock()==corner);
		return FirstCorner_.lock() == corner ? SecondCorner_ : FirstCorner_;
	}

	void				SetThickness(float val) { Thickness_ = val; }

	float				GetThickness() const { return Thickness_; }

	void				SetHeight(float val) { Height_ = val; }

	float				GetHeight() const { return Height_; }

	float				GetLength() const;

	void				UpdateBaseShape();

	void				UpdateBaseMesh();

	void				UpdateCutShape();

	void				UpdateCutMesh();

	const MeshPoints&	GetMeshPoints() { return MeshPoints_; }

	void				SetDefaultTexture();

	ChildrenList		GetHoles() const;

	void				CutHole(const HoleODLSPtr& hole);

	void				SeamHole(const HoleODLSPtr& hole);

	bool				IsCutMeshDirty() const { return CutMeshDirty_; }

	void				SetCutMeshDirty(bool val) { CutMeshDirty_ = val; }

	RoomODLList			GetRooms() const;

	void				AddRoom(const RoomODLSPtr& room);

	void				RemoveRoom(const RoomODLSPtr& room);

private:

	EdgeIndex		GraphIndex_;

	CornerODLWPtr	FirstCorner_;
	CornerODLWPtr	SecondCorner_;

	float			Thickness_;
	float			Height_;
	MeshPoints		MeshPoints_;

	bool			CutMeshDirty_;
	TopoDS_Shape	CutShape_;
	TopoDS_Shape	BottomShape_;

	GraphODLWPtr	GraphODL_;
	RoomODLWList	Rooms_;
};

typedef	std::shared_ptr<WallODL>	CWallODLSPtr;
typedef	std::weak_ptr<WallODL>		CWallODLWPtr;

#endif // WallODL_h__
