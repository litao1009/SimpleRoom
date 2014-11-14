#include "stdafx.h"

#include "RoomLayoutWindowCtrller.h"
#include "RenderController/UserEvent.h"
#include "irrEngine/SRenderContext.h"

#include "ODL/WindowODL.h"
#include "ODL/WallODL.h"
#include "ODL/GraphODL.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "gp_Lin.hxx"

#include <boost/optional.hpp>

class	RoomLayoutWindowCtrller::Imp
{
public:

	enum EWindowState
	{
		EWS_NONE,
		EWS_CREATE,
		EWS_PICKING
	};

	Imp()
	{
		State_ = EWS_NONE;
		Checker_ = false;
	}

public:

	EWindowState	State_;
	WindowODLSPtr	CreatedWindow_;
	WallODLWPtr		PickingWall_;
	bool			Checker_;
	boost::optional<SEventWindowInfo>	NewInfo_;			
};

RoomLayoutWindowCtrller::RoomLayoutWindowCtrller(const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc):IRoomLayoutODLBaseCtrller(rc),ImpUPtr_(new Imp)
{
	RootODL_ = graphODL;
}

RoomLayoutWindowCtrller::~RoomLayoutWindowCtrller()
{

}

void RoomLayoutWindowCtrller::Init()
{

}

bool RoomLayoutWindowCtrller::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
	{
		CursorIPos_.X = evt.MouseInput.X;
		CursorIPos_.Y = evt.MouseInput.Y;
	}

	if ( evt.EventType == irr::EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_TEST_WINDOW )
	{
		auto doorInfo = static_cast<SEventWindowInfo*>(reinterpret_cast<void*>(evt.UserEvent.UserData2));
		imp_.NewInfo_ = *doorInfo;

	}

	switch (imp_.State_)
	{
	case Imp::EWS_NONE:
		break;
	case Imp::EWS_CREATE:
		{
			if ( evt.EventType == irr::EET_KEY_INPUT_EVENT && evt.KeyInput.Key == irr::KEY_ESCAPE && !evt.KeyInput.PressedDown )
			{
				imp_.State_ = Imp::EWS_NONE;
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
					ImpUPtr_->PickingWall_.lock()->CutHole(ImpUPtr_->CreatedWindow_);
					imp_.State_ = Imp::EWS_NONE;
					if ( ImpUPtr_->CreatedWindow_ )
					{
						ImpUPtr_->CreatedWindow_.reset();
					}
					ImpUPtr_->Checker_ = false;
					ImpUPtr_->NewInfo_ = boost::none;
				}
			}
		}
		break;
	case Imp::EWS_PICKING:
		break;
	default:
		break;
	}

	return false;
}

void RoomLayoutWindowCtrller::OnResize()
{

}

bool RoomLayoutWindowCtrller::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case Imp::EWS_NONE:
		{
			if ( !imp_.NewInfo_ )
			{
				break;
			}

			imp_.CreatedWindow_ = WindowODL::Create<WindowODL>(GetRenderContextSPtr());
			imp_.CreatedWindow_->SetHoleSize(imp_.NewInfo_->XLength_, imp_.NewInfo_->YLength_, imp_.NewInfo_->ZLength_);
			imp_.CreatedWindow_->SetOffsetSize(0, 0, 50);
			imp_.CreatedWindow_->UpdateHole();

			RootODL_.lock()->AddChild(imp_.CreatedWindow_);

			auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);

			irr::core::plane3df curPlan(0,imp_.NewInfo_->YLength_/2 + imp_.NewInfo_->SetupHeight_, 0, 0, 1, 0);
			irr::core::vector3df newPos;
			curPlan.getIntersectionWithLine(line.start, line.getVector(), newPos);
			newPos.Y = imp_.NewInfo_->YLength_/2 + imp_.NewInfo_->SetupHeight_;

			imp_.CreatedWindow_->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
			imp_.CreatedWindow_->GetDataSceneNode()->setPosition(newPos);

			imp_.State_ = Imp::EWS_CREATE;
		}
		break;
	case Imp::EWS_CREATE:
		{
			auto curZone = imp_.CreatedWindow_->GetHoleSize();
			irr::core::plane3df curPlan(0,static_cast<float>(curZone.Y()/2)+ imp_.NewInfo_->SetupHeight_, 0, 0, 1, 0);
			auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);
			irr::core::vector3df newPos;
			curPlan.getIntersectionWithLine(line.start, line.getVector(), newPos);
			newPos.Y = static_cast<float>(curZone.Y()/2)+ imp_.NewInfo_->SetupHeight_;
			gp_Pnt lineStartPnt(line.start.X, line.start.Y, line.start.Z);

			auto curEdge = BRepBuilderAPI_MakeEdge(lineStartPnt, gp_Pnt(line.end.X, line.end.Y, line.end.Z)).Edge();
			BRepAdaptor_Curve edgeAdaptor(curEdge);
			auto curGpLine = edgeAdaptor.Line();

			auto spPickingNode = imp_.PickingWall_.lock();
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

					imp_.CreatedWindow_->SetTranslation(gp_XYZ(curPnt.X(), (curZone.Y()+imp_.NewInfo_->SetupHeight_-(yMax-yMin))/2, 0));
					imp_.CreatedWindow_->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(curPnt.X()), (static_cast<float>(curZone.Y()+imp_.NewInfo_->SetupHeight_-(yMax-yMin)))/2, 0));

					return false;
				}
				else
				{
					RootODL_.lock()->AddChild(imp_.CreatedWindow_);
					imp_.PickingWall_.reset();
				}
			}

			for ( auto& wallODL : RootODL_.lock()->GetChildrenList() )
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

				imp_.CreatedWindow_->SetHoleSize(curZone.X(), curZone.Y(), static_cast<float>(std::abs(zMax-zMin)));
				imp_.CreatedWindow_->UpdateHole();
				auto transform = wallODL->GetAbsoluteTransform().Inverted();
				gp_Pnt curPnt(newPos.X, newPos.Y, newPos.Z);
				curPnt.Transform(transform);

				imp_.PickingWall_ = std::static_pointer_cast<WallODL>(wallODL);
				wallODL->AddChild(imp_.CreatedWindow_);
				imp_.CreatedWindow_->SetTranslation(gp_XYZ(curPnt.X(), curZone.Y()/2, 0));
				imp_.CreatedWindow_->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(curPnt.X()), static_cast<float>(curPnt.Y()/2), 0));

				imp_.CreatedWindow_->Set2DLineColor(irr::video::SColor(0xFF8F8F8F));
				imp_.Checker_ = true;

				return false;
			}

			imp_.CreatedWindow_->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
			imp_.CreatedWindow_->GetDataSceneNode()->setPosition(newPos);
			imp_.CreatedWindow_->Set2DLineColor(irr::video::SColor(0xFFFF0000));
			imp_.Checker_ = false;
		}
		break;
	case Imp::EWS_PICKING:
		break;
	default:
		break;
	}

	return false;
}

void RoomLayoutWindowCtrller::PostRender3D()
{

}

bool RoomLayoutWindowCtrller::PreRender2D()
{
	return false;
}

void RoomLayoutWindowCtrller::PostRender2D()
{

}