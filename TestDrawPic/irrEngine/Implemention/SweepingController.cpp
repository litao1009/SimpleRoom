#include "stdafx.h"

#include "SweepingController.h"
#include "ODL/BaseODL.h"
#include "irrEngine/SRenderContext.h"
#include "irrEngine/irrEngine.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "Precision.hxx"

//#include <boost/timer/timer.hpp>
#include <map>

SweepingController::SweepingController()
{

}

SweepingController::~SweepingController()
{

}

bool SweepingController::OnPostEvent( const irr::SEvent& evt )
{
	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
	{
		CursorIPos_.X = evt.MouseInput.X;
		CursorIPos_.Y = evt.MouseInput.Y;
	}

	return false;
}

bool SweepingController::PreRender3D( const SRenderContext& rc )
{
	if ( !IsEnable() )
	{
		return false;
	}

	//boost::timer::cpu_timer ct;

	assert(!RootODL_.expired());

	auto smgr = GetRenderContextSPtr()->Smgr_.get();
	auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);

	auto curEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(line.start.X, line.start.Y, line.start.Z), gp_Pnt(line.end.X, line.end.Y, line.end.Z)).Edge();

	std::map<double,CBaseODLWPtr> nodes;
	auto spODL = RootODL_.lock();
	for ( auto& curODL : spODL->GetChildrenList() )
	{
		if ( curODL->GetType() == EODLT_GROUP )
		{
			for ( auto& curWall : curODL->GetChildrenList() )
			{
				if ( curWall->GetType() != EODLT_WALL )
				{
					continue;
				}

				if ( curWall->IsSelected() )
				{
					continue;
				}

				irr::core::matrix4 mat;
				auto tmpLine = line;
				auto dataSceneNode = curWall->GetDataSceneNode();
				dataSceneNode->updateAbsolutePosition();
				auto box = dataSceneNode->getBoundingBox();
				if ( !dataSceneNode->getAbsoluteTransformation().getInverse(mat) )
				{
					continue;
				}

				mat.transformVect(tmpLine.start);
				mat.transformVect(tmpLine.end);

				if ( !box.intersectsWithLine(tmpLine) )
				{
					continue;
				}

				auto& wallShape = curWall->GetBaseShape();
				auto& wallDis = curWall->GetSelector();
				if ( wallShape.IsNull() )
				{
					continue;
				}
				
				wallDis.LoadS2(curEdge);
				wallDis.Perform();
				auto dis = wallDis.Value();
				if ( dis > Precision::Confusion() )
				{
					continue;
				}

				nodes[dis] = curWall;
			}
		}
		else if ( curODL->GetType() == EODLT_DECOR )
		{

		}

	}
	
	if ( !nodes.empty() )
	{
		nodes.begin()->second.lock()->SetSwept(true);
		SweepingNode_ = nodes.begin()->second;
	}
	//IrrEngine::Dump((ct.format()+"\n").c_str());

	return false;
}

void SweepingController::PostRender3D( const SRenderContext& rc )
{
	if ( SweepingNode_.expired() )
	{
		return;
	}

	SweepingNode_.lock()->SetSwept(false);
	SweepingNode_.reset();
}

