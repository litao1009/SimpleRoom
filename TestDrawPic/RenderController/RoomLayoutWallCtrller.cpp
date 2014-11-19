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
	EWS_MOUSEHOLDING,
	//准备基线
	EWS_MOVING_INIT,
	//移动
	EWS_MOVING,
	EWS_PROPERTY,
	EWS_PROPERTY_WAIT,
	EWS_COUNT
};


class	RoomLayoutWallCtrller::Imp
{
public:

	Imp()
	{
		Cancel_ = false;
		HasSetProperty_ = false;
		LMouseLeftUp_ = false;
		LMousePressDown_ = false;
		State_ = EWallState::EWS_SWEEPING;
	}

	void	ResetMoving()
	{
		NeedCopyFirst_ = false;
		NeedCopySecond_ = false;
		FirstCorner_ = nullptr;
		SecondCorner_ = nullptr;
		FirstTrackWall1_ = nullptr;
		FirstTrackWall2_ = nullptr;
		SecondTrackWall1_ = nullptr;
		SecondTrackWall2_ = nullptr;
		FirstTrack1_ = BRepAdaptor_Curve();
		FirstTrack2_ = BRepAdaptor_Curve();
		SecondTrack1_ = BRepAdaptor_Curve();
		SecondTrack2_ = BRepAdaptor_Curve();
	}

public:

	bool			Cancel_;
	bool			HasSetProperty_;
	bool			LMouseLeftUp_;
	bool			LMousePressDown_;
	EWallState		State_;
	vector2di		CursorIPos_;
	vector3df		SavePos_;
	vector3df		CurrentPos_;
	SEventWallInfo	EventInfo_;
	GraphODLWPtr	Graph_;

	//for moving
	bool				NeedCopyFirst_;
	bool				NeedCopySecond_;
	CornerODLSPtr		FirstCorner_;
	CornerODLSPtr		SecondCorner_;
	BRepAdaptor_Curve	FirstTrack1_;
	BRepAdaptor_Curve	FirstTrack2_;
	BRepAdaptor_Curve	SecondTrack1_;
	BRepAdaptor_Curve	SecondTrack2_;
	WallODLSPtr			FirstTrackWall1_;
	WallODLSPtr			FirstTrackWall2_;
	WallODLSPtr			SecondTrackWall1_;
	WallODLSPtr			SecondTrackWall2_;
	BRepAdaptor_Curve	BaseLin_;
	gp_Lin				OffsetLin_;
	bool				Valid_;
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
		imp_.Cancel_ = evt.UserEvent.UserData2 != 1;
 		imp_.HasSetProperty_ = true;
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
				imp_.SavePos_ = imp_.CurrentPos_;
				imp_.State_ = EWallState::EWS_MOUSEHOLDING;
			}
		}
		break;
	case EWallState::EWS_MOUSEHOLDING:
		{
			auto thickness = activeWall->GetThickness();

			if ( imp_.SavePos_.getDistanceFromSQ(imp_.CurrentPos_) > thickness * thickness / 4 )
			{
				imp_.State_ = EWallState::EWS_MOVING_INIT;
			}
			else if ( imp_.LMouseLeftUp_ )
			{
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
			static	gp_Pln	compareIntCCPln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));

			if ( imp_.LMouseLeftUp_ )
			{
				imp_.State_ = EWallState::EWS_SWEEPING;
				break;
			}

			BRepAdaptor_Curve wallBC(activeWall->GetEdge());
			BRepAdaptor_Curve cursorBC(BRepBuilderAPI_MakeEdge(gp_Lin(cursorPnt, activeWall->GetDirection())));
			auto cursorBC2D = GeomAPI::To2d(cursorBC.Curve().Curve(), compareIntCCPln);

			gp_Vec cursorVec;
			{
				GeomAPI_ProjectPointOnCurve cursorProj(cursorPnt, wallBC.Curve().Curve());
				if ( cursorProj.LowerDistance() < activeWall->GetThickness() )
				{//不变
					//TODO
					break;
				}
				cursorVec = gp_Vec(cursorProj.NearestPoint(), cursorPnt);
			}

			WallODLSPtr firstTrackWall, secondTrackWall;
			BRepAdaptor_Curve firstTrackBC, secondTrackBC;
			auto needSplitFirst = false, needSplitSecond = false;
			auto needCreateFirst = false, needCreateSecond = false;
			auto needCombineFirst = false, needCombineSecond = false;
			CornerODLSPtr firstCombineCorner, secondCombineCorner;
			//WallODLSPtr firstToRemoveWall, secondToRemoveWall;

			imp_.Valid_ = true;

			{//first
				auto wallDir = activeWall->GetDirection();
				auto cursorDir = gp_Dir(cursorVec);
				auto wallRefDir = wallDir.Crossed(cursorDir).Reversed();

				imp_.FirstCorner_ = activeWall->GetFirstCorner().lock();

				auto wallsOnCorner = imp_.Graph_.lock()->GetWallsOnCorner(imp_.FirstCorner_);
				wallsOnCorner.erase(std::remove(wallsOnCorner.begin(), wallsOnCorner.end(), activeWall), wallsOnCorner.end());
				std::sort(wallsOnCorner.begin(), wallsOnCorner.end(), [&wallDir, &wallRefDir, &imp_](const WallODLSPtr& wall1, const WallODLSPtr& wall2)
				{
					auto angle1 = wall1->GetDirection(imp_.FirstCorner_).AngleWithRef(wallDir, wallRefDir);
					angle1 = angle1 < 0 ? 2 * M_PI + angle1 : angle1;

					auto angle2 = wall2->GetDirection(imp_.FirstCorner_).AngleWithRef(wallDir, wallRefDir);
					angle2 = angle2 < 0 ? 2 * M_PI + angle2 : angle2;

					return angle1 < angle2;
				});

				firstTrackWall = wallsOnCorner.empty() ? nullptr : wallsOnCorner.front();
				firstTrackBC = firstTrackWall ? firstTrackWall->GetEdge(imp_.FirstCorner_) : BRepBuilderAPI_MakeEdge(gp_Lin(imp_.FirstCorner_->GetPosition(), cursorVec));

				auto track2D = GeomAPI::To2d(firstTrackBC.Curve().Curve(), compareIntCCPln);
				Geom2dAPI_InterCurveCurve icc(track2D, cursorBC2D);
				gp_Pnt firstPnt(icc.Point(1).X(), 0, icc.Point(1).Y());
				GeomAPI_ProjectPointOnCurve firstPPC(firstPnt, firstTrackBC.Curve().Curve());
				
				auto hasDiffDirWall = std::adjacent_find(wallsOnCorner.begin(), wallsOnCorner.end(), [](const WallODLSPtr& wall1, const WallODLSPtr& wall2)
				{
					return Standard_False == wall1->GetDirection().IsParallel(wall2->GetDirection(), Precision::Angular());
				});

				if ( firstPPC.LowerDistanceParameter() > firstTrackBC.FirstParameter() )//(0,180)
				{
					if ( firstPPC.LowerDistanceParameter() > firstTrackBC.LastParameter() || firstTrackBC.LastParameter()-firstPPC.LowerDistanceParameter() < alignRadius )//吸附
					{
						assert(firstTrackWall);

						auto endCorner = firstTrackWall->GetOtherCorner(imp_.FirstCorner_).lock();
						auto wallsOnEnd = imp_.Graph_.lock()->GetWallsOnCorner(endCorner);
						wallsOnEnd.erase(std::find(wallsOnEnd.begin(), wallsOnEnd.end(), firstTrackWall), wallsOnEnd.end());

						auto valid = true;
						WallODLSPtr invalidWall;
						for ( auto& curWall : wallsOnEnd )
						{
							auto rad = curWall->GetDirection(endCorner).Angle(wallDir);

							if ( rad < 15 * M_PI / 180 )
							{
								invalidWall = curWall;
								valid = false;
								break;
							}
						}

						if ( !valid )
						{
							gp_Vec disVec(imp_.FirstCorner_->GetPosition(), endCorner->GetPosition());
							disVec.Dot(cursorVec.Normalized());

							if ( disVec.SquareMagnitude() < invalidWall->GetThickness()/2 + activeWall->GetThickness()/2 )
							{//禁止移动
								imp_.Valid_ = false;
							}
							else
							{
								auto cosRad = disVec.Normalized().Dot(cursorVec.Normalized());
								auto deltaPar = std::acos(invalidWall->GetThickness()/2);

								firstTrackBC.D0(firstTrackBC.LastParameter()-deltaPar, cursorPnt);

								cursorBC = BRepBuilderAPI_MakeEdge(gp_Lin(cursorPnt, activeWall->GetDirection())).Edge();
								cursorBC2D = GeomAPI::To2d(cursorBC.Curve().Curve(), compareIntCCPln);

								GeomAPI_ProjectPointOnCurve cursorProj(cursorPnt, wallBC.Curve().Curve());
								cursorVec = gp_Vec(cursorProj.NearestPoint(), cursorPnt);
							}
						}
						else
						{
							firstCombineCorner = endCorner;
							needCombineFirst = true;
						}
					}
					else if ( hasDiffDirWall != wallsOnCorner.end() )//需要拆分
					{
						needSplitFirst = true;
					}
				}
				else if ( firstTrackWall && Standard_True == firstTrackWall->GetDirection().IsParallel(activeWall->GetDirection(), Precision::Angular()) )//180
				{
					needCreateFirst = true;
				}
				else if ( firstPPC.LowerDistanceParameter() < firstTrackBC.FirstParameter() )//(180, 360)
				{
					assert(!wallsOnCorner.empty());
					if ( 1 < wallsOnCorner.size() )
					{
						needCreateFirst = true;
					}
				}
			}

			if ( !imp_.Valid_ )
			{
				break;
			}

			{//second
				auto wallDir = activeWall->GetDirection(activeWall->GetSecondCorner().lock());
				auto cursorDir = gp_Dir(cursorVec);
				auto wallRefDir = wallDir.Crossed(cursorDir).Reversed();

				imp_.SecondCorner_ = activeWall->GetSecondCorner().lock();

				auto wallsOnCorner = imp_.Graph_.lock()->GetWallsOnCorner(imp_.SecondCorner_);
				wallsOnCorner.erase(std::remove(wallsOnCorner.begin(), wallsOnCorner.end(), activeWall), wallsOnCorner.end());
				std::sort(wallsOnCorner.begin(), wallsOnCorner.end(), [&wallDir, &wallRefDir, &imp_](const WallODLSPtr& wall1, const WallODLSPtr& wall2)
				{
					auto angle1 = wall1->GetDirection(imp_.SecondCorner_).AngleWithRef(wallDir, wallRefDir);
					angle1 = angle1 < 0 ? 2 * M_PI + angle1 : angle1;

					auto angle2 = wall2->GetDirection(imp_.SecondCorner_).AngleWithRef(wallDir, wallRefDir);
					angle2 = angle2 < 0 ? 2 * M_PI + angle2 : angle2;

					return angle1 < angle2;
				});

				secondTrackWall = wallsOnCorner.empty() ? nullptr : wallsOnCorner.front();
				secondTrackBC = secondTrackWall ? secondTrackWall->GetEdge(imp_.SecondCorner_) : BRepBuilderAPI_MakeEdge(gp_Lin(imp_.SecondCorner_->GetPosition(), cursorVec));
				needSplitSecond = secondTrackWall && secondTrackWall->GetDirection().IsParallel(activeWall->GetDirection(), Precision::Angular());

				auto track2D = GeomAPI::To2d(secondTrackBC.Curve().Curve(), compareIntCCPln);
				Geom2dAPI_InterCurveCurve icc(track2D, cursorBC2D);
				gp_Pnt secondPnt(icc.Point(1).X(), 0, icc.Point(1).Y());
				GeomAPI_ProjectPointOnCurve secondPPC(secondPnt, secondTrackBC.Curve().Curve());

				auto hasDiffDirWall = std::adjacent_find(wallsOnCorner.begin(), wallsOnCorner.end(), [](const WallODLSPtr& wall1, const WallODLSPtr& wall2)
				{
					return Standard_False == wall1->GetDirection().IsParallel(wall2->GetDirection(), Precision::Angular());
				});

				if ( secondPPC.LowerDistanceParameter() > secondTrackBC.FirstParameter() )//(0,180)
				{
					if ( secondPPC.LowerDistanceParameter() > secondTrackBC.LastParameter() || secondTrackBC.LastParameter()-secondPPC.LowerDistanceParameter() < alignRadius )//吸附
					{
						assert(secondTrackWall);

						auto endCorner = secondTrackWall->GetOtherCorner(imp_.SecondCorner_).lock();
						auto wallsOnEnd = imp_.Graph_.lock()->GetWallsOnCorner(endCorner);
						wallsOnEnd.erase(std::find(wallsOnEnd.begin(), wallsOnEnd.end(), secondTrackWall), wallsOnEnd.end());

						auto valid = true;
						WallODLSPtr invalidWall;
						for ( auto& curWall : wallsOnEnd )
						{
							auto rad = curWall->GetDirection(endCorner).Angle(wallDir);

							if ( rad < 15 * M_PI / 180 )
							{
								invalidWall = curWall;
								valid = false;
								break;
							}
						}

						if ( !valid )
						{
							gp_Vec disVec(imp_.SecondCorner_->GetPosition(), endCorner->GetPosition());
							disVec.Dot(cursorVec.Normalized());

							if ( disVec.SquareMagnitude() < invalidWall->GetThickness()/2 + activeWall->GetThickness()/2 )
							{//禁止移动
								imp_.Valid_ = false;
							}
							else
							{
								auto cosRad = disVec.Normalized().Dot(cursorVec.Normalized());
								auto deltaPar = std::acos(invalidWall->GetThickness()/2);

								secondTrackBC.D0(secondTrackBC.LastParameter()-deltaPar, cursorPnt);

								cursorBC = BRepBuilderAPI_MakeEdge(gp_Lin(cursorPnt, activeWall->GetDirection())).Edge();
								cursorBC2D = GeomAPI::To2d(cursorBC.Curve().Curve(), compareIntCCPln);

								GeomAPI_ProjectPointOnCurve cursorProj(cursorPnt, wallBC.Curve().Curve());
								cursorVec = gp_Vec(cursorProj.NearestPoint(), cursorPnt);
							}
						}
						else
						{
							secondCombineCorner = endCorner;
							needCombineSecond = true;
						}
					}
					else if ( hasDiffDirWall != wallsOnCorner.end() )//需要拆分
					{
						needSplitSecond = true;
					}
				}
				else if ( secondTrackWall && Standard_True == secondTrackWall->GetDirection().IsParallel(activeWall->GetDirection(), Precision::Angular()) )//180
				{
					needCreateSecond = true;
				}
				else if ( secondPPC.LowerDistanceParameter() < secondTrackBC.FirstParameter() )//(180, 360)
				{
					assert(!wallsOnCorner.empty());
					if ( 1 < wallsOnCorner.size() )
					{
						needCreateSecond = true;
					}
				}

// 				else if ( secondTrackWall && Standard_True == secondTrackWall->GetDirection().IsParallel(activeWall->GetDirection(), Precision::Angular()) )
// 				{
// 					needCreateSecond = true;
// 				}
// 				else if ( secondPPC.LowerDistanceParameter() < secondTrackBC.FirstParameter() )
// 				{
// 					assert(!wallsOnCorner.empty());
// 					if ( 1 < wallsOnCorner.size() )
// 					{
// 						needCreateSecond = true;
// 					}
// 				}
// 				else if ( (secondTrackBC.LastParameter() > secondPPC.LowerDistanceParameter() && secondTrackBC.LastParameter()-secondPPC.LowerDistanceParameter() < alignRadius ) ||
// 					secondPPC.LowerDistanceParameter() > secondTrackBC.LastParameter() )
// 				{
// 					assert(secondTrackWall);
// 
// 					auto endCorner = secondTrackWall->GetOtherCorner(imp_.SecondCorner_).lock();
// 					auto wallsOnEnd = imp_.Graph_.lock()->GetWallsOnCorner(endCorner);
// 					wallsOnEnd.erase(std::find(wallsOnEnd.begin(), wallsOnEnd.end(), secondTrackWall), wallsOnEnd.end());
// 
// 					auto valid = true;
// 					WallODLSPtr invalidWall;
// 					for ( auto& curWall : wallsOnEnd )
// 					{
// 						auto rad = curWall->GetDirection(endCorner).Angle(wallDir);
// 
// 						if ( rad < 15 * M_PI / 180 )
// 						{
// 							invalidWall = curWall;
// 							valid = false;
// 							break;
// 						}
// 					}
// 
// 					if ( !valid )
// 					{
// 						gp_Vec disVec(imp_.SecondCorner_->GetPosition(), endCorner->GetPosition());
// 						disVec.Dot(cursorVec.Normalized());
// 
// 						if ( disVec.SquareMagnitude() < invalidWall->GetThickness()/2 + activeWall->GetThickness()/2 )
// 						{//禁止移动
// 							imp_.Valid_ = false;
// 						}
// 						else
// 						{
// 							auto cosRad = disVec.Normalized().Dot(cursorVec.Normalized());
// 							auto deltaPar = std::acos(invalidWall->GetThickness()/2);
// 
// 							secondTrackBC.D0(secondTrackBC.LastParameter()-deltaPar, cursorPnt);
// 
// 							cursorBC = BRepBuilderAPI_MakeEdge(gp_Lin(cursorPnt, activeWall->GetDirection())).Edge();
// 							cursorBC2D = GeomAPI::To2d(cursorBC.Curve().Curve(), compareIntCCPln);
// 
// 							GeomAPI_ProjectPointOnCurve cursorProj(cursorPnt, wallBC.Curve().Curve());
// 							cursorVec = gp_Vec(cursorProj.NearestPoint(), cursorPnt);
// 						}
// 					}
// 					else
// 					{
// 						secondCombineCorner = endCorner;
// 						needCombineSecond = true;
// 					}
// 				}
			}

			if ( !imp_.Valid_ )
			{
				break;
			}

			gp_Pnt oldFirstPnt, oldSecondPnt;
			gp_Pnt firstPnt,secondPnt;
			{
				auto firstBC2D = GeomAPI::To2d(firstTrackBC.Curve().Curve(), compareIntCCPln);
				auto secondBC2D = GeomAPI::To2d(secondTrackBC.Curve().Curve(), compareIntCCPln);
				Geom2dAPI_InterCurveCurve firstICC(firstBC2D, cursorBC2D);
				Geom2dAPI_InterCurveCurve secondICC(secondBC2D, cursorBC2D);
				firstPnt = gp_Pnt(firstICC.Point(1).X(), 0, firstICC.Point(1).Y());
				secondPnt = gp_Pnt(secondICC.Point(1).X(), 0, secondICC.Point(1).Y());
				oldFirstPnt = imp_.FirstCorner_->GetPosition();
				oldSecondPnt = imp_.SecondCorner_->GetPosition();
			}

			if ( needSplitFirst )
			{
				if ( firstTrackWall )
				{
					imp_.FirstCorner_ = imp_.Graph_.lock()->CreateCornerBySplitWall(firstTrackWall, firstPnt);
				}
				else
				{
					auto newCorner = imp_.Graph_.lock()->CreateCorner(firstPnt);
					imp_.Graph_.lock()->AddWall(imp_.FirstCorner_, newCorner);
					imp_.FirstCorner_ = newCorner;
				}
			}

			if ( needSplitSecond )
			{
				if ( secondTrackWall )
				{
					imp_.SecondCorner_ = imp_.Graph_.lock()->CreateCornerBySplitWall(secondTrackWall, secondPnt);
				}
				else
				{
					auto newCorner = imp_.Graph_.lock()->CreateCorner(secondPnt);
					imp_.Graph_.lock()->AddWall(imp_.SecondCorner_, newCorner);
					imp_.SecondCorner_ = newCorner;
				}
			}

			if ( needCreateFirst )
			{
				auto newCorner = imp_.Graph_.lock()->CreateCorner(firstPnt);
				imp_.Graph_.lock()->AddWall(imp_.FirstCorner_, newCorner);
				imp_.FirstCorner_ = newCorner;
			}

			if ( needCreateSecond )
			{
				auto newCorner = imp_.Graph_.lock()->CreateCorner(secondPnt);
				imp_.Graph_.lock()->AddWall(imp_.SecondCorner_, newCorner);
				imp_.SecondCorner_ = newCorner;
			}

			if ( needCombineFirst )
			{
				imp_.FirstCorner_ = firstCombineCorner;
			}

			if ( needCombineSecond )
			{
				imp_.SecondCorner_ = secondCombineCorner;
			}

			imp_.FirstCorner_->SetPosition(firstPnt);
			imp_.SecondCorner_->SetPosition(secondPnt);

			if ( activeWall->GetFirstCorner().lock() != imp_.FirstCorner_ || activeWall->GetSecondCorner().lock() != imp_.SecondCorner_ )
			{
				auto newWall = imp_.Graph_.lock()->AddWall(imp_.FirstCorner_, imp_.SecondCorner_);
				newWall->SetPicking(true);
				SetPickingODL(newWall);
				imp_.Graph_.lock()->RemoveWall(activeWall);
				activeWall = newWall;
			}

			if ( oldFirstPnt.SquareDistance(firstPnt) > 1 || oldSecondPnt.SquareDistance(secondPnt) > 1)
			{
				for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(imp_.FirstCorner_) )
				{
					curWall->UpdateBaseMesh();
				}

				for ( auto& curWall : imp_.Graph_.lock()->GetWallsOnCorner(imp_.SecondCorner_) )
				{
					curWall->UpdateBaseMesh();
				}
			}
			
			activeWall->SetPicking(true);
		}
		break;
	case EWallState::EWS_PROPERTY:
		{
			if ( imp_.HasSetProperty_ )
			{
				if ( imp_.Cancel_ )
				{
					imp_.State_ = EWallState::EWS_SWEEPING;
					break;
				}

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
				for ( auto& curWall : imp_.Graph_.lock()->GetAllWalls() )
				{
					if ( curWall->IsDirty() )
					{
						curWall->UpdateBaseMesh();
						curWall->SetDirty(false);
					}
				}
				imp_.State_ = EWallState::EWS_SWEEPING;
			}
		}
		break;
	default:
		break;
	}

	imp_.Cancel_ = false;
	imp_.LMouseLeftUp_ = false;
	imp_.LMousePressDown_ = false;
	imp_.HasSetProperty_ = false;

	return false;
}

void RoomLayoutWallCtrller::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	auto activeWall = std::static_pointer_cast<WallODL>(GetPickingODL().lock());

	activeWall->SetPicking(false);

	switch (imp_.State_)
	{
	case EWallState::EWS_SWEEPING:
		{
			activeWall->SetSweeping(false);
		}
		break;
	case EWallState::EWS_MOUSEHOLDING:
		{
			
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