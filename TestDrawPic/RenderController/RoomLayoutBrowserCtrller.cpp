#include "stdafx.h"

#include "RoomLayoutBrowserCtrller.h"
#include "irrEngine/SRenderContext.h"

#include "ODL/GraphODL.h"
#include "ODL/CornerODL.h"
#include "ODL/WallODL.h"

#include "UserEvent.h"

#include "RenderController/IRoomLayoutODLBaseCtrller.h"
#include "RenderController/RoomLayoutDoorController.h"
#include "RenderController/RoomLayoutWindowCtrller.h"
#include "RenderController/RoomLayoutWallCtrller.h"

#include <map>

using namespace irr;
using namespace core;

class	RoomLayoutBrowserCtrller::Imp
{
public:

	Imp()
	{
		
	}

public:

	GraphODLWPtr					Graph_;
	vector2di						CursorIPos_;
	vector3df						CurrentPos_;
	BaseODLWPtr						SweepingODL_;
	IRoomLayoutODLBaseCtrllerSPtr	ActiveCtrller_;
	std::map<EObjectDisplayLayerType,IRoomLayoutODLBaseCtrllerSPtr>	CtrllerMap_;
};

RoomLayoutBrowserCtrller::RoomLayoutBrowserCtrller( const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc ):IRenderController(rc),ImpUPtr_(new Imp)
{
	ImpUPtr_->Graph_ = graphODL;
}

RoomLayoutBrowserCtrller::~RoomLayoutBrowserCtrller()
{

}

void RoomLayoutBrowserCtrller::Init()
{
	auto& imp_ = *ImpUPtr_;

	imp_.CtrllerMap_[EODLT_DOOR] = std::make_shared<RoomLayoutDoorController>(ImpUPtr_->Graph_, GetRenderContextSPtr());
	imp_.CtrllerMap_[EODLT_WINDOW] = std::make_shared<RoomLayoutWindowCtrller>(ImpUPtr_->Graph_, GetRenderContextSPtr());
	imp_.CtrllerMap_[EODLT_WALL] = std::make_shared<RoomLayoutWallCtrller>(ImpUPtr_->Graph_, GetRenderContextSPtr());
}

bool RoomLayoutBrowserCtrller::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	if ( evt.EventType == irr::EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_TEST_DOOR )
	{
		imp_.ActiveCtrller_ = imp_.CtrllerMap_[EODLT_DOOR];
		imp_.ActiveCtrller_->SetEnable(true);
	}

	if ( evt.EventType == irr::EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_TEST_WINDOW )
	{
		imp_.ActiveCtrller_ = imp_.CtrllerMap_[EODLT_WINDOW];
		imp_.ActiveCtrller_->SetEnable(true);
	}

	if ( imp_.ActiveCtrller_ && imp_.ActiveCtrller_->IsEnable() )
	{
		return imp_.ActiveCtrller_->OnPostEvent(evt);
	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
	{
		if ( imp_.ActiveCtrller_ )
		{
			imp_.ActiveCtrller_->SetEnable(true);
		}
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

	if ( imp_.ActiveCtrller_ && imp_.ActiveCtrller_->IsEnable() )
	{
		return imp_.ActiveCtrller_->PreRender3D();
	}

	plane3df plan(0,0,0,0,1,0);
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);
	imp_.CurrentPos_.Y = 0;

	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);

	if ( !imp_.SweepingODL_.expired() )
	{
		auto odl = imp_.SweepingODL_.lock();

		if ( Standard_True == odl->GetBaseBndBox().IsOut(cursorPnt.Transformed(odl->GetAbsoluteTransform().Inverted())) )
		{
			imp_.SweepingODL_.reset();
			imp_.ActiveCtrller_->ResetActiveODL();
			imp_.ActiveCtrller_ = nullptr;
		}
		else
		{
			for ( auto& subODL : imp_.SweepingODL_.lock()->GetChildrenList() )
			{
				if ( Standard_False == subODL->GetBaseBndBox().IsOut(cursorPnt.Transformed(subODL->GetAbsoluteTransform().Inverted())) )
				{
					imp_.SweepingODL_.reset();
					imp_.ActiveCtrller_->ResetActiveODL();
					imp_.ActiveCtrller_ = nullptr;
					break;
				}
			}
		}
	}

	if ( imp_.SweepingODL_.expired() )
	{
		for ( auto& curODL : imp_.Graph_.lock()->GetChildrenList() )
		{
			auto curType = curODL->GetType();
			if ( 0 == imp_.CtrllerMap_.count(curType) )
			{
				continue;
			}

			auto needBreak = false;

			for ( auto& subODL : curODL->GetChildrenList() )
			{
				if ( 0 == imp_.CtrllerMap_.count(subODL->GetType()) )
				{
					continue;
				}

				if ( Standard_False == subODL->GetBaseBndBox().IsOut(cursorPnt.Transformed(subODL->GetAbsoluteTransform().Inverted())) )
				{
					imp_.SweepingODL_ = subODL;
					imp_.ActiveCtrller_ = imp_.CtrllerMap_[subODL->GetType()];
					imp_.ActiveCtrller_->SetEnable(false);
					imp_.ActiveCtrller_->SetActiveODL(subODL);
					needBreak = true;
					break;
				}
			}
			
			if ( needBreak )
			{
				break;
			}

			if ( Standard_False == curODL->GetBaseBndBox().IsOut(cursorPnt.Transformed(curODL->GetAbsoluteTransform().Inverted())) )
			{
				imp_.SweepingODL_ = curODL;
				imp_.ActiveCtrller_ = imp_.CtrllerMap_[curType];
				imp_.ActiveCtrller_->SetEnable(false);
				imp_.ActiveCtrller_->SetActiveODL(curODL);
				break;
			}
		}
	}

	if ( !imp_.SweepingODL_.expired() )
	{
		imp_.SweepingODL_.lock()->SetSweeping(true);
	}

	return false;
}

void RoomLayoutBrowserCtrller::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	if ( imp_.ActiveCtrller_ && imp_.ActiveCtrller_->IsEnable() )
	{
		imp_.ActiveCtrller_->PostRender3D();
		return;
	}

	if ( !imp_.SweepingODL_.expired() )
	{
		imp_.SweepingODL_.lock()->SetSweeping(false);
	}
}
