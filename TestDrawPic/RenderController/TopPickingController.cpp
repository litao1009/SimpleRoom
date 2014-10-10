#include "stdafx.h"

#include "TopPickingController.h"
#include "ODL/BaseODL.h"
#include "irrEngine/SRenderContext.h"
#include "irrEngine/irrEngine.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "Precision.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "gp_Lin.hxx"

#include "StatusMgr.h"

#include <map>

static irr::core::plane3df sPickingPlane(0,0,0,0,1,0);


TopPickingController::TopPickingController()
{
	Ignore_ = false;
	Picking_ = false;
}

TopPickingController::~TopPickingController()
{

}

bool TopPickingController::OnPostEvent( const irr::SEvent& evt )
{
	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
	{
		CursorIPos_.X = evt.MouseInput.X;
		CursorIPos_.Y = evt.MouseInput.Y;
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MMOUSE_PRESSED_DOWN )
	{
		Ignore_ = true;
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MMOUSE_LEFT_UP )
	{
		Ignore_ = false;
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
	{
		Picking_ = true;
		if ( StatusMgr::GetInstance().GridAlign_ )
		{
			SavePosition_ = *StatusMgr::GetInstance().GridAlign_;
		}
		else
		{
			auto saveLine = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);
			sPickingPlane.getIntersectionWithLine(saveLine.start, saveLine.getVector(), SavePosition_);
		}		
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP )
	{
		Picking_ = false;
		PickingNode_.reset();
	}

	return false;
}

bool TopPickingController::PreRender3D()
{
	if ( !IsEnable() )
	{
		return false;
	}

	if ( Ignore_ )
	{
		return false;
	}

	//boost::timer::cpu_timer ct;

	assert(!RootODL_.expired());

	auto smgr = GetRenderContextSPtr()->Smgr_.get();
	auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);

	irr::core::vector3df curPos;
	if ( StatusMgr::GetInstance().GridAlign_ )
	{
		curPos = *StatusMgr::GetInstance().GridAlign_;
	}
	else
	{
		sPickingPlane.getIntersectionWithLine(line.start, line.getVector(), curPos);
	}

	if ( Picking_ && !PickingNode_.expired() )
	{
		auto spPickingNode = PickingNode_.lock();

		auto curVec = curPos - SavePosition_;

		auto curTrans = spPickingNode->GetTranslation();
		auto curDataTrans = spPickingNode->GetDataSceneNode()->getPosition();
		spPickingNode->SetTranslation(gp_XYZ(curVec.X, curVec.Y, curVec.Z)+curTrans);
		spPickingNode->GetDataSceneNode()->setPosition(curVec+curDataTrans);
		SavePosition_ = curPos;
	}
	else
	{
		gp_Pnt lineStartPnt(line.start.X, line.start.Y, line.start.Z);
		auto curEdge = BRepBuilderAPI_MakeEdge(lineStartPnt, gp_Pnt(line.end.X, line.end.Y, line.end.Z)).Edge();
		BRepAdaptor_Curve edgeAdaptor(curEdge);
		auto curGpLine = edgeAdaptor.Line();

		std::map<double,CBaseODLWPtr> nodes;
		auto spODL = RootODL_.lock();

		for ( auto& curODL : spODL->GetChildrenList() )
		{
			if ( curODL->GetType() == EODLT_GROUP )//分组
			{

				if ( curODL->GetBaseBndBox().Transformed(curODL->GetAbsoluteTransform()).IsOut(curGpLine) )
				{
					continue;
				}

				for ( auto& allowSweeping : curODL->GetChildrenList() )
				{
					auto curAbsoluteTransform = allowSweeping->GetAbsoluteTransform();

					if ( allowSweeping->GetType() == EODLT_WALL || allowSweeping->GetType() == EODLT_FLOOR )//分组里的墙或地板
					{
						if ( allowSweeping->IsSelected() )
						{
							continue;
						}

						if ( allowSweeping->GetBaseBndBox().Transformed(curAbsoluteTransform).IsOut(curGpLine) )
						{
							continue;
						}

						auto& wallShape = allowSweeping->GetBaseShape();
						auto& wallDis = allowSweeping->GetBaseSelector();
						if ( wallShape.IsNull() )
						{
							continue;
						}

						auto transformedEdge = curEdge.Moved(allowSweeping->GetAbsoluteTransform().Inverted());
						wallDis.LoadS2(transformedEdge);
						wallDis.Perform();
						auto dis = wallDis.Value();
						if ( dis > Precision::Confusion() )
						{
							continue;
						}

						auto disSQ = wallDis.PointOnShape1(1).SquareDistance(lineStartPnt);

						nodes[disSQ] = allowSweeping;
					}
				}
			}
			else if ( curODL->GetType() == EODLT_FLOOR )//单独的地板
			{

			}

		}

		if ( nodes.empty() )
		{
			return false;
		}

		nodes.begin()->second.lock()->SetSwept(true);
		SweepingNode_ = nodes.begin()->second;

		if ( !Picking_ )
		{
			return false;
		}

		PickingNode_ = SweepingNode_.lock()->GetParent().lock();
		PickingNode_ = PickingNode_.expired() ? SweepingNode_ : PickingNode_;
	}

	return false;
}

void TopPickingController::PostRender3D()
{
	if ( SweepingNode_.expired() )
	{
		return;
	}

	SweepingNode_.lock()->SetSwept(false);
	SweepingNode_.reset();
}

