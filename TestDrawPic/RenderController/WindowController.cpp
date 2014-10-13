#include "stdafx.h"

#include "WindowController.h"

#include "irrEngine/SRenderContext.h"

#include "ODL/WindowODL.h"

#include "StatusMgr.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "gp_Lin.hxx"

class	WindowController::Imp
{
public:

	Imp()
	{
		Checker_ = false;
	}

	CWindowODLSPtr	CreatedWindow_;
	CBaseODLWPtr	PickingNode_;
	bool			Checker_;
};

WindowController::WindowController():ImpUPtr_(new Imp)
{
	State_ = EWS_NONE;
}

WindowController::~WindowController()
{

}

void WindowController::Init()
{

}

bool WindowController::OnPostEvent( const irr::SEvent& evt )
{
	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
	{
		CursorIPos_.X = evt.MouseInput.X;
		CursorIPos_.Y = evt.MouseInput.Y;
	}

	switch (State_)
	{
	case WindowController::EWS_NONE:
		break;
	case WindowController::EWS_CREATE:
		{
			if ( evt.EventType == irr::EET_KEY_INPUT_EVENT && evt.KeyInput.Key == irr::KEY_ESCAPE && !evt.KeyInput.PressedDown )
			{
				State_ = EWS_NONE;
				StatusMgr::GetInstance().PuttingState_ = StatusMgr::EPS_NONE;
				if ( ImpUPtr_->CreatedWindow_ )
				{
					ImpUPtr_->CreatedWindow_->RemoveFromParent();
					ImpUPtr_->CreatedWindow_.reset();
				}
			}

			if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP )
			{
				if ( ImpUPtr_->Checker_ )
				{
					State_ = EWS_NONE;
					StatusMgr::GetInstance().PuttingState_ = StatusMgr::EPS_NONE;
					if ( ImpUPtr_->CreatedWindow_ )
					{
						ImpUPtr_->CreatedWindow_.reset();
					}
					ImpUPtr_->Checker_ = false;
				}
			}
		}
		break;
	case WindowController::EWS_PICKING:
		break;
	default:
		break;
	}

	return false;
}

void WindowController::OnResize()
{

}

bool WindowController::PreRender3D()
{
	static irr::core::plane3df sPlane(0,0,0,0,1,0);

	if ( StatusMgr::GetInstance().PuttingState_ != StatusMgr::EPS_WINDOW )
	{
		return false;
	}

	switch (State_)
	{
	case WindowController::EWS_NONE:
		{
			ImpUPtr_->CreatedWindow_ = CWindowODL::Create<CWindowODL>(GetRenderContextSPtr());
			RootODL_.lock()->AddChild(ImpUPtr_->CreatedWindow_);

			auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);
			irr::core::vector3df newPos;
			sPlane.getIntersectionWithLine(line.start, line.getVector(), newPos);
			ImpUPtr_->CreatedWindow_->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
			ImpUPtr_->CreatedWindow_->GetDataSceneNode()->setPosition(newPos);

			State_ = EWS_CREATE;
		}
		break;
	case WindowController::EWS_CREATE:
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

					Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
					box.Get(xMin,yMin,zMin,xMax,yMax,zMax);

					ImpUPtr_->CreatedWindow_->SetTranslation(gp_XYZ(curPnt.X(), 0, 0));
					ImpUPtr_->CreatedWindow_->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(curPnt.X()), 0, 0));
					return false;
				}
				else
				{
					RootODL_.lock()->AddChild(ImpUPtr_->CreatedWindow_);
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
					ImpUPtr_->CreatedWindow_->Update2DZone(900, static_cast<float>(std::abs(zMax-zMin)));

					auto transform = wallODL->GetAbsoluteTransform().Inverted();
					gp_Pnt curPnt(newPos.X, newPos.Y, newPos.Z);
					curPnt.Transform(transform);

					ImpUPtr_->PickingNode_ = wallODL;
					wallODL->AddChild(ImpUPtr_->CreatedWindow_);
					ImpUPtr_->CreatedWindow_->SetTranslation(gp_XYZ(curPnt.X(), 0, 0));
					ImpUPtr_->CreatedWindow_->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(curPnt.X()), 0, 0));
					ImpUPtr_->CreatedWindow_->Set2DLineColor(irr::video::SColor(0xFF8F8F8F));
					ImpUPtr_->Checker_ = true;

					return false;
				}
			}

			ImpUPtr_->CreatedWindow_->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
			ImpUPtr_->CreatedWindow_->GetDataSceneNode()->setPosition(newPos);
			ImpUPtr_->CreatedWindow_->Set2DLineColor(irr::video::SColor(0xFFFF0000));
			ImpUPtr_->Checker_ = false;
		}
		break;
	case WindowController::EWS_PICKING:
		break;
	default:
		break;
	}

	return false;
}

void WindowController::PostRender3D()
{
	if ( StatusMgr::GetInstance().PuttingState_ != StatusMgr::EPS_WINDOW )
	{
		return;
	}

	if ( ImpUPtr_->CreatedWindow_ )
	{
		ImpUPtr_->CreatedWindow_->Draw2DMesh();
	}
}

bool WindowController::PreRender2D()
{
	return false;
}

void WindowController::PostRender2D()
{

}