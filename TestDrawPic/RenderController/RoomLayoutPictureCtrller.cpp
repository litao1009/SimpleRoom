#include "stdafx.h"

#include "RoomLayoutPictureCtrller.h"
#include "UserEvent.h"

#include "ODL/BaseODL.h"

#include "StatusMgr.h"

#include "irrEngine/SRenderContext.h"
#include "irrEngine/irrEngine.h"
#include "IMeshSceneNode.h"
#include "SMeshBuffer.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "ICameraSceneNode.h"

#include <boost/filesystem/path.hpp>

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;

class	RoomPictureNode : public irr::scene::IMeshSceneNode
{
public:

	RoomPictureNode(irr::scene::ISceneNode* parent, irr::s32 id = -1):IMeshSceneNode(parent, parent->getSceneManager(), id)
	{
		MeshBuffer_ = new irr::scene::SMeshBuffer;

		MeshBuffer_->Vertices.reallocate(4);
		MeshBuffer_->Vertices.push_back(S3DVertex(vector3df(-0.5f, 0, 0.5f), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
		MeshBuffer_->Vertices.push_back(S3DVertex(vector3df(0.5f, 0, 0.5f), vector3df(0,1,0), SColor(~0), vector2df(1,0)));
		MeshBuffer_->Vertices.push_back(S3DVertex(vector3df(0.5f, 0, -0.5f), vector3df(0,1,0), SColor(~0), vector2df(1,-1)));
		MeshBuffer_->Vertices.push_back(S3DVertex(vector3df(-0.5f, 0, -0.5f), vector3df(0,1,0), SColor(~0), vector2df(0,-1)));

		MeshBuffer_->Indices.reallocate(4);
		MeshBuffer_->Indices.push_back(0);
		MeshBuffer_->Indices.push_back(1);
		MeshBuffer_->Indices.push_back(2);
		MeshBuffer_->Indices.push_back(3);

		auto& mat = MeshBuffer_->getMaterial();
		mat.Lighting = false;
		mat.BackfaceCulling = false;
		mat.MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_VERTEX_ALPHA);
		mat.MaterialTypeParam = 0.5f;
	}

	~RoomPictureNode()
	{
		if ( MeshBuffer_ )
		{
			MeshBuffer_->drop();
		}
	}

public://ISceneNode

	virtual void OnRegisterSceneNode()
	{
		if ( isVisible() )
		{
			SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_TRANSPARENT);
		}

		ISceneNode::OnRegisterSceneNode();
	}

	virtual void render()
	{
		auto driver = SceneManager->getVideoDriver();

		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

		driver->setMaterial(MeshBuffer_->getMaterial());
		driver->drawMeshBuffer(MeshBuffer_);
		driver->drawVertexPrimitiveList(MeshBuffer_->getVertices(), MeshBuffer_->getVertexCount(), MeshBuffer_->getIndices(), MeshBuffer_->getIndexCount()/2, EVT_STANDARD, irr::scene::EPT_TRIANGLE_FAN);
	}

	virtual irr::video::SMaterial& getMaterial(irr::u32 i) { return MeshBuffer_->getMaterial(); }

	virtual irr::u32 getMaterialCount() const { return 1; }

	virtual const irr::core::aabbox3df& getBoundingBox() const { return MeshBuffer_->getBoundingBox(); }

public://IMeshSceneNode

	virtual void setMesh(irr::scene::IMesh* mesh) {}

	virtual irr::scene::IMesh* getMesh() { return nullptr; }

	virtual irr::scene::IShadowVolumeSceneNode* addShadowVolumeSceneNode(const irr::scene::IMesh*, irr::s32, bool, irr::f32) { return nullptr; }

	virtual void setReadOnlyMaterials(bool readonly) {}

	virtual bool isReadOnlyMaterials() const { return false; }

private:

	irr::scene::SMeshBuffer*	MeshBuffer_;		
};

enum RoomPicState
{
	RPS_INIT_PICTURE,
	RPS_DRAWING_FIRST_POINT,
	RPS_DRAWING_SECOND_POINT,
	RPS_SET_REFERENCE_SIZE,
	RPS_SET_POSITION,
	RPS_COUNT
};

class	RoomLayoutPictureCtrller::Imp
{
public:

	Imp()
	{
		Picture_ = nullptr;

		State_ = RPS_COUNT;

		LMousePressDown_ = false;
		EscPressDown_ = false;
		Drag_ = false;
		LMouseLeftUp_ = false;

		{
			Point_ = new irr::scene::SMeshBuffer;

			Point_->Vertices.reallocate(4);
			Point_->Vertices.push_back(S3DVertex(vector3df(-0.5f, 0, 0.5f), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
			Point_->Vertices.push_back(S3DVertex(vector3df(0.5f, 0, 0.5f), vector3df(0,1,0), SColor(~0), vector2df(1,0)));
			Point_->Vertices.push_back(S3DVertex(vector3df(0.5f, 0, -0.5f), vector3df(0,1,0), SColor(~0), vector2df(1,-1)));
			Point_->Vertices.push_back(S3DVertex(vector3df(-0.5f, 0, -0.5f), vector3df(0,1,0), SColor(~0), vector2df(0,-1)));

			Point_->Indices.reallocate(4);
			Point_->Indices.push_back(0);
			Point_->Indices.push_back(1);
			Point_->Indices.push_back(2);
			Point_->Indices.push_back(3);

			auto& mat = Point_->getMaterial();
			mat.Lighting = false;
			mat.BackfaceCulling = false;
			mat.MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_VERTEX_ALPHA);
			mat.MaterialTypeParam = 0.5f;
			mat.setTexture(0, IrrEngine::GetInstance()->GetDevice()->getVideoDriver()->getTexture("../Data/Resource/3D/dot.png"));
		}

		{
			Line_ = new irr::scene::SMeshBuffer;

			Line_->Vertices.reallocate(2);
			Line_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
			Line_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));

			Line_->Indices.reallocate(2);
			Line_->Indices.push_back(0);
			Line_->Indices.push_back(1);

			auto& mat = Line_->getMaterial();
			mat.Lighting = false;
			mat.BackfaceCulling = false;
			mat.MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
			mat.DiffuseColor = SColor(0xFF0000FF);
			mat.Thickness = 4;
		}

		Scale_.setScale(vector3df(200.f, 1, 200.f));

		RefSize_ = 0;
	}

	~Imp()
	{
		if ( Picture_ )
		{
			Picture_->drop();
		}

		if ( Point_ )
		{
			Point_->drop();
		}

		if ( Line_ )
		{
			Line_->drop();
		}
	}

	RoomPictureNode*		Picture_;
	RoomPicState			State_;
	boost::filesystem::path	PicPath_;
	SMeshBuffer*			Point_;
	SMeshBuffer*			Line_;
	matrix4					Scale_;
	vector3df				FirstPos_;
	vector3df				SecondPos_;
	vector2di				CursorIPos_;
	vector3df				CurrentPos_;
	vector3df				SavePos_;
	unsigned				RefSize_;
	bool					LMousePressDown_;
	bool					LMouseLeftUp_;
	bool					Drag_;
	bool					EscPressDown_;
	
};

RoomLayoutPictureCtrller::RoomLayoutPictureCtrller():ImpUPtr_(new Imp)
{

}

RoomLayoutPictureCtrller::~RoomLayoutPictureCtrller()
{

}

void RoomLayoutPictureCtrller::Init()
{
	ImpUPtr_->Picture_ = new RoomPictureNode(BaseODL_.lock()->GetDataSceneNode().get());
	ImpUPtr_->Picture_->setVisible(false);
}

bool RoomLayoutPictureCtrller::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	auto driver = GetRenderContextSPtr()->Smgr_->getVideoDriver();

	plane3df plan(0,0,0,0,1,0);
	vector3df planPos;
	auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(imp_.CursorIPos_);
	plan.getIntersectionWithLine(line.start, line.getVector(), planPos);
	planPos.Y = 0;
	imp_.CurrentPos_ = planPos;

	switch (imp_.State_)
	{
	case RPS_INIT_PICTURE:
		{
			
			auto tex = driver->getTexture(imp_.PicPath_.c_str());
			assert(tex);

			imp_.Picture_->getMaterial(0).setTexture(0, tex);

			auto texSize = tex->getSize();

			auto camera = GetRenderContextSPtr()->Smgr_->getActiveCamera();
			
			auto leftUp = camera->getViewFrustum()->getNearLeftUp();
			auto rightUp = camera->getViewFrustum()->getNearRightUp();
			auto leftDown = camera->getViewFrustum()->getNearLeftDown();
			auto rightDown = camera->getViewFrustum()->getNearRightDown();
			
			auto screenWidth = leftUp.getDistanceFrom(rightUp);
			auto screenHeight = rightUp.getDistanceFrom(rightDown);

			auto widthScale = screenWidth / texSize.Width;
			auto heightScale = screenHeight / texSize.Height;

			auto scale = widthScale < heightScale ? widthScale : heightScale;

			imp_.Picture_->setScale(vector3df(texSize.Width * scale, 1, texSize.Height * scale));
			
			imp_.State_ = RPS_DRAWING_FIRST_POINT;
		}
	case RPS_DRAWING_FIRST_POINT:
		{
			if ( imp_.LMousePressDown_ )
			{
				imp_.FirstPos_ = imp_.CurrentPos_;
				imp_.State_ = RPS_DRAWING_SECOND_POINT;

				imp_.Line_->getPosition(0) = imp_.FirstPos_;
				imp_.Line_->getPosition(1) = imp_.FirstPos_;
			}
		}
		break;
	case RPS_DRAWING_SECOND_POINT:
		{
			line3df curLine(imp_.FirstPos_, imp_.CurrentPos_);

			auto curDir = imp_.CurrentPos_ - imp_.FirstPos_;

			imp_.Line_->getPosition(1) = imp_.CurrentPos_;

			if ( imp_.LMousePressDown_ )
			{
				imp_.LMousePressDown_ = false;
				imp_.SecondPos_ = imp_.CurrentPos_;
				::PostMessage((HWND)(GetRenderContextSPtr()->GetHandle()), WM_IRR_DLG_MSG, WM_USER_ROOMLAYOUT_PICTUREREFLINE_SIZE, 0);
				imp_.State_ = RPS_COUNT;
			}

			if ( imp_.EscPressDown_ )
			{
				imp_.State_ = RPS_DRAWING_FIRST_POINT;
			}
		}
		break;
	case RPS_SET_REFERENCE_SIZE:
		{
			auto newScaleFactor = imp_.RefSize_ / imp_.SecondPos_.getDistanceFrom(imp_.FirstPos_);
			
			auto curScale = imp_.Picture_->getScale();
			curScale.X *= newScaleFactor;
			curScale.Z *= newScaleFactor;

			imp_.Picture_->setScale(curScale);

			irr::SEvent evt;
			evt.EventType = EET_USER_EVENT;
			evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PICTURE_FINISH;
			GetRenderContextSPtr()->PostEvent(evt);
			imp_.State_ = RPS_COUNT;
		}
		break;
	case RPS_SET_POSITION:
		{
			if ( !imp_.Drag_ )
			{
				imp_.Drag_ = imp_.LMousePressDown_;

				if ( imp_.Drag_ )
				{
					imp_.SavePos_ = imp_.CurrentPos_;
					break;
				}
			}
			else
			{
				imp_.Drag_ = !imp_.LMouseLeftUp_;

				if ( !imp_.Drag_ )
				{
					imp_.State_ = RPS_COUNT;
					break;
				}
			}

			if ( imp_.Drag_ )
			{
				auto curPos = imp_.Picture_->getPosition();
				auto curVec = imp_.CurrentPos_ - imp_.SavePos_;
				imp_.Picture_->setPosition(curVec + curPos);
				imp_.SavePos_ = imp_.CurrentPos_;
			}
		}
		break;
	default:
		break;
	}

	imp_.LMousePressDown_ = false;
	imp_.LMouseLeftUp_ = false;
	imp_.EscPressDown_ = false;

	return false;
}

void RoomLayoutPictureCtrller::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	auto driver = GetRenderContextSPtr()->Smgr_->getVideoDriver();

	matrix4 trans;
	trans.setTranslation(imp_.CurrentPos_);

	switch (imp_.State_)
	{
	case RPS_DRAWING_FIRST_POINT:
		{
			driver->setTransform(ETS_WORLD, trans*imp_.Scale_);
			driver->setMaterial(imp_.Point_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.Point_->getVertices(), imp_.Point_->getVertexCount(), imp_.Point_->getIndices(), imp_.Point_->getIndexCount()/2, EVT_STANDARD, EPT_TRIANGLE_FAN);
		}
		break;
	case RPS_DRAWING_SECOND_POINT:
		{
			driver->setTransform(ETS_WORLD, matrix4());
			driver->setMaterial(imp_.Line_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.Line_->getVertices(), imp_.Line_->getVertexCount(), imp_.Line_->getIndices(), imp_.Line_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);

			driver->setTransform(ETS_WORLD, trans*imp_.Scale_);
			driver->setMaterial(imp_.Point_->getMaterial());
			driver->drawVertexPrimitiveList(imp_.Point_->getVertices(), imp_.Point_->getVertexCount(), imp_.Point_->getIndices(), imp_.Point_->getIndexCount()/2, EVT_STANDARD, EPT_TRIANGLE_FAN);

			matrix4 firstTrans;
			firstTrans.setTranslation(imp_.FirstPos_);
			driver->setTransform(ETS_WORLD, firstTrans*imp_.Scale_);
			driver->drawVertexPrimitiveList(imp_.Point_->getVertices(), imp_.Point_->getVertexCount(), imp_.Point_->getIndices(), imp_.Point_->getIndexCount()/2, EVT_STANDARD, EPT_TRIANGLE_FAN);
		}
		break;
	case RPS_SET_POSITION:
		{

		}
		break;
	default:
		break;
	}
}

bool RoomLayoutPictureCtrller::OnPostEvent( const irr::SEvent& event )
{
	auto& imp_ = *ImpUPtr_;

	if ( event.EventType == EET_USER_EVENT && event.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTURE_SETTING )
	{
		imp_.State_ = RPS_INIT_PICTURE;
		auto filePath = static_cast<boost::filesystem::path*>(reinterpret_cast<void*>(event.UserEvent.UserData2));
		imp_.PicPath_ = *filePath;
		imp_.Picture_->setVisible(true);

		return true;
	}

	if ( event.EventType == EET_USER_EVENT )
	{
		if ( event.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTUREREFLINE_SIZE )
		{
			imp_.RefSize_ = event.UserEvent.UserData2;
			imp_.State_ = RPS_SET_REFERENCE_SIZE;

			return true;
		}
		
		if ( event.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTURE_VISIBLE )
		{
			imp_.Picture_->setVisible(event.UserEvent.UserData2 == 1);

			return true;
		}

		if ( event.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTURE_ALPHA )
		{
			imp_.Picture_->getMaterial(0).MaterialTypeParam = event.UserEvent.UserData2 / 100.f;

			return true;
		}

		if ( event.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTUREREFLINE_DRAW )
		{
			if ( imp_.Picture_ )
			{
				imp_.State_ = RPS_DRAWING_FIRST_POINT;
			}

			return true;
		}

		if ( event.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTURE_POSITION )
		{
			if ( imp_.Picture_ )
			{
				imp_.State_ = RPS_SET_POSITION;
			}

			return true;
		}
	}

	if ( event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
	{
		imp_.LMousePressDown_ = true;
	}

	if ( event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
	{
		imp_.LMouseLeftUp_ = true;
	}

	if ( event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_MOUSE_MOVED )
	{
		imp_.CursorIPos_.X = event.MouseInput.X;
		imp_.CursorIPos_.Y = event.MouseInput.Y;
	}

	if ( event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_ESCAPE && event.KeyInput.PressedDown )
	{
		if ( imp_.State_ == RPS_DRAWING_SECOND_POINT )
		{
			imp_.EscPressDown_ = true;
		}
	}

	return false;
}