#include "stdafx.h"

#include "RoomLayoutPillarController.h"
#include "RenderController/UserEvent.h"
#include "irrEngine/SRenderContext.h"

#include "ODL/BaseODL.h"
#include "ODL/PillarODL.h"
#include "ODL/RoomODL.h"
#include "ODL/WallODL.h"

#include "TopoDS.hxx"
#include "gp_Pln.hxx"
#include "gp_Lin.hxx"
#include "BRepExtrema_DistShapeShape.hxx"
#include "BRepPrimAPI_MakeBox.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "TopExp_Explorer.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "GeomAPI.hxx"
#include "Geom2dAPI_InterCurveCurve.hxx"

#include <boost/optional.hpp>

using namespace irr;
using namespace core;

enum class EPilarState
{
	EPS_SWEEPING,
	EPS_MODIFY_INIT,
	EPS_MODIFY,
	EPS_CREATING_INIT,
	EPS_MOUSEHOLDING,
	EPS_MOVING,
	EPS_PROPERTY,
	EPS_COUNT
};

class	RoomLayoutPillarController::Imp
{
public:

	Imp()
	{
		State_ = EPilarState::EPS_SWEEPING;
		EscapePressDown_ = false;
		LMouseLeftUp_ = false;
		LMousePressDown_ = false;
		Valid_ = false;
		CtrllHolding_ = false;
	}

public:

	bool			EscapePressDown_;
	bool			LMouseLeftUp_;
	bool			LMousePressDown_;
	bool			CtrllHolding_;
	bool			Valid_;
	EPilarState		State_;
	vector2di		CursorIPos_;
	vector3df		CurrentPos_;
	vector3df		SavePos_;
	gp_Dir			ModifyDir_;

	boost::optional<EUserType>			PropertyCallBack_;
	boost::optional<SEventPillarInfo>	EventInfo_;			
};

RoomLayoutPillarController::RoomLayoutPillarController(const BaseODLSPtr& baseODL, const SRenderContextWPtr& rc):IRoomLayoutODLBaseCtrller(rc),ImpUPtr_(new Imp)
{
	RootODL_ = baseODL;
}

RoomLayoutPillarController::~RoomLayoutPillarController()
{

}

void RoomLayoutPillarController::Init()
{

}

bool RoomLayoutPillarController::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	if ( evt.EventType == EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_PILLAR_PROPERTY )
	{
		imp_.PropertyCallBack_ = static_cast<EUserType>(evt.UserEvent.UserData2);
	}

	if ( evt.EventType == irr::EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_TEST_PILLAR )
	{
		auto pilarInfo = static_cast<SEventPillarInfo*>(reinterpret_cast<void*>(evt.UserEvent.UserData2));
		imp_.EventInfo_ = *pilarInfo;
		imp_.State_ = EPilarState::EPS_CREATING_INIT;
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
	{
		imp_.CursorIPos_.X = evt.MouseInput.X;
		imp_.CursorIPos_.Y = evt.MouseInput.Y;
	}

	if ( evt.EventType == irr::EET_KEY_INPUT_EVENT && evt.KeyInput.Key == irr::KEY_ESCAPE && evt.KeyInput.PressedDown )
	{
		imp_.EscapePressDown_ = true;
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP )
	{
		imp_.LMouseLeftUp_ = true;
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
	{
		imp_.LMousePressDown_ = true;;
	}

	if ( evt.EventType == EET_KEY_INPUT_EVENT && evt.KeyInput.Key == KEY_LCONTROL )
	{
		imp_.CtrllHolding_ = evt.KeyInput.PressedDown;
	}

	return false;
}


bool RoomLayoutPillarController::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	plane3df plan(0,0,0,0,1,0);
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);
	imp_.CurrentPos_.Y = 0;
	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);

	switch (imp_.State_)
	{
	case EPilarState::EPS_SWEEPING:
		{
			if ( GetPickingODL().expired() )
			{
				break;
			}

			auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());

			activePilar->SetSweeping(true);

			if ( imp_.CtrllHolding_ )
			{
				imp_.State_ = EPilarState::EPS_MODIFY_INIT;
				break;
			}

			if ( imp_.LMousePressDown_ )
			{
				auto box = activePilar->GetBaseBndBox();
				auto pos = activePilar->GetTranslation();

				Standard_Real xMin,xMax,yMin,yMax,zMin,zMax;
				box.Get(xMin, yMin, zMin, xMax, yMax, zMax);

				imp_.EventInfo_ = SEventPillarInfo();
				imp_.EventInfo_->XLength_ = static_cast<float>(xMax-xMin);
				imp_.EventInfo_->YLength_ = static_cast<float>(yMax-yMin);
				imp_.EventInfo_->ZLength_ = static_cast<float>(zMax-zMin);
				imp_.EventInfo_->OffsetHeight_ = activePilar->GetOffsetHeight();

				imp_.SavePos_ = imp_.CurrentPos_;
				imp_.State_ = EPilarState::EPS_MOUSEHOLDING;
			}
		}
		break;
	case EPilarState::EPS_MODIFY_INIT:
		{
			auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
			activePilar->SetSweeping(true);

			if ( !imp_.CtrllHolding_ )
			{
				GetRenderContextSPtr()->CursorControl_->setActiveIcon(gui::ECI_NORMAL);
				imp_.State_ = EPilarState::EPS_SWEEPING;
				imp_.CtrllHolding_ = false;
				break;
			}

			auto box = activePilar->GetBaseBndBox();
			auto pos = activePilar->GetAbsoluteTransform();

			Standard_Real xMin,xMax,yMin,yMax,zMin,zMax;
			box.Get(xMin, yMin, zMin, xMax, yMax, zMax);

			auto modifyAlign = 100;

			Bnd_Box smallBox,bigBox;
			smallBox.Update(xMin+modifyAlign < 0 ? xMin+modifyAlign : 0, yMin+modifyAlign < 0 ? yMin+modifyAlign : 0, zMin+modifyAlign < 0 ? zMin+modifyAlign : 0,
				xMax-modifyAlign > 0 ? xMax-modifyAlign : 0, yMax-modifyAlign > 0 ? yMax-modifyAlign : 0, zMax-modifyAlign > 0 ? zMax-modifyAlign : 0);
			bigBox.Update(xMin-modifyAlign, yMin-modifyAlign, zMin-modifyAlign, xMax+modifyAlign, yMax+modifyAlign, zMax+modifyAlign);

			auto relationCursorPnt = cursorPnt.Transformed(activePilar->GetAbsoluteTransform().Inverted());
			relationCursorPnt.SetY(0);

			if ( Standard_True == bigBox.IsOut(relationCursorPnt) )
			{
				GetRenderContextSPtr()->CursorControl_->setActiveIcon(gui::ECI_NORMAL);
				imp_.State_ = EPilarState::EPS_SWEEPING;
				imp_.CtrllHolding_ = false;
				break;
			}

			if ( Standard_False == smallBox.IsOut(relationCursorPnt) )
			{
				GetRenderContextSPtr()->CursorControl_->setActiveIcon(gui::ECI_NORMAL);
				break;
			}

			gp_Dir cursorDir = gp_Vec(gp::Origin(), relationCursorPnt);
			{
				auto rad = cursorDir.Angle(gp::DX());
				rad = rad > M_PI_2 ? M_PI - rad : rad;

				if ( rad < M_PI_4 )
				{
					cursorDir.Rotate(gp::OY(), rad);
					GetRenderContextSPtr()->CursorControl_->setActiveIcon(gui::ECI_SIZEWE);
				}
				else
				{
					cursorDir.Rotate(gp::OY(), M_PI_2 - rad);
					GetRenderContextSPtr()->CursorControl_->setActiveIcon(gui::ECI_SIZENS);
				}
			}

			imp_.ModifyDir_ = cursorDir;

			if ( imp_.LMousePressDown_ )
			{
				imp_.State_ = EPilarState::EPS_MODIFY;
				break;
			}
		}
		break;
	case EPilarState::EPS_MODIFY:
		{
			auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
			activePilar->SetPicking(true);


		}
		break;
	case EPilarState::EPS_MOUSEHOLDING:
		{
			auto activePillar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
			activePillar->SetPicking(true);

			if ( imp_.SavePos_.getDistanceFromSQ(imp_.CurrentPos_) > 100 * 100 )
			{
				imp_.State_ = EPilarState::EPS_MOVING;
				imp_.Valid_ = true;
			}
			else 
				if ( imp_.LMouseLeftUp_ )
				{
					imp_.PropertyCallBack_ = boost::none;
					imp_.State_ = EPilarState::EPS_PROPERTY;

					auto pointer = reinterpret_cast<int>(static_cast<void*>(&(*imp_.EventInfo_)));
					::PostMessage((HWND)GetRenderContextSPtr()->GetHandle(), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_PILLAR_PROPERTY, pointer);
				}
		}
		break;
	case EPilarState::EPS_PROPERTY:
		{
			if ( !imp_.PropertyCallBack_ )
			{
				break;
			}

			auto activePillar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());

			switch (*(imp_.PropertyCallBack_))
			{
			case EUT_ROOMLAYOUT_PILLAR_NONE:
				{
					imp_.State_ = EPilarState::EPS_SWEEPING;
				}
				break;
			case EUT_ROOMLAYOUT_PILLAR_UPDATE:
				{
					activePillar->SetSize(imp_.EventInfo_->XLength_, imp_.EventInfo_->YLength_, imp_.EventInfo_->ZLength_);
					activePillar->SetOffsetHeight(imp_.EventInfo_->OffsetHeight_);

					{
						auto curTrans = activePillar->GetTranslation();
						curTrans.SetY(imp_.EventInfo_->YLength_/2 + imp_.EventInfo_->OffsetHeight_);
						activePillar->SetTranslation(curTrans);

						auto curPos = activePillar->GetDataSceneNode()->getPosition();
						curPos.Y = static_cast<float>(imp_.EventInfo_->YLength_/2 + imp_.EventInfo_->OffsetHeight_);
						activePillar->GetDataSceneNode()->setPosition(curPos);
					}

					activePillar->UpdateMesh();
					imp_.State_ = EPilarState::EPS_SWEEPING;
				}
				break;
			case EUT_ROOMLAYOUT_PILLAR_MOVE:
				{
					imp_.State_ = EPilarState::EPS_MOVING;
				}
				break;
			case EUT_ROOMLAYOUT_PILLAR_DELETE:
				{
					activePillar->RemoveFromParent();
					imp_.State_ = EPilarState::EPS_SWEEPING;
				}
				break;
			default:
				assert(0);
				break;
			}
		}
		break;
	case EPilarState::EPS_CREATING_INIT:
		{
			assert(imp_.EventInfo_);

			auto newPilar = PillarODL::Create<PillarODL>(GetRenderContextSPtr());
			newPilar->SetSize(imp_.EventInfo_->XLength_, imp_.EventInfo_->YLength_, imp_.EventInfo_->ZLength_);
			newPilar->SetOffsetHeight(imp_.EventInfo_->OffsetHeight_);
			newPilar->UpdateMesh();

			RootODL_.lock()->AddChild(newPilar);

			auto newPos = imp_.CurrentPos_;
			newPos.Y = imp_.EventInfo_->YLength_/2;

			newPilar->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
			newPilar->GetDataSceneNode()->setPosition(newPos);
			SetPickingODL(newPilar);

			imp_.State_ = EPilarState::EPS_MOVING;
		}
		break;
	case EPilarState::EPS_MOVING:
		{
			auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
			activePilar->SetPicking(true);
			activePilar->SetVaildPosition(imp_.Valid_);

			if ( imp_.LMouseLeftUp_ )
			{
				if ( imp_.Valid_ )
				{
					ImpUPtr_->Valid_ = false;
					ImpUPtr_->EventInfo_ = boost::none;

					imp_.State_ = EPilarState::EPS_SWEEPING;
					break;
				}
			}

			if ( imp_.EscapePressDown_ )
			{
				activePilar->RemoveFromParent();

				imp_.State_ = EPilarState::EPS_SWEEPING;
				break;
			}
			
			//当前柱的Box
			auto activeTransitionBox = activePilar->GetBaseBndBox();
			{
				gp_Trsf tfs;
				tfs.SetTranslationPart(activePilar->GetTranslation());
				activeTransitionBox = activeTransitionBox.Transformed(tfs);
			}

			activePilar->SetRotation(gp_Quaternion(gp::DZ(), gp::DZ()));

			auto size = activePilar->GetSize();
			auto offset = activePilar->GetOffsetHeight();

			//新的位置
			auto newPos = imp_.CurrentPos_;
			newPos.Y = static_cast<float>(size.Y()/2) + offset;
			gp_Pnt newPnt(newPos.X, newPos.Y, newPos.Z);

			//吸附距离
			static auto alignDis = 200.0;

			//当前的吸附物体
			auto alignODLs = activePilar->GetAlignList();
			alignODLs.erase(std::remove_if(alignODLs.begin(), alignODLs.end(), [&activePilar, &activeTransitionBox](const BaseODLSPtr& alignODL)
			{
				if ( !alignODL )
				{
					return true;
				}

				return alignODL->GetBaseBndBox().Distance(activeTransitionBox.Transformed(alignODL->GetAbsoluteTransform().Inverted())) > alignDis;
			}), alignODLs.end());
			activePilar->SetAlignList(BaseODLList());

			std::multimap<double,BaseODLSPtr> disMap;
			for ( auto& curODL : RootODL_.lock()->GetChildrenList() )
			{
				if ( EODLT_PILLAR != curODL->GetType() && EODLT_WALL != curODL->GetType() )
				{
					continue;
				}

				if ( std::find(alignODLs.begin(), alignODLs.end(), curODL) != alignODLs.end() )
				{
					continue;
				}

				if ( EODLT_PILLAR == curODL->GetType() && curODL == activePilar )
				{
					continue;
				}

				auto dis = curODL->GetBaseBndBox().Distance(activeTransitionBox.Transformed(curODL->GetAbsoluteTransform().Inverted()));
				if ( dis > alignDis )
				{
					continue;
				}

				disMap.emplace(dis, curODL);
			}

			for ( auto& curODL : disMap )
			{
				alignODLs.push_back(curODL.second);
			}

			//锁定旋转
			auto lockRotation = false;
			//锁定位置
			auto lockPosition = false;
			//移动方向
			TopoDS_Edge movingEdge;
			for ( auto& curODL : alignODLs )
			{
				auto alignTransformation = curODL->GetAbsoluteTransform();

				//如果锁定了旋转，则调整柱的box
				auto activeRotationPillarBox = activePilar->GetBaseBndBox();
				if ( lockRotation )
				{
					gp_Trsf tfsODL, tfsPillar;

					tfsODL.SetRotation(alignTransformation.GetRotation());
					tfsPillar.SetRotation(activePilar->GetRotation());

					activeRotationPillarBox = activeRotationPillarBox.Transformed(tfsODL.Inverted() * tfsPillar);
				}
				
				Bnd_Box movingBox;
				{
					auto curBox = curODL->GetBaseBndBox();

					Standard_Real xAlignMin,yAlignMin,zAlignMin,xAlignMax,yAlignMax,zAlignMax;
					curBox.Get(xAlignMin, yAlignMin, zAlignMin, xAlignMax, yAlignMax, zAlignMax);

					Standard_Real xPillarMin,yPillarMin,zPillarMin,xPillarMax,yPillarMax,zPillarMax;
					activeRotationPillarBox.Get(xPillarMin, yPillarMin, zPillarMin, xPillarMax, yPillarMax, zPillarMax);

					movingBox.Update(xAlignMin+xPillarMin, yAlignMin+yPillarMin, zAlignMin+zPillarMin,
						xAlignMax+xPillarMax, yAlignMax+yPillarMax, zAlignMax+zPillarMax);
				}
				
				auto inODLPnt = newPnt.Transformed(curODL->GetAbsoluteTransform().Inverted());

				//当位置锁定了以后，只需要判断是不是相交
				if ( lockPosition )
				{
					if ( Standard_True == movingBox.IsOut(inODLPnt) )
					{
						continue;
					}

					Bnd_Box pntBox;
					pntBox.Add(inODLPnt);

					if ( pntBox.Distance(movingBox) < Precision::Confusion() )
					{
						continue;
					}

					imp_.Valid_ = false;
					break;
				}

				Bnd_Box alignODLBox;
				{
					auto curBox = curODL->GetBaseBndBox();

					Standard_Real xAlignMin,yAlignMin,zAlignMin,xAlignMax,yAlignMax,zAlignMax;
					curBox.Get(xAlignMin, yAlignMin, zAlignMin, xAlignMax, yAlignMax, zAlignMax);

					Standard_Real xPillarMin,yPillarMin,zPillarMin,xPillarMax,yPillarMax,zPillarMax;
					activeRotationPillarBox.Get(xPillarMin, yPillarMin, zPillarMin, xPillarMax, yPillarMax, zPillarMax);

					alignODLBox.Update(xAlignMin+xPillarMin-alignDis, yAlignMin+yPillarMin-alignDis, zAlignMin+zPillarMin-alignDis,
						xAlignMax+xPillarMax+alignDis, yAlignMax+yPillarMax+alignDis, zAlignMax+zPillarMax+alignDis);
				}

				if ( Standard_True == alignODLBox.IsOut(inODLPnt) )
				{
					continue;
				}

				//当前被停靠物体的box shape
				Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
				movingBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);
				auto alignBoxShape = BRepPrimAPI_MakeBox(gp_Pnt(xMin, yMin, zMin), gp_Pnt(xMax, yMax, zMax)).Shape();
				TopExp_Explorer exp(alignBoxShape, TopAbs_SHELL);

				if ( !lockRotation )
				{//找到第一个被停靠物体
					BRepExtrema_DistShapeShape dss(BRepBuilderAPI_MakeVertex(inODLPnt).Shape(), exp.Current());
					assert(0 != dss.NbSolution());
					inODLPnt = dss.PointOnShape2(1);

					newPnt = inODLPnt.Transformed(curODL->GetAbsoluteTransform()).XYZ();

					auto rot = curODL->GetAbsoluteTransform().GetRotation();
					Standard_Real rX,rY,rZ;
					rot.GetEulerAngles(gp_Extrinsic_XYZ, rX, rY, rZ);
					vector3df rotation(static_cast<float>(irr::core::radToDeg(rX)), static_cast<float>(irr::core::radToDeg(rY)), static_cast<float>(irr::core::radToDeg(rZ)));

					activePilar->GetDataSceneNode()->setRotation(rotation);
					activePilar->SetRotation(rot);

					gp_Dir movingDir;
					if ( std::abs(std::abs(inODLPnt.X()) - xMax) < Precision::Confusion() )
					{
						movingDir = gp::DZ();
					}
					else
					{
						movingDir = gp::DX();
					}
					auto curMovingEdge = BRepBuilderAPI_MakeEdge(gp_Lin(inODLPnt, movingDir)).Edge();

					movingEdge = TopoDS::Edge(curMovingEdge.Moved(curODL->GetAbsoluteTransform()));

					activePilar->AddAlign(curODL);
					imp_.Valid_ = true;
					lockRotation = true;
				}
				else
				{//第二个被停靠物体
					BRepExtrema_DistShapeShape dss(movingEdge.Moved(curODL->GetAbsoluteTransform().Inverted()), exp.Current());
					
					auto foundSecond = false;
					std::map<double, gp_Pnt> tmp;
					for ( auto index=1; index<=dss.NbSolution(); ++index )
					{
						auto pntOnEdge = dss.PointOnShape1(index);
						auto pntOnBox = dss.PointOnShape2(index);

						if ( pntOnEdge.Distance(pntOnBox) > Precision::Confusion() )
						{
							continue;
						}

						tmp.emplace(pntOnEdge.Distance(inODLPnt), pntOnEdge);
						foundSecond = true;
					}

					if ( !foundSecond )
					{
						imp_.Valid_ = false;
						break;
					}

					newPnt = tmp.begin()->second.Transformed(curODL->GetAbsoluteTransform());
					activePilar->AddAlign(curODL);
					lockPosition = true;
				}
			}

			if ( activePilar->GetAlignList().empty() )
			{
				imp_.Valid_ = true;
			}

			newPos.X = static_cast<float>(newPnt.X());
			newPos.Y = static_cast<float>(newPnt.Y());
			newPos.Z = static_cast<float>(newPnt.Z());
			activePilar->GetDataSceneNode()->setPosition(newPos);
			activePilar->SetTranslation(newPnt.XYZ());

			for ( auto& curAlign : activePilar->GetAlignList() )
			{
				curAlign->SetSweeping(true);
			}
		}
		break;
	default:
		break;
	}

	imp_.LMousePressDown_ = false;
	imp_.LMouseLeftUp_ = false;
	imp_.EscapePressDown_ = false;

	return false;
}

void RoomLayoutPillarController::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case EPilarState::EPS_SWEEPING:
		{
			if ( !GetPickingODL().expired() )
			{
				auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
				activePilar->SetSweeping(false);
			}
		}
		break;
	case EPilarState::EPS_MOVING:
		{
			auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
			activePilar->SetPicking(false);

			for ( auto& curAlign : activePilar->GetAlignList() )
			{
				curAlign->SetSweeping(false);
			}
		}
		break;
	case EPilarState::EPS_MODIFY:
		{
			auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
			activePilar->SetPicking(false);
		}
		break;
	default:
		{
			auto activePilar = std::static_pointer_cast<PillarODL>(GetPickingODL().lock());
			activePilar->SetPicking(false);
		}
		break;
	}
}

bool RoomLayoutPillarController::CheckValid()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case EPilarState::EPS_SWEEPING:
		{
			return false;
		}
		break;
	default:
		return true;
		break;
	}

	return false;
}
