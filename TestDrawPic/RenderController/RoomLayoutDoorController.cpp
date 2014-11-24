#include "stdafx.h"

#include "RoomLayoutDoorController.h"
#include "RenderController/UserEvent.h"
#include "irrEngine/SRenderContext.h"
#include "irrEngine/irrEngine.h"
#include "irrEngine/IrrExtension/FreetypeFontManager.h"
#include "SMeshBuffer.h"

#include "ODL/DoorODL.h"
#include "ODL/WallODL.h"
#include "ODL/GraphODL.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "gp_Lin.hxx"

#include <boost/optional.hpp>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

enum class EDoorState
{
	EDS_SWEEPING,
	EDS_CREATING_INIT,
	EDS_MOUSEHOLDING,
	EDS_MOVING,
	EDS_PROPERTY,
	EDS_COUNT
};

class	RoomLayoutDoorController::Imp
{
public:

	Imp()
	{
		State_ = EDoorState::EDS_SWEEPING;
		Checker_ = false;
		EscapePressDown_ = false;
		LMouseLeftUp_ = false;
		LMousePressDown_ = false;

		{
			LineBuf_ = new SMeshBuffer;
			LineBuf_->Vertices.reallocate(4);
			LineBuf_->Indices.reallocate(4);
			LineBuf_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0)));
			LineBuf_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0)));
			LineBuf_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0)));
			LineBuf_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0)));
			LineBuf_->Indices.push_back(0);
			LineBuf_->Indices.push_back(1);
			LineBuf_->Indices.push_back(2);
			LineBuf_->Indices.push_back(3);

			LineBuf_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
			LineBuf_->getMaterial().DiffuseColor = SColor(0xFF0000FF);
			LineBuf_->getMaterial().Thickness = 2;
		}

		{
			Num1Buf_ = ODLTools::NEW_CreateRectMeshBuffer(.5f);
			Num1Buf_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_FONT);
			Num1Buf_->getMaterial().DiffuseColor = SColor(0xFF000000);
		}

		{
			Num2Buf_ = ODLTools::NEW_CreateRectMeshBuffer(.5f);
			Num2Buf_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_FONT);
			Num2Buf_->getMaterial().DiffuseColor = SColor(0xFF000000);
		}
	}

	~Imp()
	{
		if ( LineBuf_ )
		{
			LineBuf_->drop();
		}

		if ( Num1Buf_ )
		{
			Num1Buf_->drop();
		}

		if ( Num2Buf_ )
		{
			Num2Buf_->drop();
		}
	}

	void	UpdateLable(const DoorODLSPtr& activeDoor, IVideoDriver* driver)
	{
		auto wallLength = PickingWall_.lock()->GetLength();
		auto trans = activeDoor->GetTranslation();
		auto size = activeDoor->GetHoleSize();

		auto lin1Length = wallLength/2 + trans.X() - size.X()/2;
		auto lin2Length = wallLength - lin1Length - size.X();

		LineBuf_->Vertices[0].Pos = vector3df(static_cast<float>(-wallLength/2), 0, 0);
		LineBuf_->Vertices[1].Pos = vector3df(static_cast<float>(lin1Length - wallLength/2), 0, 0);
		LineBuf_->Vertices[2].Pos = vector3df(static_cast<float>(lin1Length + size.X() - wallLength/2), 0, 0);
		LineBuf_->Vertices[3].Pos = vector3df(static_cast<float>(wallLength - wallLength/2), 0, 0);

		auto font = FreetypeFontMgr::GetInstance().GetTtFont(driver, "arial.ttf", 32);
		assert(font);

		{
			auto tex = font->GenerateTextTexture(std::to_wstring(static_cast<int>(lin1Length+0.5)).c_str());
			auto texSize = tex->getSize();

			auto factor = 100.f / texSize.Height;
			Num1Scale_.setScale(vector3df(texSize.Width*factor, 1, texSize.Height*factor));
			Num1Pos_.setTranslation(vector3df(static_cast<float>(lin1Length/2 - wallLength/2), 0, -50));
			Num1Buf_->getMaterial().setTexture(0, tex);
		}

		{
			auto tex = font->GenerateTextTexture(std::to_wstring(static_cast<int>(lin2Length+0.5)).c_str());
			auto texSize = tex->getSize();

			auto factor = 100.f / texSize.Height;
			Num2Scale_.setScale(vector3df(texSize.Width*factor, 1, texSize.Height*factor));
			Num2Pos_.setTranslation(vector3df(static_cast<float>(lin1Length + size.X() + lin2Length/2 - wallLength/2), 0, -50));
			Num2Buf_->getMaterial().setTexture(0, tex);
		}
	}

public:

	bool			EscapePressDown_;
	bool			LMouseLeftUp_;
	bool			LMousePressDown_;
	bool			Checker_;
	EDoorState		State_;
	WallODLWPtr		PickingWall_;
	vector2di		CursorIPos_;
	vector3df		CurrentPos_;
	vector3df		SavePos_;

	boost::optional<EUserType>		PropertyCallBack_;
	boost::optional<SEventDoorInfo>	EventInfo_;

	SMeshBuffer*	LineBuf_;
	IMeshBuffer*	Num1Buf_;
	matrix4			Num1Pos_;
	matrix4			Num1Scale_;
	IMeshBuffer*	Num2Buf_;
	matrix4			Num2Pos_;
	matrix4			Num2Scale_;
};

RoomLayoutDoorController::RoomLayoutDoorController(const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc):IRoomLayoutODLBaseCtrller(rc),ImpUPtr_(new Imp)
{
	RootODL_ = graphODL;
}

RoomLayoutDoorController::~RoomLayoutDoorController()
{

}

void RoomLayoutDoorController::Init()
{

}

bool RoomLayoutDoorController::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	if ( evt.EventType == EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_DOOR_PROPERTY )
	{
		imp_.PropertyCallBack_ = static_cast<EUserType>(evt.UserEvent.UserData2);
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
	{
		imp_.CursorIPos_.X = evt.MouseInput.X;
		imp_.CursorIPos_.Y = evt.MouseInput.Y;
	}

	if ( evt.EventType == irr::EET_USER_EVENT && evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_TEST_DOOR )
	{
		auto doorInfo = static_cast<SEventDoorInfo*>(reinterpret_cast<void*>(evt.UserEvent.UserData2));
		imp_.EventInfo_ = *doorInfo;
		imp_.State_ = EDoorState::EDS_CREATING_INIT;
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

	return false;
}


bool RoomLayoutDoorController::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	plane3df plan(0,0,0,0,1,0);
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), imp_.CurrentPos_);
	imp_.CurrentPos_.Y = 0;
	gp_Pnt cursorPnt(imp_.CurrentPos_.X, imp_.CurrentPos_.Y, imp_.CurrentPos_.Z);

	switch (imp_.State_)
	{
	case EDoorState::EDS_SWEEPING:
		{
			auto activeDoor = std::static_pointer_cast<DoorODL>(GetPickingODL().lock());
			activeDoor->SetSweeping(true);

			if ( imp_.LMousePressDown_ )
			{
				auto size = activeDoor->GetHoleSize();
				imp_.EventInfo_ = SEventDoorInfo();
				imp_.EventInfo_->XLength_ = static_cast<float>(size.X());
				imp_.EventInfo_->YLength_ = static_cast<float>(size.Y());
				imp_.EventInfo_->ZLength_ = static_cast<float>(size.Z());

				imp_.SavePos_ = imp_.CurrentPos_;
				imp_.State_ = EDoorState::EDS_MOUSEHOLDING;
			}
		}
		break;
	case EDoorState::EDS_MOUSEHOLDING:
		{
			auto activeDoor = std::static_pointer_cast<DoorODL>(GetPickingODL().lock());
			activeDoor->SetPicking(true);

			if ( imp_.SavePos_.getDistanceFromSQ(imp_.CurrentPos_) > 100 * 100 )
			{
				imp_.State_ = EDoorState::EDS_MOVING;
				imp_.PickingWall_ = std::static_pointer_cast<WallODL>(activeDoor->GetParent().lock());
				imp_.PickingWall_.lock()->SeamHole(activeDoor);
				imp_.Checker_ = true;
			}
			else 
				if ( imp_.LMouseLeftUp_ )
				{
					imp_.PropertyCallBack_ = boost::none;
					imp_.State_ = EDoorState::EDS_PROPERTY;
					
					auto pointer = reinterpret_cast<int>(static_cast<void*>(&(*imp_.EventInfo_)));
					::PostMessage((HWND)GetRenderContextSPtr()->GetHandle(), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_DOOR_PROPERTY, pointer);
				}
		}
		break;
	case EDoorState::EDS_CREATING_INIT:
		{
			assert(imp_.EventInfo_);
			
			auto newDoor = DoorODL::Create<DoorODL>(GetRenderContextSPtr());
			newDoor->SetHoleSize(imp_.EventInfo_->XLength_, imp_.EventInfo_->YLength_, imp_.EventInfo_->ZLength_);
			newDoor->SetOffsetSize(0, 0, 50);
			newDoor->UpdateHole();

			RootODL_.lock()->AddChild(newDoor);

			auto newPos = imp_.CurrentPos_;
			newPos.Y = imp_.EventInfo_->YLength_/2;

			newDoor->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
			newDoor->GetDataSceneNode()->setPosition(newPos);
			SetPickingODL(newDoor);

			imp_.State_ = EDoorState::EDS_MOVING;
		}
		break;
	case EDoorState::EDS_MOVING:
		{
			auto activeDoor = std::static_pointer_cast<DoorODL>(GetPickingODL().lock());
			activeDoor->SetPicking(true);

			if ( imp_.LMouseLeftUp_ )
			{
				if ( imp_.Checker_ )
				{
					ImpUPtr_->PickingWall_.lock()->CutHole(activeDoor);

					ImpUPtr_->Checker_ = false;
					ImpUPtr_->EventInfo_ = boost::none;

					imp_.State_ = EDoorState::EDS_SWEEPING;
					break;
				}
			}

			if ( imp_.EscapePressDown_ )
			{
				activeDoor->RemoveFromParent();

				imp_.State_ = EDoorState::EDS_SWEEPING;
				break;
			}

			auto curZone = activeDoor->GetHoleSize();
			
			gp_Lin cursorLin(cursorPnt, gp::DY().Reversed());

			auto spPickingNode = imp_.PickingWall_.lock();
			if ( spPickingNode )
			{
				if ( !spPickingNode->GetBaseBndBox().IsOut(cursorLin.Transformed(spPickingNode->GetAbsoluteTransform().Inverted())) )
				{
					const auto& box = spPickingNode->GetBaseBndBox();
					auto transform = spPickingNode->GetAbsoluteTransform().Inverted();
					gp_Pnt curPnt = cursorPnt;
					curPnt.Transform(transform);

					Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
					box.Get(xMin,yMin,zMin,xMax,yMax,zMax);

					activeDoor->SetTranslation(gp_XYZ(curPnt.X(), (curZone.Y()-(yMax-yMin))/2, 0));
					activeDoor->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(curPnt.X()), (static_cast<float>(curZone.Y()-(yMax-yMin)))/2, 0));
					
					imp_.UpdateLable(activeDoor, GetRenderContextSPtr()->Smgr_->getVideoDriver());

					break;
				}
				else
				{
					RootODL_.lock()->AddChild(activeDoor);
					imp_.PickingWall_.reset();
				}
			}

			for ( auto& wallODL : RootODL_.lock()->GetChildrenList() )
			{
				if ( wallODL->GetType() != EODLT_WALL )
				{
					continue;
				}

				if ( wallODL->GetBaseBndBox().IsOut(cursorLin.Transformed(wallODL->GetAbsoluteTransform().Inverted())) )
				{
					continue;
				}

				const auto& box = wallODL->GetBaseBndBox();
				Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
				box.Get(xMin,yMin,zMin,xMax,yMax,zMax);

				activeDoor->SetHoleSize(curZone.X(), curZone.Y(), static_cast<float>(std::abs(zMax-zMin)));
				activeDoor->UpdateHole();
				auto transform = wallODL->GetAbsoluteTransform().Inverted();
				gp_Pnt curPnt = cursorPnt;
				curPnt.Transform(transform);

				imp_.PickingWall_ = std::static_pointer_cast<WallODL>(wallODL);
				wallODL->AddChild(activeDoor);
				activeDoor->SetTranslation(gp_XYZ(curPnt.X(), curZone.Y()/2, 0));
				activeDoor->GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(curPnt.X()), static_cast<float>(curPnt.Y()/2), 0));

				activeDoor->Set2DLineColor(irr::video::SColor(0xFF8F8F8F));
				imp_.Checker_ = true;

				break;
			}

			if ( imp_.PickingWall_.expired() )
			{
				auto newPos = imp_.CurrentPos_;
				newPos.Y = imp_.EventInfo_->YLength_/2;
				activeDoor->SetTranslation(gp_XYZ(newPos.X, newPos.Y, newPos.Z));
				activeDoor->GetDataSceneNode()->setPosition(newPos);
				activeDoor->Set2DLineColor(irr::video::SColor(0xFFFF0000));
				imp_.Checker_ = false;
			}

			if ( imp_.Checker_ )
			{
				imp_.UpdateLable(activeDoor, GetRenderContextSPtr()->Smgr_->getVideoDriver());
			}
		}
		break;
	case EDoorState::EDS_PROPERTY:
		{
			if ( !imp_.PropertyCallBack_ )
			{
				break;
			}

			auto activeDoor = std::static_pointer_cast<DoorODL>(GetPickingODL().lock());

			switch (*(imp_.PropertyCallBack_))
			{
			case EUT_ROOMLAYOUT_DOOR_NONE:
				{
					imp_.State_ = EDoorState::EDS_SWEEPING;
				}
				break;
			case EUT_ROOMLAYOUT_DOOR_UPDATE:
				{
// 					activeDoor->SetHeight(imp_.EventInfo_.Height_);
// 					activeDoor->SetThickness(imp_.EventInfo_.Thickness_);
					imp_.State_ = EDoorState::EDS_SWEEPING;
				}
				break;
			case EUT_ROOMLAYOUT_DOOR_MOVE:
				{
					imp_.State_ = EDoorState::EDS_MOVING;
					imp_.PickingWall_ = std::static_pointer_cast<WallODL>(activeDoor->GetParent().lock());
					imp_.PickingWall_.lock()->SeamHole(activeDoor);
					imp_.Checker_ = true;
				}
				break;
			case EUT_ROOMLAYOUT_DOOR_DELETE:
				{
					activeDoor->RemoveFromParent();
					SetPickingODL(BaseODLSPtr());
					imp_.State_ = EDoorState::EDS_SWEEPING;
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

	imp_.LMousePressDown_ = false;
	imp_.LMouseLeftUp_ = false;
	imp_.LMousePressDown_ = false;

	return false;
}

void RoomLayoutDoorController::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case EDoorState::EDS_SWEEPING:
		{
			if ( !GetPickingODL().expired() )
			{
				auto activeDoor = std::static_pointer_cast<DoorODL>(GetPickingODL().lock());
				activeDoor->SetSweeping(false);
			}
		}
		break;
	case EDoorState::EDS_MOVING:
		{
			if ( !imp_.Checker_ )
			{
				break;
			}

			auto pntMat = imp_.PickingWall_.lock()->GetDataSceneNode()->getAbsoluteTransformation();

			auto driver = GetRenderContextSPtr()->Smgr_->getVideoDriver();
			
			{
				driver->setTransform(ETS_WORLD, pntMat);
				driver->setMaterial(imp_.LineBuf_->getMaterial());
				driver->drawVertexPrimitiveList(imp_.LineBuf_->getVertices(), imp_.LineBuf_->getVertexCount(), imp_.LineBuf_->getIndices(), imp_.LineBuf_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
			}

			{
				driver->setTransform(ETS_WORLD, pntMat * imp_.Num1Pos_ * imp_.Num1Scale_);
				driver->setMaterial(imp_.Num1Buf_->getMaterial());
				driver->drawMeshBuffer(imp_.Num1Buf_);
			}

			{
				driver->setTransform(ETS_WORLD, pntMat * imp_.Num2Pos_ * imp_.Num2Scale_);
				driver->setMaterial(imp_.Num2Buf_->getMaterial());
				driver->drawMeshBuffer(imp_.Num2Buf_);
			}
		}
		break;
	default:
		{
			auto activeDoor = std::static_pointer_cast<DoorODL>(GetPickingODL().lock());
			activeDoor->SetPicking(false);
		}
		break;
	}
}

bool RoomLayoutDoorController::CheckValid()
{
	auto& imp_ = *ImpUPtr_;

	switch (imp_.State_)
	{
	case EDoorState::EDS_SWEEPING:
		{
			return false;
		}
		break;
	case EDoorState::EDS_CREATING_INIT:
		{
			return true;
		}
		break;
	case EDoorState::EDS_MOVING:
		{
			return true;
		}
		break;
	default:
		return true;
		break;
	}

	return false;
}
