#ifndef DrawingLineWallCtrller_h__
#define DrawingLineWallCtrller_h__

#pragma once

#include "irrEngine/Implemention/EventUserType.h"
#include "irrEngine/IRenderController.h"

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

	irr::scene::IMeshBuffer*	GetMeshBuffer() const;

	void						SetWallThickness(float val) { WallThickness_ = val; }

	TopoDS_Shape				GetShape() const { return FaceShape_; }

private:

	bool						NeedUpdateMesh_;
	float						WallThickness_;
	PointList					Pnts_;
	irr::scene::IMeshBuffer*	TmpRect_;
	irr::scene::IMeshBuffer*	MeshBuf_;
	irr::scene::IMeshBuffer*	LineMeshBuf_;
	irr::video::SMaterial		Material_;
	TopoDS_Shape				FaceShape_;
	irr::core::vector2di		CurrentPos_;
	EDrawingWallLineState		State_;

};

typedef	std::shared_ptr<DrawingLineWallCtrller>	DrawingLineWallCtrllerSPtr;

#endif // DrawingLineWallCtrller_h__