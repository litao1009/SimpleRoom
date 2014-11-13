#include "stdafx.h"

#include "RoomLayoutDrawingCtrller.h"
#include "SMeshBuffer.h"
#include "irrEngine/SRenderContext.h"
#include "irrEngine/irrEngine.h"
#include "UserEvent.h"

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

class	RoomLayoutDrawingCtrller::Imp
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
		PositionRect_->getMaterial().ZBuffer = ECFN_ALWAYS;

		FloatingLine_ = new SMeshBuffer;
		FloatingLine_->getMaterial().Lighting = false;
		FloatingLine_->getMaterial().BackfaceCulling = false;
		FloatingLine_->getMaterial().ZWriteEnable = false;
		FloatingLine_->getMaterial().Thickness = 4;
		FloatingLine_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
		FloatingLine_->getMaterial().ZBuffer = ECFN_ALWAYS;

		AuxiliaryLine_ = new SMeshBuffer;
		AuxiliaryLine_->getMaterial().Lighting = false;
		AuxiliaryLine_->getMaterial().BackfaceCulling = false;
		AuxiliaryLine_->getMaterial().ZWriteEnable = false;
		AuxiliaryLine_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
		AuxiliaryLine_->getMaterial().DiffuseColor = 0xFF000000;
		AuxiliaryLine_->getMaterial().Thickness = 2;
		AuxiliaryLine_->getMaterial().ZBuffer = ECFN_ALWAYS;

		LMousePressDown_ = false;
		EscPressDown_ = false;

		Polar_ = 30.f;
		PolarRange_ = 10.f;
		AllowPolar_ = true;
		AllowPerpendicular_ = true;
		Reset_ = false;
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
	
	bool						LMousePressDown_;
	bool						EscPressDown_;
	bool						Reset_;
	boost::optional<int>		CustomLength_;
	boost::optional<gp_Pnt>		CustomPnt_;

	GraphODLWPtr				Graph_;
};

RoomLayoutDrawingCtrller::RoomLayoutDrawingCtrller(const GraphODLWPtr& graphODL):ImpUPtr_(new Imp)
{
	ImpUPtr_->Graph_ = graphODL;
}

RoomLayoutDrawingCtrller::~RoomLayoutDrawingCtrller()
{

}

bool RoomLayoutDrawingCtrller::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	if ( imp_.Reset_ )
	{
		Reset();
		imp_.Reset_ = false;
	}

	CCombineSceneNode::SetRenderMode(CBaseSceneNode::ESNT_2D);

	plane3df plan(0,0,0,0,1,0);
	vector3df planPos;
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), planPos);

	//当前鼠标点
	gp_Pnt cursorPnt(planPos.X, 0, planPos.Z);
	
	//吸附半径
	auto alignDistance = 100.f;

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
			}
			else
			{
				lineGroup.push_back(curBC);
			}

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

		//极轴追踪
		if ( imp_.AllowPolar_ && imp_.State_ == EState::ES_DRAWING && imp_.LastCorner_ && GeomAbs_OtherCurve == secondAuxiliaryLine.GetType() )
		{
			auto polarWalls = imp_.Graph_.lock()->GetWallsOnCorner(imp_.LastCorner_);
			if ( !polarWalls.empty() && imp_.LastCorner_->GetPosition().SquareDistance(cursorPnt) > alignDistance * alignDistance )
			{//起始点有墙
				auto floatingEdge = BRepBuilderAPI_MakeEdge(imp_.LastCorner_->GetPosition(), cursorPnt).Edge();
				BRepAdaptor_Curve floatingBC(floatingEdge);

				for ( auto& curWall : polarWalls )
				{
					auto curEdge = curWall->GetEdge(imp_.LastCorner_);
					BRepAdaptor_Curve curBC(curEdge);

					auto rad = curBC.Line().Direction().AngleWithRef(floatingBC.Line().Direction(), gp::DY().Reversed());
					rad = rad < 0 ? 2 * M_PI + rad : rad;
					auto angle = irr::core::radToDeg(rad);

					auto rest = std::fmod(angle, imp_.Polar_);
					if ( rest < imp_.PolarRange_/2 )
					{
						angle -= rest;
					}
					if ( imp_.Polar_-rest < imp_.PolarRange_/2 )
					{
						angle += imp_.Polar_ - rest;
					}

					auto newDir = curBC.Line().Direction().Rotated(gp::OY().Reversed(), irr::core::degToRad(angle));

					if ( Standard_True == newDir.IsEqual(floatingBC.Line().Direction(), Precision::Angular()) )
					{
						continue;
					}

					if ( GeomAbs_OtherCurve != firstAuxiliaryLine.GetType() && Standard_True == newDir.IsParallel(firstAuxiliaryLine.Line().Direction(), Precision::Angular()) )
					{
						continue;
					}

					gp_Lin newLin(imp_.LastCorner_->GetPosition(), newDir);
					auto newEdge = BRepBuilderAPI_MakeEdge(newLin).Edge();
					BRepAdaptor_Curve newBC(newEdge);
					auto new2D = GeomAPI::To2d(newBC.Curve().Curve(), compareIntCCPln);

					if ( GeomAbs_OtherCurve != firstAuxiliaryLine.GetType() )
					{
						auto first2D = GeomAPI::To2d(firstAuxiliaryLine.Curve().Curve(), compareIntCCPln);

						Geom2dAPI_InterCurveCurve icc(new2D, first2D);
						assert(1 == icc.NbPoints());
						auto iccPnt2D = icc.Point(1);
						gp_Pnt iccPnt(iccPnt2D.X(), 0, iccPnt2D.Y());

						if ( iccPnt.SquareDistance(cursorPnt) < alignDistance * alignDistance )
						{
							cursorPnt = iccPnt;
							secondAuxiliaryLine = newBC;
							break;
						}
					}
					else
					{
						GeomAPI_ProjectPointOnCurve ppcPolar(cursorPnt, newBC.Curve().Curve());
						auto polarPnt = ppcPolar.NearestPoint();
						auto testDis = polarPnt.Distance(cursorPnt);

						if ( polarPnt.SquareDistance(cursorPnt) < alignDistance * alignDistance )
						{
							cursorPnt = polarPnt;
							firstAuxiliaryLine = newBC;
							break;
						}
					}
				}
			}
			else
			{//起始点无墙
				auto floatingEdge = BRepBuilderAPI_MakeEdge(imp_.LastCorner_->GetPosition(), cursorPnt).Edge();
				BRepAdaptor_Curve floatingBC(floatingEdge);

				gp_Lin dxLin(imp_.LastCorner_->GetPosition(), gp::DX());
				auto dxEdge = BRepBuilderAPI_MakeEdge(dxLin);
				BRepAdaptor_Curve dxBC(dxEdge);

				auto rad = dxBC.Line().Direction().AngleWithRef(floatingBC.Line().Direction(), gp::DY().Reversed());
				rad = rad < 0 ? 2 * M_PI + rad : rad;
				auto angle = irr::core::radToDeg(rad);

				auto rest = std::fmod(angle, imp_.Polar_);
				if ( rest < imp_.PolarRange_/2 )
				{
					angle -= rest;
				}
				if ( imp_.Polar_-rest < imp_.PolarRange_/2 )
				{
					angle += imp_.Polar_ - rest;
				}

				auto newDir = dxBC.Line().Direction().Rotated(gp::OY().Reversed(), irr::core::degToRad(angle));

				if ( Standard_False == newDir.IsEqual(floatingBC.Line().Direction(), Precision::Angular()) )
				{
					gp_Lin newLin(imp_.LastCorner_->GetPosition(), newDir);
					auto newEdge = BRepBuilderAPI_MakeEdge(newLin).Edge();
					BRepAdaptor_Curve newBC(newEdge);

					if ( (GeomAbs_OtherCurve != firstAuxiliaryLine.GetType() && Standard_False == newDir.IsParallel(firstAuxiliaryLine.Line().Direction(), Precision::Angular())) )
					{
						auto new2D = GeomAPI::To2d(newBC.Curve().Curve(), compareIntCCPln);
						auto first2D = GeomAPI::To2d(firstAuxiliaryLine.Curve().Curve(), compareIntCCPln);

						Geom2dAPI_InterCurveCurve icc(new2D, first2D);
						assert(1 == icc.NbPoints());
						auto iccPnt2D = icc.Point(1);
						gp_Pnt iccPnt(iccPnt2D.X(), 0, iccPnt2D.Y());

						if ( iccPnt.SquareDistance(cursorPnt) < alignDistance * alignDistance )
						{
							cursorPnt = iccPnt;
							secondAuxiliaryLine = newBC;
						}
					}
					else if (GeomAbs_OtherCurve == firstAuxiliaryLine.GetType())
					{
						GeomAPI_ProjectPointOnCurve ppcPolar(cursorPnt, newBC.Curve().Curve());
						auto polarPnt = ppcPolar.NearestPoint();
						auto testDis = polarPnt.Distance(cursorPnt);

						if ( polarPnt.SquareDistance(cursorPnt) < alignDistance * alignDistance )
						{
							cursorPnt = polarPnt;
							firstAuxiliaryLine = newBC;
						}
					}
				}
			}
		}

		if ( GeomAbs_OtherCurve == firstAuxiliaryLine.GetType() && StatusMgr::GetInstance().GridAlign_ )
		{
			auto pos = *StatusMgr::GetInstance().GridAlign_;
			cursorPnt = gp_Pnt(pos.X, 0, pos.Z);
		}
		else if ( GeomAbs_OtherCurve != firstAuxiliaryLine.GetType() && GeomAbs_OtherCurve == secondAuxiliaryLine.GetType() )
		{
			GeomAPI_ProjectPointOnCurve ppc(cursorPnt, firstAuxiliaryLine.Curve().Curve());
			cursorPnt = ppc.NearestPoint();
		}
	}

	switch (imp_.State_)
	{
	case EState::ES_READY:
		{
			if ( imp_.LMousePressDown_ )
			{
				imp_.LMousePressDown_ = false;
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
							curWall->UpdateBaseMesh();
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

			if ( imp_.EscPressDown_ )
			{
				irr::SEvent evt;
				evt.EventType = EET_USER_EVENT;
				evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_DRAWLINE_FINISH;
				GetRenderContextSPtr()->PostEvent(evt);
			}
		}
		break;
	case EState::ES_DRAWING:
		{
			if ( imp_.CustomLength_ )
			{
				assert(imp_.LastCorner_);

				gp_Dir dir = gp_Vec(imp_.LastCorner_->GetPosition(), cursorPnt);
				cursorPnt = imp_.LastCorner_->GetPosition().XYZ() + (gp_Vec(dir) * (*imp_.CustomLength_)).XYZ();
				imp_.CustomPnt_ = cursorPnt;
				vector3df newPos3D(static_cast<float>(cursorPnt.X()), static_cast<float>(cursorPnt.Y()), static_cast<float>(cursorPnt.Z()));
				auto newPos2D = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(newPos3D);
				GetRenderContextSPtr()->CursorControl_->setPosition(newPos2D);
			}
			else if ( imp_.CustomPnt_ )
			{
				if ( cursorPnt.SquareDistance(*imp_.CustomPnt_) < alignDistance * alignDistance * 4 )
				{
					cursorPnt = *imp_.CustomPnt_;
				}
				else
				{
					imp_.CustomPnt_ = boost::none;
				}
			}

			bool valid = true;
			{//checkValid

				//与起始点点重合
				if ( imp_.LastCorner_ == nearestCorner )
				{
					valid = false;
				}
				else if ( imp_.LastCorner_->GetPosition().SquareDistance(cursorPnt) < alignDistance * alignDistance )
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

			if ( imp_.LMousePressDown_ )
			{
				imp_.LMousePressDown_ = false;

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

			if ( imp_.EscPressDown_ )
			{
				imp_.EscPressDown_ = false;
				imp_.State_ = EState::ES_READY;
				
				for ( auto& curCorner : imp_.DrawingPathList_ )
				{
					curCorner->SetVisible(false);
				}
				imp_.DrawingPathList_.clear();

				auto walls = imp_.Graph_.lock()->GetWallsOnCorner(imp_.LastCorner_);
				if ( walls.empty() )
				{
					imp_.Graph_.lock()->RemoveCorner(imp_.LastCorner_);
				}
				else
				{
					imp_.Graph_.lock()->MergeWallIfNeeded(imp_.LastCorner_);
				}
				
				imp_.LastCorner_.reset();
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
			curWall->UpdateBaseMesh();
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

	for ( auto& curCorner : imp_.DrawingPathList_ )
	{
		curCorner->SetVisible(true);
	}

	if ( imp_.State_ == ES_DRAWING )
	{
		::PostMessage((HWND)(GetRenderContextSPtr()->GetHandle()), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_DLG_LINELENGTH_SHOW, 0);

		auto length = 0;
		auto fl = imp_.LastCorner_->GetPosition().Distance(cursorPnt) + .5f;
		length = static_cast<int>(fl);
		
		::PostMessage((HWND)(GetRenderContextSPtr()->GetHandle()), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_LINELENGTH_SET, length);
	}
	else
	{
		::PostMessage((HWND)(GetRenderContextSPtr()->GetHandle()), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_DLG_LINELENGTH_HIDE, 0);
	}

	imp_.CustomLength_ = boost::none;

	return false;
}

void RoomLayoutDrawingCtrller::PostRender3D()
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

bool RoomLayoutDrawingCtrller::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	if ( evt.EventType == EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_DRAWLINE )
	{
		imp_.Reset_ = true;

		return true;
	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT )
	{
		if ( evt.MouseInput.Event == EMIE_MOUSE_MOVED )
		{
			imp_.CursorIPos_.X = evt.MouseInput.X;
			imp_.CursorIPos_.Y = evt.MouseInput.Y;
		}

		if ( evt.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
		{
			imp_.LMousePressDown_ = true;
		}
	}

	if ( evt.EventType == EET_KEY_INPUT_EVENT )
	{
		if ( evt.KeyInput.Key == KEY_ESCAPE && evt.KeyInput.PressedDown )
		{
			imp_.EscPressDown_ = true;
		}
	}

	if ( evt.EventType == EET_USER_EVENT )
	{
		if ( evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_LINELENGTH_SET )
		{
			imp_.CustomLength_ = evt.UserEvent.UserData2;
			return true;
		}
	}

	return false;
}

void RoomLayoutDrawingCtrller::Init()
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

void RoomLayoutDrawingCtrller::Reset()
{
	auto& imp_ = *ImpUPtr_;

	imp_.EscPressDown_ = false;
	imp_.CustomLength_ = boost::none;
	imp_.CustomPnt_ = boost::none;
	imp_.LastWall_ = nullptr;

	for ( auto& curCorner : imp_.DrawingPathList_ )
	{
		curCorner->SetVisible(false);
	}
	imp_.DrawingPathList_.clear();
	imp_.State_ = EState::ES_READY;
}
