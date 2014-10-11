#include "stdafx.h"

#include "DoorController.h"

#include "irrEngine/SRenderContext.h"

#include "ODL/DoorODL.h"

#include "StatusMgr.h"

#include "ODL/MeshSceneNode/DoorMeshNode2D.h"

class	DoorController::Imp
{
public:

	Imp()
	{
		DoorMesh_ = nullptr;
	}

	DoorMeshNode2D*	DoorMesh_;
};

DoorController::DoorController():ImpUPtr_(new Imp)
{
	State_ = EDS_NONE;
}

DoorController::~DoorController()
{

}

void DoorController::Init()
{

}

bool DoorController::OnPostEvent( const irr::SEvent& evt )
{
	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
	{
		CursorIPos_.X = evt.MouseInput.X;
		CursorIPos_.Y = evt.MouseInput.Y;
	}

	return false;
}

void DoorController::OnResize()
{

}

bool DoorController::PreRender3D()
{
	static irr::core::plane3df sPlane(0,0,0,0,1,0);

	switch (State_)
	{
	case DoorController::EDS_NONE:
		{
			if ( StatusMgr::GetInstance().Test_CreateDoor_ )
			{
				StatusMgr::GetInstance().Test_CreateDoor_ = false;

				ImpUPtr_->DoorMesh_ = new DoorMeshNode2D(RootODL_.lock()->GetDataSceneNode().get());
				ImpUPtr_->DoorMesh_->UpdateMesh(900, 300);

				State_ = EDS_CREATE;
			}
		}
		break;
	case DoorController::EDS_CREATE:
		{
			auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);
			irr::core::vector3df newPos;
			sPlane.getIntersectionWithLine(line.start, line.getVector(), newPos);

			ImpUPtr_->DoorMesh_->setPosition(newPos);
		}
		break;
	case DoorController::EDS_PICKING:
		break;
	default:
		break;
	}

	return false;
}

void DoorController::PostRender3D()
{

}

bool DoorController::PreRender2D()
{
	return false;
}

void DoorController::PostRender2D()
{

}