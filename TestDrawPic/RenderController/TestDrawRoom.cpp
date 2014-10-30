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
#include "BRepExtrema_DistShapeShape.hxx"

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

		PositionRect_ = new SMeshBuffer;
		PositionRect_->getMaterial().Lighting = false;
		PositionRect_->getMaterial().BackfaceCulling = false;
		PositionRect_->getMaterial().ZWriteEnable = false;
		PositionRect_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_VERTEX_ALPHA);//EMT_TRANSPARENT_ALPHA_CHANNEL;
		PositionRect_->getMaterial().MaterialTypeParam = 0.65f;

		FloatingLine_ = new SMeshBuffer;
		FloatingLine_->getMaterial().Lighting = false;
		FloatingLine_->getMaterial().BackfaceCulling = false;
		FloatingLine_->getMaterial().ZWriteEnable = false;
		FloatingLine_->getMaterial().Thickness = 4;
		FloatingLine_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);

		DrawingPath_ = new SMeshBuffer;
		DrawingPath_->getMaterial().Lighting = false;
		DrawingPath_->getMaterial().BackfaceCulling = false;
		DrawingPath_->getMaterial().ZWriteEnable = false;
		DrawingPath_->getMaterial().MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

		AuxiliaryLine_ = new SMeshBuffer;
		AuxiliaryLine_->getMaterial().Lighting = false;
		AuxiliaryLine_->getMaterial().BackfaceCulling = false;
		AuxiliaryLine_->getMaterial().ZWriteEnable = false;
		AuxiliaryLine_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
		AuxiliaryLine_->getMaterial().DiffuseColor = 0xFF000000;
		FloatingLine_->getMaterial().Thickness = 2;

		PressDown_ = false;
		Finish_ = false;
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

		if ( DrawingPath_ )
		{
			DrawingPath_->drop();
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
	
	//目前的线
	SMeshBuffer*				FloatingLine_;

	//目前的点
	SMeshBuffer*				PositionRect_;
	matrix4						PositionRectScaleMat_;
	matrix4						PositionRectTransMat_;

	//绘制路径点
	SMeshBuffer*				DrawingPath_;
	std::vector<vector3df>		DrawingPathList_;
	matrix4						DrawingPathScaleMat_;

	//Floating的起始点
	CornerODLSPtr				BeginCorner_;
	vector2di					CursorIPos_;

	//辅助线
	SMeshBuffer*				AuxiliaryLine_;
	
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

	gp_Pnt cursorPnt(planPos.X, 0, planPos.Z);
	auto alignDistance = 200.f;

	CornerODLSPtr foundCorner;

	for ( auto& curCorner : imp_.Graph_.lock()->GetAllCorners() )
	{
		if ( curCorner->GetPosition().SquareDistance(cursorPnt) < alignDistance * alignDistance )
		{
			cursorPnt = curCorner->GetPosition();
			foundCorner = curCorner;

			break;
		}
	}

	std::vector<BRepAdaptor_Curve> lineGroup;
	WallODLSPtr toSplit;
	BRepAdaptor_Curve ontoCurve;
	BRepAdaptor_Curve firstCurve,secondCurve;

	if ( !foundCorner )
	{		
		for ( auto& curWall : imp_.Graph_.lock()->GetAllWalls() )
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

			auto curPar = ppc.LowerDistanceParameter();
			auto isOnCurve = curPar > bc.FirstParameter() + curWall->GetThickness()/2 && curPar < bc.LastParameter()-curWall->GetThickness()/2;
			
			if ( !toSplit && isOnCurve)
			{
				toSplit = curWall;
				ontoCurve = bc;
				continue;
			}

			auto itor = std::find_if(lineGroup.begin(), lineGroup.end(), [&bc](const BRepAdaptor_Curve& curBC)
			{
				return Standard_True == bc.Line().Direction().IsParallel(curBC.Line().Direction(), Precision::Angular());
			});

			if ( itor == lineGroup.end() )
			{
				lineGroup.push_back(bc);
			}
		}

		if ( toSplit )
		{
			auto itor = std::remove_if(lineGroup.begin(), lineGroup.end(), [&ontoCurve](const BRepAdaptor_Curve& curBC)
			{
				return ontoCurve.Line().Direction().IsParallel(curBC.Line().Direction(), Precision::Angular());
			});
			lineGroup.erase(itor, lineGroup.end());
		}

		if ( toSplit )
		{
			firstCurve = ontoCurve;
		}
		else if ( !lineGroup.empty() )
		{
			firstCurve = lineGroup.front();
			lineGroup.erase(lineGroup.begin());
		}
		
		if ( GeomAbs_OtherCurve != firstCurve.GetType() )
		{
			auto foundSecond = false;
			
			if ( !lineGroup.empty() )
			{
				gp_Pln pln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));
				auto firstline2D = GeomAPI::To2d(firstCurve.Curve().Curve(), pln);

				for ( auto& curBC : lineGroup )
				{
					auto secondline2D = GeomAPI::To2d(curBC.Curve().Curve(), pln);

					Geom2dAPI_InterCurveCurve icc(firstline2D, secondline2D);
					assert(icc.NbPoints() == 1);

					auto iccPnt = icc.Point(1);
					gp_Pnt iccPnt3D(iccPnt.X(), 0, iccPnt.Y());

					if ( cursorPnt.SquareDistance(iccPnt3D) < alignDistance * alignDistance )
					{
						cursorPnt = iccPnt3D;
						secondCurve = curBC;
						foundSecond = true;
						break;
					}
				}				
			}

			if ( !foundSecond )
			{
				GeomAPI_ProjectPointOnCurve ppc(cursorPnt, firstCurve.Curve().Curve());
				cursorPnt = ppc.Point(1);
			}
		}
	}

	if ( !foundCorner && !toSplit && lineGroup.empty() && StatusMgr::GetInstance().GridAlign_ )
	{
		auto pos = *StatusMgr::GetInstance().GridAlign_;
		cursorPnt = gp_Pnt(pos.X, 0, pos.Z);
	}

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
						imp_.BeginCorner_ = imp_.Graph_.lock()->CreateCornerBySplitWall(toSplit, cursorPnt);
						for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(imp_.BeginCorner_) )
						{
							curWall->UpdateMesh();
						}
					}
					else
					{
						imp_.BeginCorner_ = imp_.Graph_.lock()->CreateCorner(cursorPnt);
					}
				}

				auto& pos = imp_.BeginCorner_->GetPosition();
				imp_.DrawingPathList_.emplace_back(static_cast<float>(pos.X()), static_cast<float>(pos.Y()), static_cast<float>(pos.Z()));
				imp_.State_ = EState::ES_DRAWING;
			}
		}
		break;
	case EState::ES_DRAWING:
		{
			bool valid = true;
			{//checkValid

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
						for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(imp_.BeginCorner_) )
						{
							auto beginCorner = curWall->GetFirstCorner().lock();
							auto endCorner = curWall->GetSecondCorner().lock();

							if ( beginCorner != imp_.BeginCorner_ )
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
						auto floatingEdge = BRepBuilderAPI_MakeEdge(imp_.BeginCorner_->GetPosition(), cursorPnt).Edge();
						Standard_Real floatingFirstPar,floatingLastPar;
						BRep_Tool::Range(floatingEdge, floatingFirstPar, floatingLastPar);

						for ( auto& curWall : imp_.Graph_.lock()->GetAllWalls() )
						{
							if ( curWall->GetFirstCorner().lock() == imp_.BeginCorner_ || curWall->GetSecondCorner().lock() == imp_.BeginCorner_ )
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
				imp_.FloatingLine_->getMaterial().DiffuseColor = 0xFF0000FF; 
			}
			else
			{
				imp_.FloatingLine_->getMaterial().DiffuseColor = 0xFFFF0000; 
			}

			if ( imp_.PressDown_ )
			{
				imp_.PressDown_ = false;

				if ( valid )
				{
					imp_.SetNeedUpdateMeshOnCorner(imp_.BeginCorner_);

					if ( foundCorner )
					{
						imp_.Graph_.lock()->AddWall(imp_.BeginCorner_, foundCorner);
						imp_.BeginCorner_ = foundCorner;
					}
					else
					{
						if ( toSplit )
						{
							auto newCorner = imp_.Graph_.lock()->CreateCornerBySplitWall(toSplit, cursorPnt);
							imp_.Graph_.lock()->AddWall(imp_.BeginCorner_, newCorner);
							imp_.BeginCorner_ = newCorner;
						}
						else
						{
							auto newCorner = imp_.Graph_.lock()->CreateCorner(cursorPnt);
							imp_.Graph_.lock()->AddWall(imp_.BeginCorner_, newCorner);
							imp_.BeginCorner_ = newCorner;
						}
					}

					auto& pos = imp_.BeginCorner_->GetPosition();
					imp_.DrawingPathList_.emplace_back(static_cast<float>(pos.X()), static_cast<float>(pos.Y()), static_cast<float>(pos.Z()));

					imp_.SetNeedUpdateMeshOnCorner(imp_.BeginCorner_);
				}
			}

			if ( imp_.Finish_ )
			{
				imp_.Finish_ = false;

				auto walls = imp_.Graph_.lock()->GetWallsOnCorner(imp_.BeginCorner_);
				if ( walls.empty() )
				{
					imp_.Graph_.lock()->RemoveCorner(imp_.BeginCorner_);
				}
				imp_.BeginCorner_ = nullptr;

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

	if ( GeomAbs_OtherCurve != firstCurve.GetType() )
	{
		gp_Pnt p1,p2;

		firstCurve.D0(-100000, p1);
		firstCurve.D0(100000, p2);

		imp_.AuxiliaryLine_->getPosition(0) = vector3df(static_cast<float>(p1.X()), static_cast<float>(p1.Y()), static_cast<float>(p1.Z()));
		imp_.AuxiliaryLine_->getPosition(1) = vector3df(static_cast<float>(p2.X()), static_cast<float>(p2.Y()), static_cast<float>(p2.Z()));
	}

	if ( GeomAbs_OtherCurve != secondCurve.GetType() )
	{
		gp_Pnt p1,p2;

		secondCurve.D0(-100000, p1);
		secondCurve.D0(100000, p2);

		imp_.AuxiliaryLine_->getPosition(2) = vector3df(static_cast<float>(p1.X()), static_cast<float>(p1.Y()), static_cast<float>(p1.Z()));
		imp_.AuxiliaryLine_->getPosition(3) = vector3df(static_cast<float>(p2.X()), static_cast<float>(p2.Y()), static_cast<float>(p2.Z()));
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
			if ( imp_.BeginCorner_ )
			{
				driver->setTransform(ETS_WORLD, matrix4());
				driver->setMaterial(imp_.FloatingLine_->getMaterial());
				driver->drawVertexPrimitiveList(imp_.FloatingLine_->getVertices(), imp_.FloatingLine_->getVertexCount(), imp_.FloatingLine_->getIndices(), imp_.FloatingLine_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
			}

			if ( !imp_.DrawingPathList_.empty() )
			{
				driver->setMaterial(imp_.DrawingPath_->getMaterial());
			}
			matrix4 mat;
			for ( auto& curPos : imp_.DrawingPathList_ )
			{
				mat.setTranslation(curPos);
				driver->setTransform(ETS_WORLD, mat*imp_.DrawingPathScaleMat_);
				driver->drawVertexPrimitiveList(imp_.DrawingPath_->getVertices(), imp_.DrawingPath_->getVertexCount(), imp_.DrawingPath_->getIndices(), imp_.DrawingPath_->getIndexCount()/2, EVT_STANDARD, EPT_TRIANGLE_FAN);
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
		imp_.DrawingPath_->Vertices.reallocate(4);
		imp_.DrawingPath_->Indices.reallocate(4);

		auto radius = 50.f;

		imp_.DrawingPath_->Vertices.push_back(S3DVertex(vector3df(-1, 0, -1), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
		imp_.DrawingPath_->Vertices.push_back(S3DVertex(vector3df(1, 0, -1), vector3df(0,1,0), SColor(~0), vector2df(1,0)));
		imp_.DrawingPath_->Vertices.push_back(S3DVertex(vector3df(1, 0, 1), vector3df(0,1,0), SColor(~0), vector2df(1,1)));
		imp_.DrawingPath_->Vertices.push_back(S3DVertex(vector3df(-1, 0, 1), vector3df(0,1,0), SColor(~0), vector2df(0,1)));

		imp_.DrawingPath_->Indices.push_back(0);
		imp_.DrawingPath_->Indices.push_back(1);
		imp_.DrawingPath_->Indices.push_back(2);
		imp_.DrawingPath_->Indices.push_back(3);

		imp_.DrawingPathScaleMat_.setScale(vector3df(radius, 1, radius));
		imp_.DrawingPath_->getMaterial().setTexture(0, GetRenderContextSPtr()->Smgr_->getVideoDriver()->getTexture("../Data/Resource/3D/dot.png"));
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
