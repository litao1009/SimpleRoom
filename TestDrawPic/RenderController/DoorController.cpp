#include "stdafx.h"

#include "DoorController.h"

#include "irrEngine/SRenderContext.h"

#include "ODL/DoorODL.h"

#include "StatusMgr.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "gp_Lin.hxx"

class	DoorController::Imp
{
public:

	Imp()
	{
		

	}

	CDoorODLSPtr	CreatedDoor_;
	CBaseODLWPtr	PickingNode_;
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
				StatusMgr::GetInstance().PutingDoor_ = true;

				ImpUPtr_->CreatedDoor_ = CDoorODL::Create<CDoorODL>(GetRenderContextSPtr());
				RootODL_.lock()->AddChild(ImpUPtr_->CreatedDoor_);

				auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);
				irr::core::vector3df newPos;
				sPlane.getIntersectionWithLine(line.start, line.getVector(), newPos);
				ImpUPtr_->CreatedDoor_->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
				ImpUPtr_->CreatedDoor_->GetDataSceneNode()->setPosition(newPos);

				State_ = EDS_CREATE;
			}
		}
		break;
	case DoorController::EDS_CREATE:
		{
			auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);
			irr::core::vector3df newPos;
			sPlane.getIntersectionWithLine(line.start, line.getVector(), newPos);

			gp_Pnt lineStartPnt(line.start.X, line.start.Y, line.start.Z);
			auto curEdge = BRepBuilderAPI_MakeEdge(lineStartPnt, gp_Pnt(line.end.X, line.end.Y, line.end.Z)).Edge();
			BRepAdaptor_Curve edgeAdaptor(curEdge);
			auto curGpLine = edgeAdaptor.Line();

			auto spPickingNode = ImpUPtr_->PickingNode_.lock();
			if ( spPickingNode )
			{
				if ( !spPickingNode->GetBaseBndBox().IsOut(curGpLine.Transformed(spPickingNode->GetAbsoluteTransform().Inverted())) )
				{
					const auto& box = spPickingNode->GetBaseBndBox();
					auto transform = spPickingNode->GetAbsoluteTransform().Inverted();
					gp_Pnt curPnt(newPos.X, newPos.Y, newPos.Z);
					curPnt.Transform(transform);

					ImpUPtr_->CreatedDoor_->SetTranslation(gp_XYZ(curPnt.X(), 0, 0));
					ImpUPtr_->CreatedDoor_->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(curPnt.X()), 0, 0));
					return false;
				}
				else
				{
					RootODL_.lock()->AddChild(ImpUPtr_->CreatedDoor_);
					ImpUPtr_->PickingNode_.reset();
				}
			}

			for ( auto& groupODL : RootODL_.lock()->GetChildrenList() )
			{
				if ( groupODL->GetType() != EODLT_GROUP )
				{
					continue;
				}

				if ( groupODL->GetBaseBndBox().IsOut(curGpLine.Transformed(groupODL->GetAbsoluteTransform().Inverted())) )
				{
					continue;
				}

				for ( auto& wallODL : groupODL->GetChildrenList() )
				{
					if ( wallODL->GetType() != EODLT_WALL )
					{
						continue;
					}

					if ( wallODL->GetBaseBndBox().IsOut(curGpLine.Transformed(wallODL->GetAbsoluteTransform().Inverted())) )
					{
						continue;
					}

					const auto& box = wallODL->GetBaseBndBox();
					Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
					box.Get(xMin,yMin,zMin,xMax,yMax,zMax);
					ImpUPtr_->CreatedDoor_->Update2DZone(900, static_cast<float>(std::abs(zMax-zMin)));

					auto transform = wallODL->GetAbsoluteTransform().Inverted();
					gp_Pnt curPnt(newPos.X, newPos.Y, newPos.Z);
					curPnt.Transform(transform);

					ImpUPtr_->PickingNode_ = wallODL;
					wallODL->AddChild(ImpUPtr_->CreatedDoor_);
					ImpUPtr_->CreatedDoor_->SetTranslation(gp_XYZ(curPnt.X(), 0, 0));
					ImpUPtr_->CreatedDoor_->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(curPnt.X()), 0, 0));
					ImpUPtr_->CreatedDoor_->Set2DLineColor(irr::video::SColor(0xFF8F8F8F));

					return false;
				}
			}

			ImpUPtr_->CreatedDoor_->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
			ImpUPtr_->CreatedDoor_->GetDataSceneNode()->setPosition(newPos);
			ImpUPtr_->CreatedDoor_->Set2DLineColor(irr::video::SColor(0xFFFF0000));
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
	if ( ImpUPtr_->CreatedDoor_ )
	{
		ImpUPtr_->CreatedDoor_->Draw2DMesh();
	}
}

bool DoorController::PreRender2D()
{
	return false;
}

void DoorController::PostRender2D()
{

}