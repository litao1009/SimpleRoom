#include "stdafx.h"

#include "TestDrawRoom.h"
#include "SMeshBuffer.h"
#include "irrEngine/SRenderContext.h"
#include "RenderController/FloorPlan/GraphController.h"
#include "RenderController/FloorPlan/Corner.h"
#include "RenderController/FloorPlan/Wall.h"
#include "StatusMgr.h"

#include "TopExp.hxx"
#include "TopTools_IndexedMapOfShape.hxx"
#include "TopTools_IndexedDataMapOfShapeListOfShape.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "TopoDS_Shape.hxx"
#include "gp_Pnt.hxx"
#include "BRepTools_WireExplorer.hxx"
#include "TopoDS.hxx"
#include "BRep_Tool.hxx"
#include "BRepExtrema_ExtPC.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"
#include "gp_Pln.hxx"
#include "GeomAPI.hxx"
#include "Geom2dAPI_InterCurveCurve.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepExtrema_ExtCC.hxx"

#include <vector>


using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

enum EState
{
	ES_READY,
	ES_DRAWING,
	ES_COUNT
};

class	TestDrawRoomCtrller::Imp
{
public:

	Imp()
	{
		State_ = ES_READY;
		TestRoomLine_ = new SMeshBuffer;
		TestRoomLine_->getMaterial().Lighting = false;
		TestRoomLine_->getMaterial().BackfaceCulling = false;
		TestRoomLine_->getMaterial().Thickness = 4;
		TestRoomLine_->getMaterial().ZWriteEnable = false;

		FloatingLine_ = new SMeshBuffer;
		FloatingLine_->getMaterial().Lighting = false;
		FloatingLine_->getMaterial().BackfaceCulling = false;
		FloatingLine_->getMaterial().ZWriteEnable = false;

		PressDown_ = false;
		Finish_ = false;
	}

	~Imp()
	{
		if ( TestRoomLine_ )
		{
			TestRoomLine_->drop();
		}

		if ( FloatingLine_ )
		{
			FloatingLine_->drop();
		}
	}

public:

	EState						State_;
	SMeshBuffer*				TestRoomLine_;
	SMeshBuffer*				FloatingLine_;
	SMeshBuffer*				PositionRect_;

	vector3df					CurrentPos_;
	vector2di					CursorIPos_;
	bool						PressDown_;
	bool						Finish_;
};

TestDrawRoomCtrller::TestDrawRoomCtrller():ImpUPtr_(new Imp)
{

}

TestDrawRoomCtrller::~TestDrawRoomCtrller()
{

}

bool TestDrawRoomCtrller::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	plane3df plan(0,0,0,0,1,0);

	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);

	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);
	auto alignDistance = 200.f;

	auto foundPosition = false;
	CornerSPtr alignConer;

	for ( auto& curCorner : GraphController::GetInstance().GetAllCorners() )
	{
		if ( curCorner->GetPosition().SquareDistance(cursorPnt) < alignDistance * alignDistance )
		{
			cursorPnt = curCorner->GetPosition();
			foundPosition = true;
			alignConer = curCorner;

			break;
		}
	}
	
	BRepAdaptor_Curve firstCurve,secondCurve;
	if ( !foundPosition )
	{
		gp_Pnt firstPnt;
		auto foundLinesCount = 0;
		for ( auto& curWall : GraphController::GetInstance().GetAllWalls() )
		{
			if ( curWall->IsBezierCurve() )
			{
				continue;
			}

			auto curEdge = curWall->GetEdge();
			BRepAdaptor_Curve bc(curEdge);
			GeomAPI_ProjectPointOnCurve ppc(cursorPnt, bc.Curve().Curve());
			if ( ppc.LowerDistance() > alignDistance )
			{
				continue;
			}

			if ( 0 == foundLinesCount )
			{
				firstCurve = bc;
				bc.D0(ppc.LowerDistanceParameter(), firstPnt);
				++foundLinesCount;
			}
			else
			{
				if ( bc.Line().Direction().IsParallel(firstCurve.Line().Direction(), Precision::Confusion()) )
				{
					continue;
				}

				gp_Pln pln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));
				
				auto firstline2D = GeomAPI::To2d(firstCurve.Curve().Curve(), pln);
				auto secondline2D = GeomAPI::To2d(bc.Curve().Curve(), pln);

				Geom2dAPI_InterCurveCurve icc(firstline2D, secondline2D);
				if ( 0 == icc.NbPoints() )
				{
					continue;
				}

				assert(icc.NbPoints() == 1);

				auto iccPnt = icc.Point(1);
				gp_Pnt iccPnt3D(iccPnt.X(), 0, iccPnt.Y());

				if ( iccPnt3D.SquareDistance(cursorPnt) > alignDistance * alignDistance )
				{
					continue;
				}

				cursorPnt = iccPnt3D;
				secondCurve = bc;
				++foundLinesCount;

				break;
			}
		}

		if ( 0 != foundLinesCount )
		{
			foundPosition = true;
		}

		if ( 1 == foundLinesCount )
		{
			cursorPnt = firstPnt;
		}
	}

	if ( !foundPosition && StatusMgr::GetInstance().GridAlign_ )
	{
		imp_.CurrentPos_ = *StatusMgr::GetInstance().GridAlign_;
		cursorPnt = gp_Pnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);
	}

	switch (imp_.State_)
	{
	case EState::ES_READY:
		{
			if ( imp_.PressDown_ )
			{
				imp_.PressDown_ = false;
				if ( alignConer )
				{
					GraphController::GetInstance().AddCornerToTempPath(alignConer);
				}
				else
				{
					GraphController::GetInstance().AddCornerToTempPath(GraphController::GetInstance().CreateCorner(cursorPnt));
				}

				imp_.State_ = EState::ES_DRAWING;
			}
		}
		break;
	case EState::ES_DRAWING:
		{
			bool valid = true;
			{//checkValid

				//目前的路径
				auto pathCorners = GraphController::GetInstance().GetTempPathCorners();

				//与最后一个点重合
				auto lastCorner = pathCorners.back();
				if ( lastCorner == alignConer )
				{
					valid = false;
				}

				//目前的边
				auto floatingEdge = BRepBuilderAPI_MakeEdge(lastCorner->GetPosition(), cursorPnt).Edge();
				BRepAdaptor_Curve floatingBC(floatingEdge);

				//与上一条边的夹角小于15度
				if ( valid )
				{
					if ( pathCorners.size() >= 2 )
					{
						auto lastCorner = pathCorners.back();
						for ( auto& curWall : GraphController::GetInstance().GetWallsOnCorner(lastCorner) )
						{
							auto curWallEdge = curWall->GetEdge();
							BRepAdaptor_Curve wallBC(curWallEdge);

							auto curAngle = floatingBC.Line().Direction().Angle(wallBC.Line().Direction());
							if ( curAngle > M_PI_2 )
							{
								curAngle = M_PI - curAngle;
							}

							static auto rad15 = 15 * M_PI / 180.f;

							if ( curAngle < rad15 )
							{
								valid = false;
								break;
							}

						}
					}
				}

				//是否有交叉
				if ( valid )
				{
					//目前的边
					auto floatingEdge = BRepBuilderAPI_MakeEdge(lastCorner->GetPosition(), cursorPnt).Edge();
					BRepAdaptor_Curve floatingBC(floatingEdge);

					BRepExtrema_ExtCC becc;
					becc.Initialize(floatingEdge);
					Standard_Real firstP,lastP;
					BRep_Tool::Range(floatingEdge, firstP, lastP);

					for ( auto& curWall : GraphController::GetInstance().GetAllWalls() )
					{
						auto wallEdge = curWall->GetEdge();

						becc.Perform(wallEdge);

						if ( 0 == becc.NbExt() )
						{
							continue;
						}

						Standard_Real firstCurP,lastCurP;
						BRep_Tool::Range(wallEdge, firstCurP, lastCurP);

						auto par1 = becc.ParameterOnE1(1);
						auto par2 = becc.ParameterOnE2(1);

						if ( par1 > firstP && par1 < lastP && par2 > firstCurP && par2 < lastCurP )
						{
							valid = false;
							break;
						}
					}
				}
			}

			if ( imp_.PressDown_ )
			{
				imp_.PressDown_ = false;

				if ( valid )
				{
					if ( alignConer )
					{
						GraphController::GetInstance().AddCornerToTempPath(alignConer);
					}
					else
					{
						GraphController::GetInstance().AddCornerToTempPath(GraphController::GetInstance().CreateCorner(cursorPnt));
					}
				}
			}

			if ( imp_.Finish_ )
			{
				if ( GraphController::GetInstance().UpdatePath() )
				{

				}

				imp_.PressDown_ = false;
				imp_.Finish_ = false;
				imp_.State_ = EState::ES_READY;
			}
		}
		break;
	default:
		break;
	}


	return false;
}

void TestDrawRoomCtrller::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	auto driver = GetRenderContextSPtr()->Smgr_->getVideoDriver();

	driver->setTransform(ETS_WORLD, matrix4());
	driver->setMaterial(imp_.TestRoomLine_->getMaterial());
	driver->drawVertexPrimitiveList(imp_.TestRoomLine_->getVertices(), imp_.TestRoomLine_->getVertexCount(), imp_.TestRoomLine_->getIndices(), imp_.TestRoomLine_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);


	driver->drawVertexPrimitiveList(imp_.FloatingLine_->getVertices(), imp_.FloatingLine_->getVertexCount(), imp_.FloatingLine_->getIndices(), imp_.FloatingLine_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
}

bool TestDrawRoomCtrller::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT )
	{
		if ( evt.MouseInput.Event == EMIE_MOUSE_MOVED )
		{
			imp_.CursorIPos_.X = evt.MouseInput.X;
			imp_.CursorIPos_.Y = evt.MouseInput.Y;
		}

		if ( evt.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
		{
			imp_.PressDown_ = true;
		}
	}

	if ( evt.EventType == EET_KEY_INPUT_EVENT )
	{
		if ( evt.KeyInput.Key == KEY_ESCAPE )
		{
			imp_.Finish_ = true;
		}
	}

	return false;
}

void TestDrawRoomCtrller::Init()
{
	auto& imp_ = *ImpUPtr_;
}
