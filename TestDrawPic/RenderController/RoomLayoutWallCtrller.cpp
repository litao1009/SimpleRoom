#include "stdafx.h"

#include "irrEngine/SRenderContext.h"
#include "RoomLayoutWallCtrller.h"
#include "UserEvent.h"

#include "ODL/GraphODL.h"

using namespace irr;
using namespace core;

enum class EWallState
{
	EWS_SWEEPING,
	EWS_MOUSEHOLDING,
	EWS_MOVING,
	EWS_PROPERTY,
	EWS_PROPERTY_WAIT,
	EWS_COUNT
};


class	RoomLayoutWallCtrller::Imp
{
public:

	Imp()
	{
		Cancel_ = false;
		HasSetProperty_ = false;
		LMouseLeftUp_ = false;
		LMousePressDown_ = false;
		State_ = EWallState::EWS_SWEEPING;
	}

public:

	bool			Cancel_;
	bool			HasSetProperty_;
	bool			LMouseLeftUp_;
	bool			LMousePressDown_;
	EWallState		State_;
	vector2di		CursorIPos_;
	vector3df		SavePos_;
	vector3df		CurrentPos_;
	SEventWallInfo	EventInfo_;
	GraphODLWPtr	Graph_;
};

RoomLayoutWallCtrller::RoomLayoutWallCtrller( const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc ):IRoomLayoutODLBaseCtrller(rc), ImpUPtr_(new Imp)
{
	ImpUPtr_->Graph_ = graphODL;
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

 	if ( evt.EventType == EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_WALL_PROPERTY )
 	{
		imp_.Cancel_ = evt.UserEvent.UserData2 != 1;
 		imp_.HasSetProperty_ = true;
 	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_MOUSE_MOVED )
	{
		imp_.CursorIPos_.X = evt.MouseInput.X;
		imp_.CursorIPos_.Y = evt.MouseInput.Y;
	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
	{
		imp_.LMousePressDown_ = true;
	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
	{
		imp_.LMouseLeftUp_ = true;
	}

	return false;
}

bool RoomLayoutWallCtrller::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	auto activeWall = std::static_pointer_cast<WallODL>(GetPickingODL().lock());

	plane3df plan(0,0,0,0,1,0);
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);
	imp_.CurrentPos_.Y = 0;
	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);

	activeWall->SetPicking(true);

	switch (imp_.State_)
	{
	case EWallState::EWS_SWEEPING:
		{
			activeWall->SetSweeping(true);

			if ( imp_.LMousePressDown_ )
			{
				imp_.SavePos_ = imp_.CurrentPos_;
				imp_.State_ = EWallState::EWS_MOUSEHOLDING;
			}
		}
		break;
	case EWallState::EWS_MOUSEHOLDING:
		{
			auto thickness = activeWall->GetThickness();

			if ( imp_.SavePos_.getDistanceFromSQ(imp_.CurrentPos_) > thickness * thickness / 4 )
			{
				imp_.State_ = EWallState::EWS_MOVING;
			}
			else if ( imp_.LMouseLeftUp_ )
			{
				imp_.State_ = EWallState::EWS_PROPERTY;

				imp_.EventInfo_.Height_ = activeWall->GetHeight();
				imp_.EventInfo_.Thickness_ = activeWall->GetThickness();
				auto pointer = reinterpret_cast<int>(static_cast<void*>(&imp_.EventInfo_));
				::PostMessage((HWND)GetRenderContextSPtr()->GetHandle(), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_WALL_PROPERTY, pointer);
			}
		}
		break;
	case EWallState::EWS_MOVING:
		{

		}
		break;
	case EWallState::EWS_PROPERTY:
		{
			if ( imp_.HasSetProperty_ )
			{
				if ( imp_.Cancel_ )
				{
					imp_.State_ = EWallState::EWS_SWEEPING;
					break;
				}

				activeWall->SetHeight(imp_.EventInfo_.Height_);
				activeWall->SetThickness(imp_.EventInfo_.Thickness_);
				for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(activeWall->GetFirstCorner().lock()))
				{
					curWall->SetDirty(true);
				}
				for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(activeWall->GetSecondCorner().lock()))
				{
					curWall->SetDirty(true);
				}
				for ( auto& curWall : imp_.Graph_.lock()->GetAllWalls() )
				{
					if ( curWall->IsDirty() )
					{
						curWall->UpdateBaseMesh();
						curWall->SetDirty(false);
					}
				}
				imp_.State_ = EWallState::EWS_SWEEPING;
			}
		}
		break;
	default:
		break;
	}

	imp_.Cancel_ = false;
	imp_.LMouseLeftUp_ = false;
	imp_.LMousePressDown_ = false;
	imp_.HasSetProperty_ = false;

	return false;
}

void RoomLayoutWallCtrller::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	auto activeWall = std::static_pointer_cast<WallODL>(GetPickingODL().lock());

	activeWall->SetPicking(false);

	switch (imp_.State_)
	{
	case EWallState::EWS_SWEEPING:
		{
			activeWall->SetSweeping(false);
		}
		break;
	case EWallState::EWS_MOUSEHOLDING:
		{
			
		}
		break;
	case EWallState::EWS_MOVING:
		{

		}
		break;
	case EWallState::EWS_PROPERTY:
		{

		}
		break;
	default:
		break;
	}
}

bool RoomLayoutWallCtrller::CheckValid()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case EWallState::EWS_SWEEPING:
		{
			return false;
		}
		break;
	default:
		break;
	}

	return true;
}