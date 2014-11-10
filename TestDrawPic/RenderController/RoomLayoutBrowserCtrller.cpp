#include "stdafx.h"

#include "RoomLayoutBrowserCtrller.h"
#include "irrEngine/SRenderContext.h"
#include "ODL/GraphODL.h"
#include "ODL/CornerODL.h"
#include "ODL/WallODL.h"

using namespace irr;
using namespace core;

class	RoomLayoutBrowserCtrller::Imp
{
public:

	Imp()
	{
		LMousePressDown_ = false;
		DeletePressDown_ = false;
	}

public:


	bool			LMousePressDown_;
	bool			DeletePressDown_;

	vector2di		CursorIPos_;
	vector3df		CurrentPos_;

	WallODLWPtr		SweepingWall_;
	WallODLWPtr		PickingWall_;
	CornerODLWPtr	SweepingCorner_;
	CornerODLWPtr	PickingCorner_;

	GraphODLWPtr	Graph_;
};

RoomLayoutBrowserCtrller::RoomLayoutBrowserCtrller( const GraphODLWPtr& graphODL ):ImpUPtr_(new Imp)
{
	ImpUPtr_->Graph_ = graphODL;
}

RoomLayoutBrowserCtrller::~RoomLayoutBrowserCtrller()
{

}

void RoomLayoutBrowserCtrller::Init()
{

}

bool RoomLayoutBrowserCtrller::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
	{
		imp_.LMousePressDown_ = true;
	}

	if ( evt.EventType == EET_KEY_INPUT_EVENT && evt.KeyInput.Key == KEY_DELETE && evt.KeyInput.PressedDown )
	{
		imp_.DeletePressDown_ = true;
	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_MOUSE_MOVED )
	{
		imp_.CursorIPos_.X = evt.MouseInput.X;
		imp_.CursorIPos_.Y = evt.MouseInput.Y;
	}

	return false;
}

bool RoomLayoutBrowserCtrller::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	plane3df plan(0,0,0,0,1,0);
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);
	imp_.CurrentPos_.Y = 0;

	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);

	if ( !imp_.SweepingWall_.expired() )
	{
		auto wall = imp_.SweepingWall_.lock();

		if ( Standard_True == wall->GetBaseBndBox().IsOut(cursorPnt.Transformed(wall->GetAbsoluteTransform().Inverted())) )
		{
			wall->SetSweeping(false);
			imp_.SweepingWall_.reset();
		}
	}

	if ( imp_.SweepingWall_.expired() )
	{
		for ( auto& curWall : imp_.Graph_.lock()->GetChildrenList() )
		{
			if ( EODLT_WALL != curWall->GetType() )
			{
				continue;
			}

			auto curTrueWall = std::static_pointer_cast<WallODL>(curWall);

			auto transform = curTrueWall->GetAbsoluteTransform();
			
			if ( Standard_False == curTrueWall->GetBaseBndBox().IsOut(cursorPnt.Transformed(curTrueWall->GetAbsoluteTransform().Inverted())) )
			{
				imp_.SweepingWall_ = curTrueWall;
				break;
			}
		}
	}

	if ( imp_.LMousePressDown_ )
	{
		if ( !imp_.PickingWall_.expired() )
		{
			imp_.PickingWall_.lock()->SetPicking(false);
		}

		imp_.PickingWall_ = imp_.SweepingWall_;
	}

	if ( imp_.DeletePressDown_ )
	{
		if ( !imp_.PickingWall_.expired() )
		{
			imp_.Graph_.lock()->RemoveWall(imp_.PickingWall_.lock());
			imp_.PickingWall_.reset();
		}
	}

	if ( !imp_.SweepingWall_.expired() )
	{
		imp_.SweepingWall_.lock()->SetSweeping(true);
	}

	if ( !imp_.PickingWall_.expired() )
	{
		imp_.PickingWall_.lock()->SetPicking(true);
	}

	imp_.LMousePressDown_ = false;
	imp_.DeletePressDown_ = false;

	return false;
}

void RoomLayoutBrowserCtrller::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	if ( !imp_.SweepingWall_.expired() )
	{
		imp_.SweepingWall_.lock()->SetSweeping(false);
	}
}
