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
		LMousePressDown_ = false;
	}

public:

	bool							LMousePressDown_;
	GraphODLWPtr					Graph_;
	vector2di						CursorIPos_;
	vector3df						CurrentPos_;
	BaseODLWPtr						SweepingODL_;
	BaseODLWPtr						PickingODL_;
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

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
	{
		imp_.LMousePressDown_ = true;
	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_MOUSE_MOVED )
	{
		imp_.CursorIPos_.X = evt.MouseInput.X;
		imp_.CursorIPos_.Y = evt.MouseInput.Y;
	}

	if ( evt.EventType == irr::EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_TEST_DOOR )
	{
		imp_.ActiveCtrller_ = imp_.CtrllerMap_[EODLT_DOOR];
	}

	if ( evt.EventType == irr::EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_TEST_WINDOW )
	{
		imp_.ActiveCtrller_ = imp_.CtrllerMap_[EODLT_WINDOW];
	}

	if ( imp_.ActiveCtrller_ )
	{
		return imp_.ActiveCtrller_->OnPostEvent(evt);
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

	if ( !imp_.SweepingODL_.expired() )
	{
		auto odl = imp_.SweepingODL_.lock();

		if ( Standard_True == odl->GetBaseBndBox().IsOut(cursorPnt.Transformed(odl->GetAbsoluteTransform().Inverted())) )
		{
			imp_.SweepingODL_.reset();
		}
		else
		{
			for ( auto& subODL : imp_.SweepingODL_.lock()->GetChildrenList() )
			{
				if ( Standard_False == subODL->GetBaseBndBox().IsOut(cursorPnt.Transformed(subODL->GetAbsoluteTransform().Inverted())) )
				{
					imp_.SweepingODL_.reset();
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
				break;
			}
		}
	}

	if ( imp_.LMousePressDown_ )
	{
		if ( !imp_.SweepingODL_.expired() )
		{
			imp_.PickingODL_ = imp_.SweepingODL_;
			imp_.ActiveCtrller_ = imp_.CtrllerMap_[imp_.PickingODL_.lock()->GetType()];
		}
	}

	if ( !imp_.SweepingODL_.expired() )
	{
		if ( imp_.SweepingODL_.lock() != imp_.PickingODL_.lock() )
		{
			imp_.SweepingODL_.lock()->SetSweeping(true);
		}
	}

	if ( !imp_.PickingODL_.expired() )
	{
		imp_.PickingODL_.lock()->SetPicking(true);
	}

	imp_.LMousePressDown_ = false;

	if ( imp_.ActiveCtrller_ )
	{
		auto ret = imp_.ActiveCtrller_->PreRender3D();
		if ( imp_.ActiveCtrller_->Valid() )
		{
			return ret;
		}
		else
		{
			imp_.ActiveCtrller_ = nullptr;
			imp_.PickingODL_.reset();
		}
	}

	return false;
}

void RoomLayoutBrowserCtrller::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	if ( !imp_.SweepingODL_.expired() )
	{
		if ( imp_.SweepingODL_.lock() != imp_.PickingODL_.lock() )
		{
			imp_.SweepingODL_.lock()->SetSweeping(false);
		}
	}

	if ( !imp_.PickingODL_.expired() )
	{
		imp_.PickingODL_.lock()->SetPicking(false);
	}

	if ( imp_.ActiveCtrller_ )
	{
		imp_.ActiveCtrller_->PostRender3D();
	}
	
}
