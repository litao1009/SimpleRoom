#include "stdafx.h"

#include "irrEngine/SRenderContext.h"
#include "RoomLayoutWallCtrller.h"
#include "UserEvent.h"

using namespace irr;
using namespace core;

enum class EWallState
{
	EWS_PROPERTY,
	EWS_PROPERTY_WAIT,
	EWS_MOVE,
	EWS_COUNT
};


class	RoomLayoutWallCtrller::Imp
{
public:

	Imp()
	{
		FirstUpdate_ = true;
		State_ = EWallState::EWS_PROPERTY;
	}

	bool		FirstUpdate_;
	EWallState	State_;
	vector2di	CursorIPos_;
	vector3df	CurrentPos_;
};

RoomLayoutWallCtrller::RoomLayoutWallCtrller( const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc ):IRoomLayoutODLBaseCtrller(rc), ImpUPtr_(new Imp)
{

}

RoomLayoutWallCtrller::~RoomLayoutWallCtrller()
{

}

void RoomLayoutWallCtrller::Init()
{

}

bool RoomLayoutWallCtrller::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

// 	if ( EWallState::EWS_PROPERTY == imp_.State_ )
// 	{
// 		::PostMessage((HWND)GetRenderContextSPtr()->GetHandle(), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_WALL_PROPERTY, 0);
// 		imp_.State_ = EWallState::EWS_PROPERTY_WAIT;
// 	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_MOUSE_MOVED )
	{
		imp_.CursorIPos_.X = evt.MouseInput.X;
		imp_.CursorIPos_.Y = evt.MouseInput.Y;
	}

	return false;
}

bool RoomLayoutWallCtrller::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	if ( GetPickingODL().expired() )
	{
		return false;
	}

	auto activeWall = std::static_pointer_cast<WallODL>(GetPickingODL().lock());

	plane3df plan(0,0,0,0,1,0);
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);
	imp_.CurrentPos_.Y = 0;
	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);

	

	return false;
}

void RoomLayoutWallCtrller::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	}

bool RoomLayoutWallCtrller::Valid()
{
	auto& imp_ = *ImpUPtr_;



	return false;
}