#ifndef DrawingRectWallCtrller_h__
#define DrawingRectWallCtrller_h__

#pragma once

#include "irrEngine/IRenderController.h"

#include "ODL/BaseODLFwd.h"

#include "IMeshBuffer.h"

#include "TopoDS_Shape.hxx"

#include <tuple>

class	DrawingRectWallCtrller : public IRenderController
{
public:

	enum EDrawingWallRectState
	{
		EDWRS_BEGIN,
		EDWRS_DRAWING,
		EDWRS_FINISH,
		EDWRS_COUNT
	};

public:

	DrawingRectWallCtrller();
	~DrawingRectWallCtrller();

public://IRenderController

	virtual bool	OnPostEvent(const irr::SEvent& event);

	virtual bool	PreRender3D();
	virtual void	PostRender3D();

public:

	EDrawingWallRectState		GetState() const { return State_; }

	void						Reset();

	void						SetWallThickness(float val) { WallThickness_ = val; }

	void						SetRootODL(const CBaseODLWPtr& rootODL) { RootODL_ = rootODL; }

private:

	float						WallThickness_;

	irr::core::vector3df		FirstPnt_;
	irr::core::vector3df		LastPnt_;

	irr::scene::IMeshBuffer*	MeshBuf_;
	irr::scene::IMeshBuffer*	LineMeshBuf_;
	irr::scene::IMeshBuffer*	PathMeshBuf_;

	irr::video::SMaterial		Material_;

	irr::core::vector2di		CursolIPos_;
	irr::core::vector3df		CurrentPos_;
	bool						Checker_;

	EDrawingWallRectState		State_;
	CBaseODLWPtr				RootODL_;

};

typedef	std::shared_ptr<DrawingRectWallCtrller>	DrawingRectWallCtrllerSPtr;

#endif // DrawingRectWallCtrller_h__