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
		TestRoomLine_->getMaterial().Thickness = 2;
		TestRoomLine_->getMaterial().ZWriteEnable = false;

		PositionRect_ = new SMeshBuffer;
		PositionRect_->getMaterial().Lighting = false;
		PositionRect_->getMaterial().BackfaceCulling = false;
		PositionRect_->getMaterial().ZWriteEnable = false;
		PositionRect_->getMaterial().MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

		FloatingLine_ = new SMeshBuffer;
		FloatingLine_->getMaterial().Lighting = false;
		FloatingLine_->getMaterial().BackfaceCulling = false;
		FloatingLine_->getMaterial().ZWriteEnable = false;
		FloatingLine_->getMaterial().Thickness = 4;

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

		if ( PositionRect_ )
		{
			PositionRect_->drop();
		}
	}

public:

	EState						State_;

	//画好的线
	SMeshBuffer*				TestRoomLine_;
	
	//目前的线
	SMeshBuffer*				FloatingLine_;

	//目前的点
	SMeshBuffer*				PositionRect_;
	matrix4						PositionRectScaleMat_;
	matrix4						PositionRectTransMat_;

	CornerSPtr					BeginCorner_;
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
	vector3df planPos;
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), planPos);

	gp_Pnt cursorPnt(planPos.X, planPos.Y, planPos.Z);
	auto alignDistance = 200.f;

	CornerSPtr foundCorner;

	for ( auto& curCorner : GraphController::GetInstance().GetAllCorners() )
	{
		if ( curCorner->GetPosition().SquareDistance(cursorPnt) < alignDistance * alignDistance )
		{
			cursorPnt = curCorner->GetPosition();
			foundCorner = curCorner;

			break;
		}
	}
	
	//在边延长线上
	auto foundPosWithLine = false;
	//在边上
	auto foundPosInEdge = false;

	auto foundLinesCount = 0;

	BRepAdaptor_Curve firstCurve,secondCurve;
	WallSPtr firstWall,secondWall;
	Standard_Real firstPar,secondPar;
	WallSPtr toSplit;

	if ( !foundCorner )
	{		
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
				firstPar = ppc.LowerDistanceParameter();
				firstWall = curWall;

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

				secondCurve = bc;

				GeomAPI_ProjectPointOnCurve ppc1(iccPnt3D, firstCurve.Curve().Curve());
				firstPar = ppc1.LowerDistanceParameter();

				GeomAPI_ProjectPointOnCurve ppc2(iccPnt3D, secondCurve.Curve().Curve());
				secondPar = ppc2.LowerDistanceParameter();

				secondWall = curWall;
				
				++foundLinesCount;

				break;
			}
		}

		if ( 0 != foundLinesCount )
		{
			foundPosWithLine = true;

			firstCurve.D0(firstPar, cursorPnt);

			auto wallThick = 200.f;

			if ( firstPar > firstCurve.FirstParameter() + wallThick && firstPar < firstCurve.LastParameter() - wallThick )
			{
				toSplit = firstWall;
			}
			if ( 2 == foundLinesCount && secondPar > secondCurve.FirstParameter() + wallThick && secondPar < secondCurve.LastParameter() - wallThick )
			{
				toSplit = secondWall;
			}
		}
	}

	if ( !foundCorner && 0 == foundLinesCount && StatusMgr::GetInstance().GridAlign_ )
	{
		auto pos = *StatusMgr::GetInstance().GridAlign_;
		cursorPnt = gp_Pnt(pos.X, pos.Y, pos.Z);
	}

	auto needUpdateMesh = false;

	switch (imp_.State_)
	{
	case EState::ES_READY:
		{
			if ( imp_.PressDown_ )
			{
				imp_.PressDown_ = false;
				if ( foundCorner )
				{
					imp_.BeginCorner_ = foundCorner;
				}
				else
				{
					if ( toSplit )
					{
						imp_.BeginCorner_ = GraphController::GetInstance().CreateCornerBySplitWall(toSplit, cursorPnt);
						needUpdateMesh = true;
					}
					else
					{
						imp_.BeginCorner_ = GraphController::GetInstance().CreateCorner(cursorPnt);
					}
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
				auto pathCorners = GraphController::GetInstance().GetAllCorners();

				//与起始点点重合
				if ( imp_.BeginCorner_ == foundCorner )
				{
					valid = false;
				}
				else
				{
					//目前的边
					auto floatingEdge = BRepBuilderAPI_MakeEdge(imp_.BeginCorner_->GetPosition(), cursorPnt).Edge();
					BRepAdaptor_Curve floatingBC(floatingEdge);

					//与边的夹角小于15度
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
									GeomAPI_ProjectPointOnCurve ppc(cursorPnt, wallBC.Curve().Curve());

									auto par = ppc.LowerDistanceParameter();
									if ( par < wallBC.FirstParameter() || par > wallBC.LastParameter() )
									{
										continue;
									}

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
						auto floatingEdge = BRepBuilderAPI_MakeEdge(imp_.BeginCorner_->GetPosition(), cursorPnt).Edge();
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
			}

			if ( imp_.PressDown_ )
			{
				imp_.PressDown_ = false;

				if ( valid )
				{
					if ( foundCorner )
					{
						GraphController::GetInstance().AddWall(imp_.BeginCorner_, foundCorner);
						imp_.BeginCorner_ = foundCorner;
					}
					else
					{
						if ( toSplit )
						{
							auto newCorner = GraphController::GetInstance().CreateCornerBySplitWall(toSplit, cursorPnt);
							GraphController::GetInstance().AddWall(imp_.BeginCorner_, newCorner);
							imp_.BeginCorner_ = newCorner;
						}
						else
						{
							auto newCorner = GraphController::GetInstance().CreateCorner(cursorPnt);
							GraphController::GetInstance().AddWall(imp_.BeginCorner_, newCorner);
							imp_.BeginCorner_ = newCorner;
						}
					}

					needUpdateMesh = true;
				}
			}

			if ( imp_.Finish_ )
			{
				//imp_.PressDown_ = false;
				imp_.Finish_ = false;

				auto walls = GraphController::GetInstance().GetWallsOnCorner(imp_.BeginCorner_);
				if ( walls.empty() )
				{
					GraphController::GetInstance().RemoveCorner(imp_.BeginCorner_);
				}
				imp_.BeginCorner_ = nullptr;

				imp_.State_ = EState::ES_READY;
			}
		}
		break;
	default:
		break;
	}

	if ( needUpdateMesh )
	{
		auto allWalls = GraphController::GetInstance().GetAllWalls();

		imp_.TestRoomLine_->Vertices.clear();
		imp_.TestRoomLine_->Vertices.reallocate(allWalls.size()*6);
		imp_.TestRoomLine_->Indices.clear();
		imp_.TestRoomLine_->Indices.reallocate(allWalls.size()*6*4);

		for ( auto& curWall : allWalls )
		{
			curWall->UpdateMesh();

			auto& meshPoints = curWall->GetMeshPoints();

			using namespace irr;
			using namespace video;
			using namespace core;

			vector3df normal(0,1,0);
			SColor clr(0xFF000000);
			vector2df coord(0,0);

			auto curVertexSize = imp_.TestRoomLine_->Vertices.size();

			imp_.TestRoomLine_->Vertices.push_back(S3DVertex(vector3df(static_cast<float>(meshPoints[0].X()), static_cast<float>(meshPoints[0].Y()), static_cast<float>(meshPoints[0].Z())), normal, clr, coord));
			imp_.TestRoomLine_->Vertices.push_back(S3DVertex(vector3df(static_cast<float>(meshPoints[1].X()), static_cast<float>(meshPoints[1].Y()), static_cast<float>(meshPoints[1].Z())), normal, clr, coord));
			imp_.TestRoomLine_->Vertices.push_back(S3DVertex(vector3df(static_cast<float>(meshPoints[2].X()), static_cast<float>(meshPoints[2].Y()), static_cast<float>(meshPoints[2].Z())), normal, clr, coord));
			imp_.TestRoomLine_->Vertices.push_back(S3DVertex(vector3df(static_cast<float>(meshPoints[3].X()), static_cast<float>(meshPoints[3].Y()), static_cast<float>(meshPoints[3].Z())), normal, clr, coord));
			imp_.TestRoomLine_->Vertices.push_back(S3DVertex(vector3df(static_cast<float>(meshPoints[4].X()), static_cast<float>(meshPoints[4].Y()), static_cast<float>(meshPoints[4].Z())), normal, clr, coord));
			imp_.TestRoomLine_->Vertices.push_back(S3DVertex(vector3df(static_cast<float>(meshPoints[5].X()), static_cast<float>(meshPoints[5].Y()), static_cast<float>(meshPoints[5].Z())), normal, clr, coord));

// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize);
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize + 1);
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize + 2);
// 
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize);
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize + 2);
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize + 3);
// 
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize);
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize + 3);
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize + 4);
// 
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize);
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize + 4);
// 			imp_.TestRoomLine_->Indices.push_back(curVertexSize + 5);

			imp_.TestRoomLine_->Indices.push_back(curVertexSize);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+1);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+1);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+2);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+2);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+3);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+3);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+4);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+4);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+5);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+5);
			imp_.TestRoomLine_->Indices.push_back(curVertexSize+0);
		}
	}

	{
		vector3df center(static_cast<float>(cursorPnt.X()), static_cast<float>(cursorPnt.Y()), static_cast<float>(cursorPnt.Z()));
		imp_.PositionRectTransMat_.setTranslation(center);
	}

	if ( imp_.BeginCorner_ )
	{
		auto beginPnt = imp_.BeginCorner_->GetPosition();
		vector3df beginVec(static_cast<float>(beginPnt.X()), static_cast<float>(beginPnt.Y()), static_cast<float>(beginPnt.Z()));
		vector3df curVec(static_cast<float>(cursorPnt.X()), static_cast<float>(cursorPnt.Y()), static_cast<float>(cursorPnt.Z()));
		imp_.FloatingLine_->getPosition(0) = beginVec;
		imp_.FloatingLine_->getPosition(1) = curVec;
	}

	return false;
}

void TestDrawRoomCtrller::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	auto driver = GetRenderContextSPtr()->Smgr_->getVideoDriver();

	switch (imp_.State_)
	{
	case EState::ES_READY:
		{
			driver->setTransform(ETS_WORLD, matrix4());
			driver->setMaterial(imp_.TestRoomLine_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.TestRoomLine_->getVertices(), imp_.TestRoomLine_->getVertexCount(), imp_.TestRoomLine_->getIndices(), imp_.TestRoomLine_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);

			driver->setTransform(ETS_WORLD, imp_.PositionRectTransMat_*imp_.PositionRectScaleMat_);
			driver->setMaterial(imp_.PositionRect_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.PositionRect_->getVertices(), imp_.PositionRect_->getVertexCount(), imp_.PositionRect_->getIndices(), imp_.PositionRect_->getIndexCount()/2, EVT_STANDARD, EPT_TRIANGLE_FAN);
		}
		break;
	case EState::ES_DRAWING:
		{
			driver->setTransform(ETS_WORLD, matrix4());
			driver->setMaterial(imp_.TestRoomLine_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.TestRoomLine_->getVertices(), imp_.TestRoomLine_->getVertexCount(), imp_.TestRoomLine_->getIndices(), imp_.TestRoomLine_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);

			if ( imp_.BeginCorner_ )
			{
				driver->setTransform(ETS_WORLD, matrix4());
				driver->setMaterial(imp_.FloatingLine_->getMaterial());
				driver->drawVertexPrimitiveList(imp_.FloatingLine_->getVertices(), imp_.FloatingLine_->getVertexCount(), imp_.FloatingLine_->getIndices(), imp_.FloatingLine_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
			}

			driver->setTransform(ETS_WORLD, imp_.PositionRectTransMat_*imp_.PositionRectScaleMat_);
			driver->setMaterial(imp_.PositionRect_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.PositionRect_->getVertices(), imp_.PositionRect_->getVertexCount(), imp_.PositionRect_->getIndices(), imp_.PositionRect_->getIndexCount()/2, EVT_STANDARD, EPT_TRIANGLE_FAN);
		}
		break;
	default:
		break;
	}
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
		if ( evt.KeyInput.Key == KEY_ESCAPE && evt.KeyInput.PressedDown )
		{
			imp_.Finish_ = true;
		}
	}

	return false;
}

void TestDrawRoomCtrller::Init()
{
	auto& imp_ = *ImpUPtr_;

	{
		imp_.FloatingLine_->Vertices.reallocate(2);
		imp_.FloatingLine_->Indices.reallocate(2);

		imp_.FloatingLine_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(0xFF0000FF), vector2df(0,0)));
		imp_.FloatingLine_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(0xFF0000FF), vector2df(0,0)));
		imp_.FloatingLine_->Indices.push_back(0);
		imp_.FloatingLine_->Indices.push_back(1);
	}

	{
		imp_.PositionRect_->Vertices.reallocate(4);
		imp_.PositionRect_->Indices.reallocate(4);

		auto radius = 200.f;

		imp_.PositionRect_->Vertices.push_back(S3DVertex(vector3df(-1, 0, -1), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
		imp_.PositionRect_->Vertices.push_back(S3DVertex(vector3df(1, 0, -1), vector3df(0,1,0), SColor(~0), vector2df(1,0)));
		imp_.PositionRect_->Vertices.push_back(S3DVertex(vector3df(1, 0, 1), vector3df(0,1,0), SColor(~0), vector2df(1,1)));
		imp_.PositionRect_->Vertices.push_back(S3DVertex(vector3df(-1, 0, 1), vector3df(0,1,0), SColor(~0), vector2df(0,1)));

		imp_.PositionRect_->Indices.push_back(0);
		imp_.PositionRect_->Indices.push_back(1);
		imp_.PositionRect_->Indices.push_back(2);
		imp_.PositionRect_->Indices.push_back(3);

		imp_.PositionRectScaleMat_.setScale(vector3df(radius, 1, radius));

		imp_.PositionRect_->getMaterial().setTexture(0, GetRenderContextSPtr()->Smgr_->getVideoDriver()->getTexture("../Data/Resource/3D/dot.png"));
	}
	
	{
		imp_.FloatingLine_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(0xFF0000FF), vector2df(0,0)));
		imp_.FloatingLine_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(0xFF0000FF), vector2df(0,0)));
		imp_.FloatingLine_->Indices.push_back(0);
		imp_.FloatingLine_->Indices.push_back(1);
	}
}
