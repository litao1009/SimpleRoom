#ifndef DrawingLineWallCtrller_h__
#define DrawingLineWallCtrller_h__

#pragma once

#include "irrEngine/Implemention/EventUserType.h"
#include "irrEngine/IRenderController.h"

#include "ODL/BaseODLFwd.h"

#include "IMeshBuffer.h"

#include "TopoDS_Shape.hxx"

#include <vector>

class	DrawingLineWallCtrller : public IRenderController
{
public:

	typedef	std::vector<irr::core::vector3df>	PointList;

	enum EDrawingWallLineState
	{
		EDWLS_BEGIN,
		EDWLS_DRAWING,
		EDWLS_FINISH,
		EDWLS_COUNT
	};

	class	SResult
	{
	public:
		TopoDS_Shape	Shape_;
		PointList		Pnts_;
	};

public:

	DrawingLineWallCtrller();
	~DrawingLineWallCtrller();

public://IRenderController

	virtual bool	OnPostEvent(const irr::SEvent& event);

	virtual bool	PreRender3D(const SRenderContext& rc);
	virtual void	PostRender3D(const SRenderContext& rc);

public:

	EDrawingWallLineState		GetState() const { return State_; }

	void						Reset();

	void						SetWallThickness(float val) { WallThickness_ = val; }

	SResult						GetResult() const
	{
		SResult s;
		s.Shape_ = FaceShape_;
		s.Pnts_ = Pnts_;
		return s;
	}

	bool						UpdateMesh();

	void						SetRootODL(const CBaseODLWPtr& rootODL) { CurrentRoot_ = rootODL; }

private:

	bool						NeedUpdateMesh_;
	bool						Checker_;
	float						WallThickness_;
	float						PolarAngle_;
	float						PolarAngleRange_;
	float						LastAngle_;
	irr::scene::IMeshBuffer*	TmpRect_;
	irr::scene::IMeshBuffer*	MeshBuf_;
	irr::scene::IMeshBuffer*	LineMeshBuf_;
	irr::scene::IMeshBuffer*	PathMeshBuf_;
	irr::scene::IMeshBuffer*	CircleMeshBuf_;
	irr::core::vector2di		CursorIPos_;
	irr::core::vector3df		CurrentPos_;
	irr::video::SMaterial		Material_;
	EDrawingWallLineState		State_;
	TopoDS_Shape				FaceShape_;
	PointList					Pnts_;
	CBaseODLWPtr				CurrentRoot_;
};

typedef	std::shared_ptr<DrawingLineWallCtrller>	DrawingLineWallCtrllerSPtr;

#endif // DrawingLineWallCtrller_h__