#include "stdafx.h"

#include "RoomLayoutPillarController.h"
#include "RenderController/UserEvent.h"
#include "irrEngine/SRenderContext.h"

#include "ODL/BaseODL.h"
#include "ODL/PillarODL.h"
#include "ODL/RoomODL.h"
#include "ODL/WallODL.h"

#include "gp_Lin.hxx"
#include "BRepExtrema_DistShapeShape.hxx"

#include <boost/optional.hpp>

using namespace irr;
using namespace core;

enum class EPilarState
{
	EPS_SWEEPING,
	EPS_CREATING_INIT,
	EPS_MOUSEHOLDING,
	EPS_MOVING,
	EPS_PROPERTY,
	EPS_COUNT
};

class	RoomLayoutPillarController::Imp
{
public:

	Imp()
	{
		State_ = EPilarState::EPS_SWEEPING;
		EscapePressDown_ = false;
		LMouseLeftUp_ = false;
		LMousePressDown_ = false;
		Valid_ = false;
	}

public:

	bool			EscapePressDown_;
	bool			LMouseLeftUp_;
	bool			LMousePressDown_;
	bool			Valid_;
	EPilarState		State_;
	vector2di		CursorIPos_;
	vector3df		CurrentPos_;
	vector3df		SavePos_;
	RoomODLWPtr		PickingRoom_;

	boost::optional<EUserType>			PropertyCallBack_;
	boost::optional<SEventPilarInfo>	EventInfo_;			
};

RoomLayoutPillarController::RoomLayoutPillarController(const BaseODLSPtr& baseODL, const SRenderContextWPtr& rc):IRoomLayoutODLBaseCtrller(rc),ImpUPtr_(new Imp)
{
	RootODL_ = baseODL;
}

RoomLayoutPillarController::~RoomLayoutPillarController()
{

}

void RoomLayoutPillarController::Init()
{

}

bool RoomLayoutPillarController::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	if ( evt.EventType == EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_PILLAR_PROPERTY )
	{
		imp_.PropertyCallBack_ = static_cast<EUserType>(evt.UserEvent.UserData2);
	}

	if ( evt.EventType == irr::EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_TEST_PILLAR )
	{
		auto pilarInfo = static_cast<SEventPilarInfo*>(reinterpret_cast<void*>(evt.UserEvent.UserData2));
		imp_.EventInfo_ = *pilarInfo;
		imp_.State_ = EPilarState::EPS_CREATING_INIT;
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


bool RoomLayoutPillarController::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	plane3df plan(0,0,0,0,1,0);
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);
	imp_.CurrentPos_.Y = 0;
	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);

	switch (imp_.State_)
	{
	case EPilarState::EPS_SWEEPING:
		{
			auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());

			activePilar->SetSweeping(true);

			if ( imp_.LMousePressDown_ )
			{
				auto box = activePilar->GetBaseBndBox();
				auto pos = activePilar->GetTranslation();

				Standard_Real xMin,xMax,yMin,yMax,zMin,zMax;
				box.Get(xMin, yMin, zMin, xMax, yMax, zMax);

				imp_.EventInfo_ = SEventPilarInfo();
				imp_.EventInfo_->XLength_ = static_cast<float>(xMax-xMin);
				imp_.EventInfo_->YLength_ = static_cast<float>(yMax-yMin);
				imp_.EventInfo_->ZLength_ = static_cast<float>(zMax-zMin);
				imp_.EventInfo_->OffsetHeight_ = activePilar->GetOffsetHeight();

				imp_.SavePos_ = imp_.CurrentPos_;
				imp_.State_ = EPilarState::EPS_PROPERTY;

				auto pointer = reinterpret_cast<int>(static_cast<void*>(&(*imp_.EventInfo_)));
				::PostMessage((HWND)GetRenderContextSPtr()->GetHandle(), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_PILLAR_PROPERTY, pointer);
			}
		}
		break;
	case EPilarState::EPS_PROPERTY:
		{
			if ( !imp_.PropertyCallBack_ )
			{
				break;
			}

			auto activeRoom = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());

			switch (*(imp_.PropertyCallBack_))
			{
			case EUT_ROOMLAYOUT_ROOM_NONE:
				{
					imp_.State_ = EPilarState::EPS_SWEEPING;
				}
				break;
			case EUT_ROOMLAYOUT_ROOM_UPDATE:
				{
					imp_.State_ = EPilarState::EPS_SWEEPING;
				}
				break;
			default:
				assert(0);
				break;
			}
		}
		break;
	case EPilarState::EPS_CREATING_INIT:
		{
			assert(imp_.EventInfo_);

			auto newPilar = PillarODL::Create<PillarODL>(GetRenderContextSPtr());
			newPilar->SetSize(imp_.EventInfo_->XLength_, imp_.EventInfo_->YLength_, imp_.EventInfo_->ZLength_);
			newPilar->SetOffsetHeight(imp_.EventInfo_->OffsetHeight_);
			newPilar->UpdateMesh();

			RootODL_.lock()->AddChild(newPilar);

			auto newPos = imp_.CurrentPos_;
			newPos.Y = imp_.EventInfo_->YLength_/2;

			newPilar->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
			newPilar->GetDataSceneNode()->setPosition(newPos);
			SetPickingODL(newPilar);
			if ( !imp_.PickingRoom_.expired() )
			{
				imp_.PickingRoom_.reset();
			}
			imp_.State_ = EPilarState::EPS_MOVING;
		}
		break;
	case EPilarState::EPS_MOVING:
		{
			auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
			activePilar->SetPicking(true);

			if ( imp_.LMouseLeftUp_ )
			{
				if ( imp_.Valid_ )
				{
					ImpUPtr_->Valid_ = false;
					ImpUPtr_->EventInfo_ = boost::none;

					imp_.State_ = EPilarState::EPS_SWEEPING;
					break;
				}
			}

			if ( imp_.EscapePressDown_ )
			{
				activePilar->RemoveFromParent();

				imp_.State_ = EPilarState::EPS_SWEEPING;
				break;
			}

			gp_Lin cursorLin(cursorPnt, gp::DY().Reversed());

			auto newPos = imp_.CurrentPos_;
			newPos.Y = imp_.EventInfo_->YLength_/2;

			activePilar->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
			activePilar->GetDataSceneNode()->setPosition(newPos);

			auto& curSize = activePilar->GetSize();

			std::vector<WallODLSPtr>	alignWalls;
			auto alignDis = 100.0;

			for ( auto& curWall : RootODL_.lock()->GetChildrenList() )
			{
				if ( EODLT_WALL != curWall->GetType() )
				{
					continue;
				}

				auto wall = std::static_pointer_cast<WallODL>(curWall);

				Bnd_Box curLinBox;
				curLinBox.Add(cursorPnt, gp::DY());

				if ( wall->GetBaseBndBox().Distance(activePilar->GetBaseBndBox().Transformed(wall->GetAbsoluteTransform().Inverted())) > alignDis )
				{
					continue;
				}

				BRepExtrema_DistShapeShape dss(wall->GetBaseShape().Moved(wall->GetAbsoluteTransform()), activePilar->GetBaseShape().Moved(activePilar->GetAbsoluteTransform()));
				if ( 0 != dss.NbSolution() && dss.Value() < alignDis )
				{
					alignWalls.push_back(wall);
				}
			}

			if ( alignWalls.empty() )
			{
				imp_.Valid_ = true;
				break;
			}

			for ( auto& curWall : alignWalls )
			{

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

void RoomLayoutPillarController::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case EPilarState::EPS_SWEEPING:
		{
			if ( !GetPickingODL().expired() )
			{
				auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
				activePilar->SetSweeping(false);
			}
		}
		break;
	default:
		{
			auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
			activePilar->SetPicking(false);
		}
		break;
	}
}

bool RoomLayoutPillarController::CheckValid()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case EPilarState::EPS_SWEEPING:
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
