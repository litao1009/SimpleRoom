#include "stdafx.h"

#include "irrEngine/SRenderContext.h"
#include "RoomLayoutWallCtrller.h"
#include "UserEvent.h"

#include "ODL/GraphODL.h"
#include "ODL/CornerODL.h"
#include "ODL/WallODL.h"


#include "BRepAdaptor_Curve.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "gp_Lin.hxx"
#include "gp_Ax3.hxx"
#include "gp_Pln.hxx"
#include "GeomAPI.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"
#include "Geom2dAPI_InterCurveCurve.hxx"


using namespace irr;
using namespace core;

enum class EWallState
{
	EWS_SWEEPING,
	//准备基线
	EWS_MOVING_INIT,
	//移动
	EWS_MOVING,
	EWS_PROPERTY,
	EWS_PROPERTY_WAIT,
	EWS_COUNT
};

class	SMoveInfo
{
public:

	SMoveInfo()
	{
		NeedSplit_ = false;
		NeedCreate_ = false;
		Valid_ = true;
	}

	bool				Valid_;
	bool				NeedSplit_;
	bool				NeedCreate_;
	CornerODLSPtr		CombineCorner_;
	WallODLSPtr			TrackWall_;
	BRepAdaptor_Curve	TrackCurve_;
};

class	RoomLayoutWallCtrller::Imp
{
public:

	Imp()
	{
		LMouseLeftUp_ = false;
		LMousePressDown_ = false;
		State_ = EWallState::EWS_SWEEPING;
	}

	SMoveInfo	MoveCorner(CornerODLSPtr& movingCorner, WallODLSPtr& movingWall, BRepAdaptor_Curve& wallCurve, BRepAdaptor_Curve& cursorCurve, Handle(Geom2d_Curve)& cursorBC2D, gp_Pnt& cursorPnt, gp_Vec& cursorVec)
	{
		SMoveInfo ret;

		static	gp_Pln	compareIntCCPln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));

		auto alignRadius = 200.0;
		auto wallDir = movingWall->GetDirection(movingCorner);
		auto cursorDir = gp_Dir(cursorVec);
		auto wallRefDir = wallDir.Crossed(cursorDir).Reversed();

		auto wallsOnCorner = Graph_.lock()->GetWallsOnCorner(movingCorner);
		wallsOnCorner.erase(std::remove(wallsOnCorner.begin(), wallsOnCorner.end(), movingWall), wallsOnCorner.end());
		std::sort(wallsOnCorner.begin(), wallsOnCorner.end(), [&wallDir, &wallRefDir, &movingCorner](const WallODLSPtr& wall1, const WallODLSPtr& wall2)
		{
			auto angle1 = wall1->GetDirection(movingCorner).AngleWithRef(wallDir, wallRefDir);
			angle1 = angle1 < 0 ? 2 * M_PI + angle1 : angle1;

			auto angle2 = wall2->GetDirection(movingCorner).AngleWithRef(wallDir, wallRefDir);
			angle2 = angle2 < 0 ? 2 * M_PI + angle2 : angle2;

			return angle1 < angle2;
		});

		ret.TrackWall_ = wallsOnCorner.empty() ? nullptr : wallsOnCorner.front();
		ret.TrackCurve_ = ret.TrackWall_ ? ret.TrackWall_->GetEdge(movingCorner) : BRepBuilderAPI_MakeEdge(gp_Lin(movingCorner->GetPosition(), cursorVec)).Edge();

		auto trackRad = ret.TrackCurve_.Line().Direction().AngleWithRef(wallDir, wallRefDir);
		trackRad = trackRad < 0 ? 2 * M_PI + trackRad : trackRad;

		if ( trackRad > Precision::Angular() && trackRad + Precision::Angular() < M_PI )//(0,180)
		{
			auto track2D = GeomAPI::To2d(ret.TrackCurve_.Curve().Curve(), compareIntCCPln);
			Geom2dAPI_InterCurveCurve icc(track2D, cursorBC2D);
			gp_Pnt newPnt(icc.Point(1).X(), 0, icc.Point(1).Y());
			GeomAPI_ProjectPointOnCurve ppc(newPnt, ret.TrackCurve_.Curve().Curve());

			if ( ppc.LowerDistanceParameter() > ret.TrackCurve_.LastParameter() || ret.TrackCurve_.LastParameter()-ppc.LowerDistanceParameter() < alignRadius )//是否需要融合
			{
				assert(ret.TrackWall_);

				auto endCorner = ret.TrackWall_->GetOtherCorner(movingCorner).lock();
				auto wallsOnEnd = Graph_.lock()->GetWallsOnCorner(endCorner);
				wallsOnEnd.erase(std::remove(wallsOnEnd.begin(), wallsOnEnd.end(), ret.TrackWall_), wallsOnEnd.end());

				//是否已经有其他的墙把位置占了
				auto alreadyHasWall = false;
				WallODLSPtr invalidWall;
				for ( auto& curWall : wallsOnEnd )
				{
					auto rad = curWall->GetDirection(endCorner).Angle(wallDir);
					if ( rad < 15 * M_PI / 180 )
					{
						invalidWall = curWall;
						alreadyHasWall = true;
						break;
					}
				}

				if ( alreadyHasWall )
				{//尝试调整位置
					gp_Vec disVec(movingCorner->GetPosition(), endCorner->GetPosition());
					disVec.Dot(cursorVec.Normalized());

					if ( disVec.Magnitude() - cursorVec.Magnitude() < invalidWall->GetThickness()/2 + movingWall->GetThickness()/2 )
					{//禁止移动
						ret.Valid_ = false;
					}
					else
					{
						auto cosRad = disVec.Normalized().Dot(cursorVec.Normalized());
						auto deltaPar = std::acos(invalidWall->GetThickness()/2);

						ret.TrackCurve_.D0(ret.TrackCurve_.LastParameter()-deltaPar, cursorPnt);

						cursorCurve = BRepBuilderAPI_MakeEdge(gp_Lin(cursorPnt, movingWall->GetDirection())).Edge();
						cursorBC2D = GeomAPI::To2d(cursorCurve.Curve().Curve(), compareIntCCPln);

						GeomAPI_ProjectPointOnCurve cursorProj(cursorPnt, wallCurve.Curve().Curve());
						cursorVec = gp_Vec(cursorProj.NearestPoint(), cursorPnt);
					}
				}
				else
				{//可以融合
					ret.CombineCorner_ = endCorner;

					cursorPnt = ret.CombineCorner_->GetPosition();
					cursorCurve = BRepBuilderAPI_MakeEdge(gp_Lin(cursorPnt, movingWall->GetDirection())).Edge();
					cursorBC2D = GeomAPI::To2d(cursorCurve.Curve().Curve(), compareIntCCPln);

					GeomAPI_ProjectPointOnCurve cursorProj(cursorPnt, wallCurve.Curve().Curve());
					cursorVec = gp_Vec(cursorProj.NearestPoint(), cursorPnt);
				}
			}
			else//是否需要拆分
			{
				auto hasDiffDirWall = std::adjacent_find(wallsOnCorner.begin(), wallsOnCorner.end(), [](const WallODLSPtr& wall1, const WallODLSPtr& wall2)
				{
					return Standard_False == wall1->GetDirection().IsParallel(wall2->GetDirection(), Precision::Angular());
				});

				if ( hasDiffDirWall != wallsOnCorner.end() )
				{
					ret.NeedSplit_ = true;
				}
			}

		}
		else if ( std::abs(trackRad-M_PI) < Precision::Angular() )//180
		{
			ret.TrackCurve_ = BRepBuilderAPI_MakeEdge(gp_Lin(movingCorner->GetPosition(), gp_Dir(cursorVec))).Edge();
			ret.TrackWall_ = nullptr;
			ret.NeedCreate_ = true;
		}
		else if ( trackRad > M_PI + Precision::Angular() )//(180, 360)
		{
			assert(!wallsOnCorner.empty());
			if ( 1 < wallsOnCorner.size() )
			{
				ret.NeedCreate_ = true;
			}
		}

		return ret;
	}

public:

	boost::optional<EUserType>		PropertyCallBack_;
	bool			LMouseLeftUp_;
	bool			LMousePressDown_;
	EWallState		State_;
	vector2di		CursorIPos_;
	vector3df		SavePos_;
	vector3df		CurrentPos_;
	SEventWallInfo	EventInfo_;
	GraphODLWPtr	Graph_;

	//for moving
	BRepAdaptor_Curve	BaseLin_;
	gp_Lin				OffsetLin_;
};

RoomLayoutWallCtrller::RoomLayoutWallCtrller( const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc ):IRoomLayoutODLBaseCtrller(rc), ImpUPtr_(new Imp)
{
	ImpUPtr_->Graph_ = graphODL;
}

RoomLayoutWallCtrller::~RoomLayoutWallCtrller()
{

}

void RoomLayoutWallCtrller::Init()
{

}

bool RoomLayoutWallCtrller::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

 	if ( evt.EventType == EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_WALL_PROPERTY )
 	{
		imp_.PropertyCallBack_ = static_cast<EUserType>(evt.UserEvent.UserData2);
 	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_MOUSE_MOVED )
	{
		imp_.CursorIPos_.X = evt.MouseInput.X;
		imp_.CursorIPos_.Y = evt.MouseInput.Y;
	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
	{
		imp_.LMousePressDown_ = true;
	}

	if ( evt.EventType == EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
	{
		imp_.LMouseLeftUp_ = true;
	}

	return false;
}

bool RoomLayoutWallCtrller::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	auto activeWall = std::static_pointer_cast<WallODL>(GetPickingODL().lock());

	plane3df plan(0,0,0,0,1,0);
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);
	imp_.CurrentPos_.Y = 0;
	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);

	activeWall->SetPicking(true);

	auto alignRadius = 200.0;

	switch (imp_.State_)
	{
	case EWallState::EWS_SWEEPING:
		{
			activeWall->SetSweeping(true);

			if ( imp_.LMousePressDown_ )
			{
				imp_.PropertyCallBack_ = boost::none;
				imp_.State_ = EWallState::EWS_PROPERTY;

				imp_.EventInfo_.Height_ = activeWall->GetHeight();
				imp_.EventInfo_.Thickness_ = activeWall->GetThickness();
				auto pointer = reinterpret_cast<int>(static_cast<void*>(&imp_.EventInfo_));
				::PostMessage((HWND)GetRenderContextSPtr()->GetHandle(), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_WALL_PROPERTY, pointer);
			}
		}
		break;
	case EWallState::EWS_MOVING_INIT:
		{
			imp_.BaseLin_ = BRepAdaptor_Curve(activeWall->GetEdge());
			imp_.State_ = EWallState::EWS_MOVING;
		}
	case EWallState::EWS_MOVING:
		{
			if ( imp_.LMouseLeftUp_ )
			{
				imp_.Graph_.lock()->SearchRooms();
				imp_.State_ = EWallState::EWS_SWEEPING;
				break;
			}

			//y==0平面
			static	gp_Pln	compareIntCCPln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));

			BRepAdaptor_Curve wallBC(activeWall->GetEdge());
			BRepAdaptor_Curve cursorBC(BRepBuilderAPI_MakeEdge(gp_Lin(cursorPnt, activeWall->GetDirection())));
			auto cursorBC2D = GeomAPI::To2d(cursorBC.Curve().Curve(), compareIntCCPln);

			//墙面到鼠标的垂直向量
			gp_Vec cursorVec;
			{
				GeomAPI_ProjectPointOnCurve cursorProj(cursorPnt, wallBC.Curve().Curve());
				if ( cursorProj.LowerDistance() < activeWall->GetThickness() )
				{//不变
					break;
				}
				cursorVec = gp_Vec(cursorProj.NearestPoint(), cursorPnt);
			}

			auto firstCorner = activeWall->GetFirstCorner().lock();
			auto secondCorner = activeWall->GetSecondCorner().lock();
			
			auto firstInfo = imp_.MoveCorner(firstCorner, activeWall, wallBC, cursorBC, cursorBC2D, cursorPnt, cursorVec);
			if ( !firstInfo.Valid_ )
			{
				break;
			}

			auto secondInfo = imp_.MoveCorner(secondCorner, activeWall, wallBC, cursorBC, cursorBC2D, cursorPnt, cursorVec);
			if ( !secondInfo.Valid_ )
			{
				break;
			}

			auto oldFirstPnt = firstCorner->GetPosition();
			auto oldSecondPnt = secondCorner->GetPosition();
			gp_Pnt firstPnt,secondPnt;
			{
				auto firstBC2D = GeomAPI::To2d(firstInfo.TrackCurve_.Curve().Curve(), compareIntCCPln);
				auto secondBC2D = GeomAPI::To2d(secondInfo.TrackCurve_.Curve().Curve(), compareIntCCPln);
				Geom2dAPI_InterCurveCurve firstICC(firstBC2D, cursorBC2D);
				Geom2dAPI_InterCurveCurve secondICC(secondBC2D, cursorBC2D);
				firstPnt = gp_Pnt(firstICC.Point(1).X(), 0, firstICC.Point(1).Y());
				secondPnt = gp_Pnt(secondICC.Point(1).X(), 0, secondICC.Point(1).Y());
			}

			if ( firstInfo.NeedSplit_ )
			{
				assert(firstInfo.TrackWall_);
				firstCorner = imp_.Graph_.lock()->CreateCornerBySplitWall(firstInfo.TrackWall_, firstPnt, false, false);
			}
			else if ( firstInfo.CombineCorner_ )
			{
				firstCorner = firstInfo.CombineCorner_;
			}
			else if ( firstInfo.NeedCreate_ )
			{
				auto newCorner = imp_.Graph_.lock()->CreateCorner(firstPnt);
				imp_.Graph_.lock()->AddWall(firstCorner, newCorner, false, false);
				firstCorner = newCorner;
			}
			else
			{
				firstCorner->SetPosition(firstPnt);
			}

			if ( secondInfo.NeedSplit_ )
			{
				assert(secondInfo.TrackWall_);
				secondCorner = imp_.Graph_.lock()->CreateCornerBySplitWall(secondInfo.TrackWall_, secondPnt, false, false);
			}
			else if ( secondInfo.CombineCorner_ )
			{
				secondCorner = secondInfo.CombineCorner_;
			}
			else if ( secondInfo.NeedCreate_ )
			{
				auto newCorner = imp_.Graph_.lock()->CreateCorner(secondPnt);
				imp_.Graph_.lock()->AddWall(secondCorner, newCorner, false, false);
				secondCorner = newCorner;
			}
			else
			{
				secondCorner->SetPosition(secondPnt);
			}

			if ( activeWall->GetFirstCorner().lock() != firstCorner || activeWall->GetSecondCorner().lock() != secondCorner )
			{
				auto newWall = imp_.Graph_.lock()->AddWall(firstCorner, secondCorner, false, false);
				imp_.Graph_.lock()->RemoveWall(activeWall, false, false, false);
				
				if ( firstInfo.CombineCorner_ )
				{
					auto walls = imp_.Graph_.lock()->GetWallsOnCorner(activeWall->GetFirstCorner().lock());
					if ( 1 == walls.size() )
					{
						imp_.Graph_.lock()->RemoveWall(walls.front(), false, false, false);
					}
					else
					{
						imp_.Graph_.lock()->MergeWallIfNeeded(activeWall->GetFirstCorner().lock(), false, false);
					}
				}
				if ( secondInfo.CombineCorner_ )
				{
					auto walls = imp_.Graph_.lock()->GetWallsOnCorner(activeWall->GetSecondCorner().lock());
					if ( 1 == walls.size() )
					{
						imp_.Graph_.lock()->RemoveWall(walls.front(), false, false, false);
					}
					else
					{
						imp_.Graph_.lock()->MergeWallIfNeeded(activeWall->GetSecondCorner().lock(), false, false);
					}
				}

				activeWall = newWall;
				SetPickingODL(activeWall);
			}

			for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(firstCorner) )
			{
				curWall->SetDirty(true);
			}

			for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(secondCorner) )
			{
				curWall->SetDirty(true);
			}

			imp_.Graph_.lock()->UpdateWallMeshIfNeeded();

			activeWall->SetPicking(true);
		}
		break;
	case EWallState::EWS_PROPERTY:
		{
			if ( !imp_.PropertyCallBack_ )
			{
				break;
			}

			switch (*(imp_.PropertyCallBack_))
			{
			case EUT_ROOMLAYOUT_WALL_NONE:
				{
					imp_.State_ = EWallState::EWS_SWEEPING;
				}
				break;
			case EUT_ROOMLAYOUT_WALL_UPDATE:
				{
					activeWall->SetHeight(imp_.EventInfo_.Height_);
					activeWall->SetThickness(imp_.EventInfo_.Thickness_);
					for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(activeWall->GetFirstCorner().lock()))
					{
						curWall->SetDirty(true);
					}
					for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(activeWall->GetSecondCorner().lock()))
					{
						curWall->SetDirty(true);
					}
					imp_.Graph_.lock()->UpdateWallMeshIfNeeded();
					imp_.State_ = EWallState::EWS_SWEEPING;
				}
				break;
			case EUT_ROOMLAYOUT_WALL_MOVE:
				{
					imp_.State_ = EWallState::EWS_MOVING_INIT;
				}
				break;
			case EUT_ROOMLAYOUT_WALL_DELETE:
				{
					imp_.Graph_.lock()->RemoveWall(activeWall);
					SetPickingODL(BaseODLSPtr());
					imp_.State_ = EWallState::EWS_SWEEPING;
				}
				break;
			default:
				assert(0);
				break;
			}
		}
		break;
	default:
		break;
	}

	imp_.LMouseLeftUp_ = false;
	imp_.LMousePressDown_ = false;

	return false;
}

void RoomLayoutWallCtrller::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	if ( GetPickingODL().expired() )
	{
		return;
	}

	auto activeWall = std::static_pointer_cast<WallODL>(GetPickingODL().lock());

	activeWall->SetPicking(false);

	switch (imp_.State_)
	{
	case EWallState::EWS_SWEEPING:
		{
			activeWall->SetSweeping(false);
		}
		break;
	case EWallState::EWS_MOVING_INIT:
		{

		}
		break;
	case EWallState::EWS_PROPERTY:
		{

		}
		break;
	default:
		break;
	}
}

bool RoomLayoutWallCtrller::CheckValid()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case EWallState::EWS_SWEEPING:
		{
			return false;
		}
		break;
	default:
		break;
	}

	return true;
}