#include "stdafx.h"

#include "RoomLayoutRoomController.h"
#include "RenderController/UserEvent.h"
#include "irrEngine/SRenderContext.h"

#include "ODL/RoomODL.h"
#include "ODL/WallODL.h"
#include "ODL/GraphODL.h"

#include <boost/optional.hpp>

using namespace irr;
using namespace core;

enum class ERoomState
{
	ERS_SWEEPING,
	ERS_PROPERTY,
	ERS_COUNT
};

class	RoomLayoutRoomController::Imp
{
public:

	Imp()
	{
		State_ = ERoomState::ERS_SWEEPING;
		EscapePressDown_ = false;
		LMouseLeftUp_ = false;
		LMousePressDown_ = false;
	}

public:

	bool			EscapePressDown_;
	bool			LMouseLeftUp_;
	bool			LMousePressDown_;
	ERoomState		State_;
	vector2di		CursorIPos_;
	vector3df		CurrentPos_;
	vector3df		SavePos_;

	boost::optional<EUserType>		PropertyCallBack_;
	boost::optional<SEventRoomInfo>	EventInfo_;			
};

RoomLayoutRoomController::RoomLayoutRoomController(const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc):IRoomLayoutODLBaseCtrller(rc),ImpUPtr_(new Imp)
{
	RootODL_ = graphODL;
}

RoomLayoutRoomController::~RoomLayoutRoomController()
{

}

void RoomLayoutRoomController::Init()
{

}

bool RoomLayoutRoomController::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	if ( evt.EventType == EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_ROOM_PROPERTY )
	{
		imp_.PropertyCallBack_ = static_cast<EUserType>(evt.UserEvent.UserData2);
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
	{
		imp_.CursorIPos_.X = evt.MouseInput.X;
		imp_.CursorIPos_.Y = evt.MouseInput.Y;
	}

	if ( evt.EventType == irr::EET_KEY_INPUT_EVENT && evt.KeyInput.Key == irr::KEY_ESCAPE && evt.KeyInput.PressedDown )
	{
		imp_.EscapePressDown_ = true;
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP )
	{
		imp_.LMouseLeftUp_ = true;
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
	{
		imp_.LMousePressDown_ = true;;
	}

	return false;
}


bool RoomLayoutRoomController::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	plane3df plan(0,0,0,0,1,0);
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);
	imp_.CurrentPos_.Y = 0;
	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);

	switch (imp_.State_)
	{
	case ERoomState::ERS_SWEEPING:
		{
			auto activeRoom = std::static_pointer_cast<RoomODL>(GetPickingODL().lock());
			auto& center = activeRoom->GetCenter();

			if ( cursorPnt.Distance(center) > 200.f )
			{
				break;
			}

			activeRoom->SetSweeping(true);

			if ( imp_.LMousePressDown_ )
			{
				imp_.EventInfo_ = SEventRoomInfo();
				imp_.EventInfo_->Name_ = activeRoom->GetName();

				imp_.SavePos_ = imp_.CurrentPos_;
				imp_.PropertyCallBack_ = boost::none;
				imp_.State_ = ERoomState::ERS_PROPERTY;

				auto pointer = reinterpret_cast<int>(static_cast<void*>(&(*imp_.EventInfo_)));
				::PostMessage((HWND)GetRenderContextSPtr()->GetHandle(), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_ROOM_PROPERTY, pointer);
			}
		}
		break;
	case ERoomState::ERS_PROPERTY:
		{
			if ( !imp_.PropertyCallBack_ )
			{
				break;
			}

			auto activeRoom = std::static_pointer_cast<RoomODL>(GetPickingODL().lock());

			switch (*(imp_.PropertyCallBack_))
			{
			case EUT_ROOMLAYOUT_ROOM_NONE:
				{
					imp_.State_ = ERoomState::ERS_SWEEPING;
				}
				break;
			case EUT_ROOMLAYOUT_ROOM_UPDATE:
				{
					activeRoom->SetName(imp_.EventInfo_->Name_);
					imp_.State_ = ERoomState::ERS_SWEEPING;
				}
				break;
			default:
				assert(0);
				break;
			}
		}
		break;
	default:
		break;
	}

	imp_.LMousePressDown_ = false;
	imp_.LMouseLeftUp_ = false;
	imp_.LMousePressDown_ = false;

	return false;
}

void RoomLayoutRoomController::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case ERoomState::ERS_SWEEPING:
		{
			if ( !GetPickingODL().expired() )
			{
				auto activeRoom = std::static_pointer_cast<RoomODL>(GetPickingODL().lock());
				activeRoom->SetSweeping(false);
			}
		}
		break;
	default:
		{
			auto activeRoom = std::static_pointer_cast<RoomODL>(GetPickingODL().lock());
			activeRoom->SetPicking(false);
		}
		break;
	}
}

bool RoomLayoutRoomController::CheckValid()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case ERoomState::ERS_SWEEPING:
		{
			return false;
		}
		break;
	default:
		return true;
		break;
	}

	return false;
}
