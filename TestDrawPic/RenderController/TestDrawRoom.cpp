#include "stdafx.h"

#include "TestDrawRoom.h"
#include "SMeshBuffer.h"
#include "irrEngine/SRenderContext.h"
#include "irrEngine/irrEngine.h"

#include "ODL/GraphODL.h"
#include "ODL/CornerODL.h"
#include "ODL/WallODL.h"

#include "StatusMgr.h"

#include "gp_Pnt.hxx"
#include "gp_Pln.hxx"
#include "GeomAPI.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"
#include "Geom2dAPI_InterCurveCurve.hxx"
#include "BRep_Tool.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "gce_MakeLin.hxx"

#include <vector>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

static	gp_Pln	compareIntCCPln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));

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

		PositionRect_ = new SMeshBuffer;
		PositionRect_->getMaterial().Lighting = false;
		PositionRect_->getMaterial().BackfaceCulling = false;
		PositionRect_->getMaterial().ZWriteEnable = false;
		PositionRect_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_VERTEX_ALPHA);
		PositionRect_->getMaterial().MaterialTypeParam = 0.65f;

		FloatingLine_ = new SMeshBuffer;
		FloatingLine_->getMaterial().Lighting = false;
		FloatingLine_->getMaterial().BackfaceCulling = false;
		FloatingLine_->getMaterial().ZWriteEnable = false;
		FloatingLine_->getMaterial().Thickness = 4;
		FloatingLine_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);

		AuxiliaryLine_ = new SMeshBuffer;
		AuxiliaryLine_->getMaterial().Lighting = false;
		AuxiliaryLine_->getMaterial().BackfaceCulling = false;
		AuxiliaryLine_->getMaterial().ZWriteEnable = false;
		AuxiliaryLine_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
		AuxiliaryLine_->getMaterial().DiffuseColor = 0xFF000000;
		AuxiliaryLine_->getMaterial().Thickness = 2;

		PressDown_ = false;
		Finish_ = false;

		Polar_ = 30.f;
		PolarRange_ = 10.f;
		AllowPolar_ = true;
		AllowPerpendicular_ = true;
	}

	~Imp()
	{
		if ( FloatingLine_ )
		{
			FloatingLine_->drop();
		}

		if ( PositionRect_ )
		{
			PositionRect_->drop();
		}

		if ( AuxiliaryLine_ )
		{
			AuxiliaryLine_->drop();
		}
	}

	void	SetNeedUpdateMeshOnCorner(const CornerODLSPtr& corner)
	{
		for ( auto& curWall : Graph_.lock()->GetWallsOnCorner(corner) )
		{
			curWall->SetDirty(true);
		}
	}

public:

	EState						State_;

	//极轴角度
	float						Polar_;
	
	//极轴范围
	float						PolarRange_;
	
	//极轴追踪
	bool						AllowPolar_;
	
	//垂线追踪
	bool						AllowPerpendicular_;
	
	//目前的线
	SMeshBuffer*				FloatingLine_;

	//目前的点
	SMeshBuffer*				PositionRect_;
	matrix4						PositionRectScaleMat_;
	matrix4						PositionRectTransMat_;

	//辅助线
	SMeshBuffer*				AuxiliaryLine_;

	//绘制路径点
	std::vector<CornerODLSPtr>	DrawingPathList_;

	//Floating的起始点
	CornerODLSPtr				LastCorner_;
	WallODLSPtr					LastWall_;

	vector2di					CursorIPos_;
	
	bool						PressDown_;
	bool						Finish_;

	GraphODLWPtr				Graph_;
};

TestDrawRoomCtrller::TestDrawRoomCtrller(const GraphODLWPtr& graphODL):ImpUPtr_(new Imp)
{
	ImpUPtr_->Graph_ = graphODL;
}

TestDrawRoomCtrller::~TestDrawRoomCtrller()
{

}

bool TestDrawRoomCtrller::PreRender3D()
{
	CCombineSceneNode::SetRenderMode(CBaseSceneNode::ESNT_2D);

	auto& imp_ = *ImpUPtr_;

	plane3df plan(0,0,0,0,1,0);
	vector3df planPos;
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), planPos);

	//当前鼠标点
	gp_Pnt cursorPnt(planPos.X, 0, planPos.Z);
	
	//吸附半径
	auto alignDistance = 200.f;

	//最近的墙角
	CornerODLSPtr nearestCorner;
	
	//只有一个墙的墙角
	std::vector<CornerODLSPtr>	lonelyCorners;

	//需要拆分的墙
	WallODLSPtr toSplit;

	//两条辅助线
	BRepAdaptor_Curve firstAuxiliaryLine,secondAuxiliaryLine;

	//寻找最近的墙角和一个墙的墙角
	for ( auto& curCorner : imp_.Graph_.lock()->GetAllCorners() )
	{
		if ( curCorner->GetPosition().SquareDistance(cursorPnt) < alignDistance * alignDistance )
		{
			cursorPnt = curCorner->GetPosition();
			nearestCorner = curCorner;

			break;
		}

		auto walls = imp_.Graph_.lock()->GetWallsOnCorner(curCorner);
		if ( 1 == walls.size() )
		{
			lonelyCorners.push_back(curCorner);
		}
	}

	//寻找线
	if ( !nearestCorner )
	{
		std::vector<BRepAdaptor_Curve>	lineGroup;

		//满足条件的线
		BRepAdaptor_Curve ontoCurve;

		auto allWalls = imp_.Graph_.lock()->GetAllWalls();

		//延长线和拆分点
		for ( auto& curWall : allWalls )
		{
			//暂时只考虑直线
			if ( curWall->IsBezierCurve() )
			{
				continue;
			}

			auto curEdge = curWall->GetEdge();
			BRepAdaptor_Curve curBC(curEdge);
			
			GeomAPI_ProjectPointOnCurve projCursorToWall(cursorPnt, curBC.Curve().Curve());

			//排除未发生吸附的
			if ( projCursorToWall.LowerDistance() > alignDistance )
			{
				continue;
			}

			auto curPar = projCursorToWall.LowerDistanceParameter();
			auto isOnCurve = curPar > curBC.FirstParameter() + curWall->GetThickness()/2 && curPar < curBC.LastParameter()-curWall->GetThickness()/2;
			
			//检查是否为拆分边
			if ( !toSplit && isOnCurve)
			{
				toSplit = curWall;
				ontoCurve = curBC;
				continue;
			}
			
			lineGroup.push_back(curBC);

			//墙边垂线
			if ( imp_.AllowPerpendicular_ )
			{
				if ( imp_.LastCorner_ )
				{
					GeomAPI_ProjectPointOnCurve projLastCornerToWall(imp_.LastCorner_->GetPosition(), curBC.Curve().Curve());
					auto pnt = projLastCornerToWall.NearestPoint();
					if ( pnt.SquareDistance(cursorPnt) < alignDistance * alignDistance )
					{
						BRepAdaptor_Curve bc(BRepBuilderAPI_MakeEdge(imp_.LastCorner_->GetPosition(), pnt).Edge());
						lineGroup.push_back(bc);
					}
				}
			}
		}

		//墙角垂线
		if ( lineGroup.size() < 2 )
		{
			for ( auto& curCorner : lonelyCorners )
			{
				auto curWalls = imp_.Graph_.lock()->GetWallsOnCorner(curCorner);
				assert(curWalls.size() == 1);

				auto& curWall = curWalls.front();
				gp_Dir curDir = gp_Vec(curWall->GetFirstCorner().lock()->GetPosition(), curWall->GetSecondCorner().lock()->GetPosition());
				curDir.Rotate(gp::OY(), M_PI_2);
				
				auto curLin = gce_MakeLin(curCorner->GetPosition(), curDir).Value();
				if ( curLin.Distance(cursorPnt) < alignDistance )
				{
					BRepAdaptor_Curve curBC(BRepBuilderAPI_MakeEdge(curLin).Edge());
					lineGroup.push_back(curBC);
				}
			}
		}

		{//排序、归并已找到的边
			std::sort(lineGroup.begin(), lineGroup.end(), [](const BRepAdaptor_Curve& bc1, const BRepAdaptor_Curve& bc2)
			{
				auto angle1 = bc1.Line().Direction().AngleWithRef(gp::DX(), gp::DY().Reversed());
				angle1 = angle1 < 0 ? 2 * M_PI + angle1 : angle1;

				auto angle2 = bc2.Line().Direction().AngleWithRef(gp::DX(), gp::DY().Reversed());
				angle2 = angle2 < 0 ? 2 * M_PI + angle2 : angle2;

				return angle1 < angle2;
			});

			std::unique(lineGroup.begin(), lineGroup.end(), [](const BRepAdaptor_Curve& bc1, const BRepAdaptor_Curve& bc2)
			{
				auto angle1 = bc1.Line().Direction();
				auto angle2 = bc2.Line().Direction();

				return Standard_True == angle1.IsParallel(angle2, Precision::Angular());
			});

			if ( toSplit )
			{
				auto splitDir = ontoCurve.Line().Direction();
				auto itor = std::remove_if(lineGroup.begin(), lineGroup.end(), [&splitDir](const BRepAdaptor_Curve& bc)
				{
					return Standard_True == splitDir.IsParallel(bc.Line().Direction(), Precision::Angular()); 
				});
				lineGroup.erase(itor, lineGroup.end());
			}
		}

		if ( toSplit )
		{
			firstAuxiliaryLine = ontoCurve;
			
			auto ontoCurve2D = GeomAPI::To2d(ontoCurve.Curve().Curve(), compareIntCCPln);

			for ( auto& curBC : lineGroup )
			{
				auto cur2D = GeomAPI::To2d(curBC.Curve().Curve(), compareIntCCPln);
				Geom2dAPI_InterCurveCurve icc(ontoCurve2D, cur2D);

				assert(1 == icc.NbPoints());

				auto iccPnt2D = icc.Point(1);
				gp_Pnt iccPnt(iccPnt2D.X(), 0, iccPnt2D.Y());

				if ( iccPnt.SquareDistance(cursorPnt) < alignDistance * alignDistance )
				{
					cursorPnt = iccPnt;
					secondAuxiliaryLine = curBC;
					break;
				}
			}
		}
		else if ( !lineGroup.empty() )
		{
			firstAuxiliaryLine = lineGroup.front();

			do
			{
				auto tmpFirstLine = lineGroup.front();
				auto tmpFirstLine2D = GeomAPI::To2d(tmpFirstLine.Curve().Curve(), compareIntCCPln);

				lineGroup.erase(lineGroup.begin());
				auto needBreak = false;
				for ( auto& curBC : lineGroup )
				{
					auto cur2D = GeomAPI::To2d(curBC.Curve().Curve(), compareIntCCPln);
					Geom2dAPI_InterCurveCurve icc(tmpFirstLine2D, cur2D);

					if ( 1 != icc.NbPoints() )
					{
						continue;
					}

					auto iccPnt2D = icc.Point(1);
					gp_Pnt iccPnt(iccPnt2D.X(), 0, iccPnt2D.Y());

					if ( iccPnt.SquareDistance(cursorPnt) < alignDistance * alignDistance )
					{
						cursorPnt = iccPnt;
						firstAuxiliaryLine = tmpFirstLine;
						secondAuxiliaryLine = curBC;
						needBreak = true;
						break;
					}
				}

				if ( needBreak )
				{
					break;
				}
			}while ( !lineGroup.empty() );
		}

		if ( GeomAbs_OtherCurve != firstAuxiliaryLine.GetType() && GeomAbs_OtherCurve == secondAuxiliaryLine.GetType() )
		{
			GeomAPI_ProjectPointOnCurve ppc(cursorPnt, firstAuxiliaryLine.Curve().Curve());
			cursorPnt = ppc.NearestPoint();
		}

		if ( GeomAbs_OtherCurve == firstAuxiliaryLine.GetType() && StatusMgr::GetInstance().GridAlign_ )
		{
			auto pos = *StatusMgr::GetInstance().GridAlign_;
			cursorPnt = gp_Pnt(pos.X, 0, pos.Z);
		}
	}

	switch (imp_.State_)
	{
	case EState::ES_READY:
		{
			if ( imp_.PressDown_ )
			{
				imp_.PressDown_ = false;
				if ( nearestCorner )
				{
					imp_.LastCorner_ = nearestCorner;
				}
				else
				{
					if ( toSplit )
					{
						imp_.LastCorner_ = imp_.Graph_.lock()->CreateCornerBySplitWall(toSplit, cursorPnt);
						for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(imp_.LastCorner_) )
						{
							curWall->UpdateMesh();
						}
					}
					else
					{
						imp_.LastCorner_ = imp_.Graph_.lock()->CreateCorner(cursorPnt);
					}
				}

				imp_.DrawingPathList_.push_back(imp_.LastCorner_);
				imp_.State_ = EState::ES_DRAWING;
			}
		}
		break;
	case EState::ES_DRAWING:
		{
			bool valid = true;
			{//checkValid

				//与起始点点重合
				if ( imp_.LastCorner_ == nearestCorner )
				{
					valid = false;
				}
				else
				{
					//目前的边
					auto floatingEdge = BRepBuilderAPI_MakeEdge(imp_.LastCorner_->GetPosition(), cursorPnt).Edge();
					BRepAdaptor_Curve floatingBC(floatingEdge);

					//与边的夹角小于15度
					if ( valid )
					{
						for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(imp_.LastCorner_) )
						{
							auto beginCorner = curWall->GetFirstCorner().lock();
							auto endCorner = curWall->GetSecondCorner().lock();

							if ( beginCorner != imp_.LastCorner_ )
							{
								beginCorner.swap(endCorner);
							}

							auto curWallEdge = BRepBuilderAPI_MakeEdge(beginCorner->GetPosition(), endCorner->GetPosition()).Edge();
							BRepAdaptor_Curve wallBC(curWallEdge);

							auto curAngle = floatingBC.Line().Direction().Angle(wallBC.Line().Direction());

							static auto rad15 = 15 * M_PI / 180.f;

							if ( curAngle < rad15 )
							{
								valid = false;
								break;
							}

						}
					}

					//是否有交叉
					if ( valid )
					{
						gp_Pln pln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));

						//目前的边
						auto floatingEdge = BRepBuilderAPI_MakeEdge(imp_.LastCorner_->GetPosition(), cursorPnt).Edge();
						Standard_Real floatingFirstPar,floatingLastPar;
						BRep_Tool::Range(floatingEdge, floatingFirstPar, floatingLastPar);

						for ( auto& curWall : imp_.Graph_.lock()->GetAllWalls() )
						{
							if ( curWall->GetFirstCorner().lock() == imp_.LastCorner_ || curWall->GetSecondCorner().lock() == imp_.LastCorner_ )
							{
								continue;
							}

							auto curThickness = curWall->GetThickness();

							auto wallEdge = curWall->GetEdge();							

							BRepExtrema_DistShapeShape dss(floatingEdge, wallEdge);

							if ( dss.NbSolution() == 0 )
							{
								continue;
							}

							if ( dss.Value() > curThickness/2 )
							{
								continue;
							}

							//平行
							if ( dss.NbSolution() != 1 )
							{
								valid = false;
								break;
							}

							Standard_Real firstCurP,lastCurP;
							BRep_Tool::Range(wallEdge, firstCurP, lastCurP);

							auto pnt = dss.PointOnShape2(1);
							
							GeomAPI_ProjectPointOnCurve ppc(pnt, BRepAdaptor_Curve(floatingEdge).Curve().Curve());
							auto parOnFloating = ppc.LowerDistanceParameter();
							
							if ( parOnFloating < floatingLastPar - curThickness/2 )
							{
								valid = false;
								break;
							}
						}
					}
				}
			}

			if ( valid )
			{
				imp_.FloatingLine_->getMaterial().DiffuseColor = 0xFF0000FF;//Blue
			}
			else
			{
				imp_.FloatingLine_->getMaterial().DiffuseColor = 0xFFFF0000;//Read
			}

			if ( imp_.PressDown_ )
			{
				imp_.PressDown_ = false;

				if ( valid )
				{
					imp_.SetNeedUpdateMeshOnCorner(imp_.LastCorner_);

					if ( nearestCorner )
					{
						imp_.LastWall_ = imp_.Graph_.lock()->AddWall(imp_.LastCorner_, nearestCorner);
						imp_.LastCorner_ = nearestCorner;
					}
					else
					{
						if ( toSplit )
						{
							auto newCorner = imp_.Graph_.lock()->CreateCornerBySplitWall(toSplit, cursorPnt);
							imp_.LastWall_ = imp_.Graph_.lock()->AddWall(imp_.LastCorner_, newCorner);
							imp_.LastCorner_ = newCorner;
						}
						else
						{
							auto newCorner = imp_.Graph_.lock()->CreateCorner(cursorPnt);
							imp_.LastWall_ = imp_.Graph_.lock()->AddWall(imp_.LastCorner_, newCorner);
							imp_.LastCorner_ = newCorner;
						}
					}

					imp_.DrawingPathList_.push_back(imp_.LastCorner_);

					imp_.SetNeedUpdateMeshOnCorner(imp_.LastCorner_);
				}
			}

			if ( imp_.Finish_ )
			{
				imp_.Finish_ = false;

				auto walls = imp_.Graph_.lock()->GetWallsOnCorner(imp_.LastCorner_);
				if ( walls.empty() )
				{
					imp_.Graph_.lock()->RemoveCorner(imp_.LastCorner_);
				}
				imp_.LastCorner_ = nullptr;
				imp_.LastWall_ = nullptr;

				for ( auto& curCorner : imp_.DrawingPathList_ )
				{
					curCorner->SetVisible(false);
				}
				imp_.DrawingPathList_.clear();
				imp_.State_ = EState::ES_READY;
			}
		}
		break;
	default:
		break;
	}

	for ( auto& curWall : imp_.Graph_.lock()->GetAllWalls() )
	{
		if ( curWall->IsDirty() )
		{
			curWall->UpdateMesh();
			curWall->SetDirty(false);
		}
	}

	imp_.AuxiliaryLine_->getPosition(0) = vector3df(static_cast<float>(cursorPnt.X()), static_cast<float>(cursorPnt.Y()), static_cast<float>(cursorPnt.Z()));
	imp_.AuxiliaryLine_->getPosition(1) = vector3df(static_cast<float>(cursorPnt.X()), static_cast<float>(cursorPnt.Y()), static_cast<float>(cursorPnt.Z()));
	imp_.AuxiliaryLine_->getPosition(2) = vector3df(static_cast<float>(cursorPnt.X()), static_cast<float>(cursorPnt.Y()), static_cast<float>(cursorPnt.Z()));
	imp_.AuxiliaryLine_->getPosition(3) = vector3df(static_cast<float>(cursorPnt.X()), static_cast<float>(cursorPnt.Y()), static_cast<float>(cursorPnt.Z()));

	if ( GeomAbs_OtherCurve != firstAuxiliaryLine.GetType() )
	{
		gp_Pnt p1,p2;

		firstAuxiliaryLine.D0(-100000, p1);
		firstAuxiliaryLine.D0(100000, p2);

		imp_.AuxiliaryLine_->getPosition(0) = vector3df(static_cast<float>(p1.X()), static_cast<float>(p1.Y()), static_cast<float>(p1.Z()));
		imp_.AuxiliaryLine_->getPosition(1) = vector3df(static_cast<float>(p2.X()), static_cast<float>(p2.Y()), static_cast<float>(p2.Z()));
	}

	if ( GeomAbs_OtherCurve != secondAuxiliaryLine.GetType() )
	{
		gp_Pnt p1,p2;

		secondAuxiliaryLine.D0(-100000, p1);
		secondAuxiliaryLine.D0(100000, p2);

		imp_.AuxiliaryLine_->getPosition(2) = vector3df(static_cast<float>(p1.X()), static_cast<float>(p1.Y()), static_cast<float>(p1.Z()));
		imp_.AuxiliaryLine_->getPosition(3) = vector3df(static_cast<float>(p2.X()), static_cast<float>(p2.Y()), static_cast<float>(p2.Z()));
	}

	{
		vector3df center(static_cast<float>(cursorPnt.X()), static_cast<float>(cursorPnt.Y()), static_cast<float>(cursorPnt.Z()));
		imp_.PositionRectTransMat_.setTranslation(center);
	}

	if ( imp_.LastCorner_ )
	{
		auto beginPnt = imp_.LastCorner_->GetPosition();
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
			driver->setTransform(ETS_WORLD, imp_.PositionRectTransMat_*imp_.PositionRectScaleMat_);
			driver->setMaterial(imp_.PositionRect_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.PositionRect_->getVertices(), imp_.PositionRect_->getVertexCount(), imp_.PositionRect_->getIndices(), imp_.PositionRect_->getIndexCount()/2, EVT_STANDARD, EPT_TRIANGLE_FAN);

			driver->setTransform(ETS_WORLD, matrix4());
			driver->setMaterial(imp_.AuxiliaryLine_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.AuxiliaryLine_->getVertices(), imp_.AuxiliaryLine_->getVertexCount(), imp_.AuxiliaryLine_->getIndices(), imp_.AuxiliaryLine_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
		}
		break;
	case EState::ES_DRAWING:
		{
			if ( imp_.LastCorner_ )
			{
				driver->setTransform(ETS_WORLD, matrix4());
				driver->setMaterial(imp_.FloatingLine_->getMaterial());
				driver->drawVertexPrimitiveList(imp_.FloatingLine_->getVertices(), imp_.FloatingLine_->getVertexCount(), imp_.FloatingLine_->getIndices(), imp_.FloatingLine_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
			}

			driver->setTransform(ETS_WORLD, imp_.PositionRectTransMat_*imp_.PositionRectScaleMat_);
			driver->setMaterial(imp_.PositionRect_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.PositionRect_->getVertices(), imp_.PositionRect_->getVertexCount(), imp_.PositionRect_->getIndices(), imp_.PositionRect_->getIndexCount()/2, EVT_STANDARD, EPT_TRIANGLE_FAN);

			driver->setTransform(ETS_WORLD, matrix4());
			driver->setMaterial(imp_.AuxiliaryLine_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.AuxiliaryLine_->getVertices(), imp_.AuxiliaryLine_->getVertexCount(), imp_.AuxiliaryLine_->getIndices(), imp_.AuxiliaryLine_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
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

		imp_.FloatingLine_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
		imp_.FloatingLine_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
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

	{
		imp_.AuxiliaryLine_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
		imp_.AuxiliaryLine_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
		imp_.AuxiliaryLine_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
		imp_.AuxiliaryLine_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
		imp_.AuxiliaryLine_->Indices.push_back(0);
		imp_.AuxiliaryLine_->Indices.push_back(1);
		imp_.AuxiliaryLine_->Indices.push_back(2);
		imp_.AuxiliaryLine_->Indices.push_back(3);
	}
}
