#include "stdafx.h"

#include "RoomLayoutDoorController.h"
#include "RenderController/UserEvent.h"
#include "irrEngine/SRenderContext.h"

#include "ODL/DoorODL.h"
#include "ODL/WallODL.h"
#include "ODL/GraphODL.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "gp_Lin.hxx"

#include <boost/optional.hpp>

using namespace irr;
using namespace core;

enum class EDoorState
{
	EDS_SWEEPING,
	EDS_CREATING_INIT,
	EDS_CREATING,
	EDS_PICKING
};

class	RoomLayoutDoorController::Imp
{
public:

	Imp()
	{
		State_ = EDoorState::EDS_SWEEPING;
		Checker_ = false;
		EscapePressDown_ = false;
		LMouseLeftUp_ = false;
		LMousePressDown_ = false;
	}

public:

	bool			EscapePressDown_;
	bool			LMouseLeftUp_;
	bool			LMousePressDown_;
	bool			Checker_;
	EDoorState		State_;
	DoorODLSPtr		CreatedDoor_;
	WallODLWPtr		PickingWall_;
	vector2di		CursorIPos_;
	vector3df		CurrentPos_;

	boost::optional<SEventDoorInfo>	NewInfo_;			
};

RoomLayoutDoorController::RoomLayoutDoorController(const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc):IRoomLayoutODLBaseCtrller(rc),ImpUPtr_(new Imp)
{
	RootODL_ = graphODL;
}

RoomLayoutDoorController::~RoomLayoutDoorController()
{

}

void RoomLayoutDoorController::Init()
{

}

bool RoomLayoutDoorController::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
	{
		imp_.CursorIPos_.X = evt.MouseInput.X;
		imp_.CursorIPos_.Y = evt.MouseInput.Y;
	}

	if ( evt.EventType == irr::EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_TEST_DOOR )
	{
		auto doorInfo = static_cast<SEventDoorInfo*>(reinterpret_cast<void*>(evt.UserEvent.UserData2));
		imp_.NewInfo_ = *doorInfo;
		imp_.State_ = EDoorState::EDS_CREATING_INIT;
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


bool RoomLayoutDoorController::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	plane3df plan(0,0,0,0,1,0);
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);
	imp_.CurrentPos_.Y = 0;
	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);

	switch (imp_.State_)
	{
	case EDoorState::EDS_SWEEPING:
		{
			auto activeDoor = std::static_pointer_cast<DoorODL>(GetPickingODL().lock());
			activeDoor->SetSweeping(true);
		}
		break;
	case EDoorState::EDS_CREATING_INIT:
		{
			assert(imp_.NewInfo_);

			imp_.CreatedDoor_ = DoorODL::Create<DoorODL>(GetRenderContextSPtr());
			imp_.CreatedDoor_->SetHoleSize(imp_.NewInfo_->XLength_, imp_.NewInfo_->YLength_, imp_.NewInfo_->ZLength_);
			imp_.CreatedDoor_->SetOffsetSize(0, 0, 50);
			imp_.CreatedDoor_->UpdateHole();

			RootODL_.lock()->AddChild(imp_.CreatedDoor_);

			auto newPos = imp_.CurrentPos_;
			newPos.Y = imp_.NewInfo_->YLength_/2;

			imp_.CreatedDoor_->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
			imp_.CreatedDoor_->GetDataSceneNode()->setPosition(newPos);

			imp_.State_ = EDoorState::EDS_CREATING;
		}
		break;
	case EDoorState::EDS_CREATING:
		{
			if ( imp_.LMousePressDown_ )
			{
				if ( imp_.Checker_ )
				{
					ImpUPtr_->PickingWall_.lock()->CutHole(ImpUPtr_->CreatedDoor_);
					if ( ImpUPtr_->CreatedDoor_ )
					{
						ImpUPtr_->CreatedDoor_.reset();
					}
					ImpUPtr_->Checker_ = false;
					ImpUPtr_->NewInfo_ = boost::none;

					imp_.State_ = EDoorState::EDS_SWEEPING;
					break;
				}
			}

			if ( imp_.EscapePressDown_ )
			{
				if ( imp_.CreatedDoor_ )
				{
					imp_.CreatedDoor_->RemoveFromParent();
					imp_.CreatedDoor_.reset();
				}

				imp_.State_ = EDoorState::EDS_SWEEPING;
				break;
			}

			auto curZone = imp_.CreatedDoor_->GetHoleSize();
			
			gp_Lin cursorLin(cursorPnt, gp::DY().Reversed());

			auto spPickingNode = imp_.PickingWall_.lock();
			if ( spPickingNode )
			{
				if ( !spPickingNode->GetBaseBndBox().IsOut(cursorLin.Transformed(spPickingNode->GetAbsoluteTransform().Inverted())) )
				{
					const auto& box = spPickingNode->GetBaseBndBox();
					auto transform = spPickingNode->GetAbsoluteTransform().Inverted();
					gp_Pnt curPnt = cursorPnt;
					curPnt.Transform(transform);

					Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
					box.Get(xMin,yMin,zMin,xMax,yMax,zMax);

					imp_.CreatedDoor_->SetTranslation(gp_XYZ(curPnt.X(), (curZone.Y()-(yMax-yMin))/2, 0));
					imp_.CreatedDoor_->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(curPnt.X()), (static_cast<float>(curZone.Y()-(yMax-yMin)))/2, 0));
					
					break;
				}
				else
				{
					RootODL_.lock()->AddChild(imp_.CreatedDoor_);
					imp_.PickingWall_.reset();
				}
			}

			for ( auto& wallODL : RootODL_.lock()->GetChildrenList() )
			{
				if ( wallODL->GetType() != EODLT_WALL )
				{
					continue;
				}

				if ( wallODL->GetBaseBndBox().IsOut(cursorLin.Transformed(wallODL->GetAbsoluteTransform().Inverted())) )
				{
					continue;
				}

				const auto& box = wallODL->GetBaseBndBox();
				Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
				box.Get(xMin,yMin,zMin,xMax,yMax,zMax);

				imp_.CreatedDoor_->SetHoleSize(curZone.X(), curZone.Y(), static_cast<float>(std::abs(zMax-zMin)));
				imp_.CreatedDoor_->UpdateHole();
				auto transform = wallODL->GetAbsoluteTransform().Inverted();
				gp_Pnt curPnt = cursorPnt;
				curPnt.Transform(transform);

				imp_.PickingWall_ = std::static_pointer_cast<WallODL>(wallODL);
				wallODL->AddChild(imp_.CreatedDoor_);
				imp_.CreatedDoor_->SetTranslation(gp_XYZ(curPnt.X(), curZone.Y()/2, 0));
				imp_.CreatedDoor_->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(curPnt.X()), static_cast<float>(curPnt.Y()/2), 0));

				imp_.CreatedDoor_->Set2DLineColor(irr::video::SColor(0xFF8F8F8F));
				imp_.Checker_ = true;

				break;
			}

			if ( imp_.PickingWall_.expired() )
			{
				auto newPos = imp_.CurrentPos_;
				newPos.Y = imp_.NewInfo_->YLength_/2;
				imp_.CreatedDoor_->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
				imp_.CreatedDoor_->GetDataSceneNode()->setPosition(newPos);
				imp_.CreatedDoor_->Set2DLineColor(irr::video::SColor(0xFFFF0000));
				imp_.Checker_ = false;
			}
		}
		break;
	case EDoorState::EDS_PICKING:
		break;
	default:
		break;
	}

	imp_.LMousePressDown_ = false;
	imp_.LMouseLeftUp_ = false;
	imp_.LMousePressDown_ = false;

	return false;
}

void RoomLayoutDoorController::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case EDoorState::EDS_SWEEPING:
		{
			if ( !GetPickingODL().expired() )
			{
				auto activeDoor = std::static_pointer_cast<DoorODL>(GetPickingODL().lock());
				activeDoor->SetSweeping(false);
			}
		}
		break;
	case EDoorState::EDS_CREATING_INIT:
		{

		}
		break;
	case EDoorState::EDS_CREATING:
		{

		}
		break;
	case EDoorState::EDS_PICKING:
		{

		}
		break;
	default:
		break;
	}
}

bool RoomLayoutDoorController::CheckValid()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case EDoorState::EDS_SWEEPING:
		{
			return false;
		}
		break;
	case EDoorState::EDS_CREATING_INIT:
		{
			return true;
		}
		break;
	case EDoorState::EDS_CREATING:
		{
			return true;
		}
		break;
	case EDoorState::EDS_PICKING:
		{

		}
		break;
	default:
		break;
	}

	return false;
}
