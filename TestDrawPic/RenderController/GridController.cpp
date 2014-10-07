#include "stdafx.h"

#include "GridController.h"

#include "irrEngine/SRenderContext.h"
#include "irrEngine/irrEngine.h"

#include "ISceneManager.h"
#include "SMeshBuffer.h"
#include "SMesh.h"
#include "IMeshSceneNode.h"

#include "StatusMgr.h"

class GridMeshSceneNode : public irr::scene::ISceneNode
{
	irr::scene::IMesh*		Mesh_;
	irr::core::vector3df	CameraCenter_;
	int						GridCount_;

public:

	GridMeshSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id) : irr::scene::ISceneNode(parent, mgr, id)
	{
		auto meshbuf = new irr::scene::SMeshBuffer;

		GridCount_ = 40;
		auto lineCount = GridCount_ + 1;
		auto vertexCount = lineCount * 2;

		irr::video::SColor clr(0xFF808080);

		auto offset = -GridCount_/2;

		meshbuf->Vertices.reallocate(vertexCount);
		meshbuf->Indices.reallocate(vertexCount);
		for ( auto index=0; index<vertexCount; ++index )
		{
			auto xIndex = static_cast<float>(index / 2);
			auto zIndex = static_cast<float>(index & 1);

			meshbuf->Vertices.push_back(irr::video::S3DVertex(xIndex+offset, 0, zIndex*GridCount_+offset, 0, 1, 0, clr, 0, 0));

			meshbuf->Indices.push_back(index);
		}

		meshbuf->getMaterial().Lighting = false;
		meshbuf->getMaterial().AntiAliasing = irr::video::EAAM_QUALITY|irr::video::EAAM_LINE_SMOOTH;
		meshbuf->recalculateBoundingBox();

		auto newMesh = new irr::scene::SMesh;
		newMesh->addMeshBuffer(meshbuf);
		newMesh->recalculateBoundingBox();
		meshbuf->drop();
		meshbuf->getMaterial().PolygonOffsetDirection = irr::video::EPO_BACK;
		meshbuf->getMaterial().PolygonOffsetFactor = 7;
		Mesh_ = newMesh;
	}

	~GridMeshSceneNode()
	{
		Mesh_->drop();
	}

public:

	void	SetCameraCenter(const irr::core::vector3df& vec) { CameraCenter_ = vec; }

	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	virtual void render()
	{
		auto meshBuf = Mesh_->getMeshBuffer(0);

		auto driver = SceneManager->getVideoDriver();

		driver->setMaterial(meshBuf->getMaterial());

		auto vec = CameraCenter_;
		vec.X /= getScale().X * GridCount_;
		vec.Z /= getScale().Z * GridCount_;

		auto rowOffsetStep = static_cast<int>(vec.X);
		auto columOffsetStep = static_cast<int>(vec.Z);

		irr::core::matrix4 r,s,t;
		s.setScale(getScale());
		r.setRotationDegrees(getRotation());

		for ( auto rowIndex=rowOffsetStep-1; rowIndex<=rowOffsetStep+1; ++rowIndex )
		{
			for ( auto columnIndex=columOffsetStep-1; columnIndex<=columOffsetStep+1; ++columnIndex )
			{
				irr::core::vector3df offset(getScale().X*GridCount_*rowIndex, 0, getScale().Z*GridCount_*columnIndex);

				t.setTranslation(offset);
				driver->setTransform(irr::video::ETS_WORLD, t*s);
				driver->drawVertexPrimitiveList(meshBuf->getVertices(), meshBuf->getVertexCount(), meshBuf->getIndices(), meshBuf->getIndexCount()/2, irr::video::EVT_STANDARD, irr::scene::EPT_LINES);

				driver->setTransform(irr::video::ETS_WORLD, t*r*s);
				driver->drawVertexPrimitiveList(meshBuf->getVertices(), meshBuf->getVertexCount(), meshBuf->getIndices(), meshBuf->getIndexCount()/2, irr::video::EVT_STANDARD, irr::scene::EPT_LINES);
			}
		}
	}

	const irr::core::aabbox3df& getBoundingBox() const
	{
		return Mesh_->getBoundingBox();
	}

	virtual irr::u32 getMaterialCount() const
	{
		return Mesh_->getMeshBufferCount();
	}

	virtual irr::video::SMaterial& getMaterial(irr::u32 i)
	{
		return Mesh_->getMeshBuffer(i)->getMaterial();
	}	
};

static irr::core::plane3df s_ZeroPlane(0,0,0,0,1,0);

GridController::GridController()
{
	Node_ = nullptr;
	GridSize_ = 200.f;
	Ignore_ = false;
}

GridController::~GridController()
{
	Node_->drop();
}

bool GridController::OnPreEvent( const irr::SEvent& evt )
{
	if ( evt.EventType == irr::EET_KEY_INPUT_EVENT && evt.KeyInput.Key == irr::KEY_SHIFT )
	{
		if ( !evt.KeyInput.PressedDown )
		{
			Ignore_ = !Ignore_;
		}
	}

	if ( evt.EventType == irr::EET_MOUSE_INPUT_EVENT && evt.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
	{
		CursorIPos_.X = evt.MouseInput.X;
		CursorIPos_.Y = evt.MouseInput.Y;

		auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos_);

		if ( IsEnable() && !Ignore_ )
		{
			auto ret = s_ZeroPlane.getIntersectionWithLine(line.start, line.getVector(), GridPos_);

			assert(ret);

			auto factor = std::fmod(GridPos_.X, GridSize_);
			GridPos_.X -= factor;
			if ( std::abs(factor) > GridSize_/2 )
			{
				GridPos_.X += GridPos_.X > 0 ? GridSize_ : -GridSize_;
			}

			factor = std::fmod(GridPos_.Z, GridSize_);
			GridPos_.Z -= factor;
			if ( std::abs(factor) > GridSize_/2 )
			{
				GridPos_.Z += GridPos_.Z > 0 ? GridSize_ : -GridSize_;
			}

			StatusMgr::GetInstance().GridAlign_ = GridPos_;
		}
		else
		{
			StatusMgr::GetInstance().GridAlign_ = boost::none;
		}
	}

	return false;
}

bool GridController::PreRender3D()
{
	Node_->setVisible(IsEnable());
	
	if ( !IsEnable() )
	{
		return false;
	}

	Node_->setVisible(!Ignore_);
	if ( Ignore_ )
	{
		return false;
	}

	auto curSmgr = GetRenderContextSPtr()->Smgr_.get();
	auto curCamera = curSmgr->getActiveCamera();

	const auto& topLeft = curSmgr->getVideoDriver()->getViewPort().UpperLeftCorner;
	const auto& bottomRight = curSmgr->getVideoDriver()->getViewPort().LowerRightCorner;

	auto center = (topLeft + bottomRight)/2;

	const irr::core::line3df line = curSmgr->getSceneCollisionManager()->getRayFromScreenCoordinates(center, curCamera);

	irr::core::vector3df position;
	s_ZeroPlane.getIntersectionWithLine(line.start, line.getVector(), position);
	static_cast<GridMeshSceneNode*>(Node_)->SetCameraCenter(position);
	Node_->setPosition(position);

	Node_->setRotation(irr::core::vector3df(0, 90, 0));
	Node_->setScale(irr::core::vector3df(GridSize_*2, 1, GridSize_*2));

	return false;
}

void GridController::Init()
{
	auto smgr = GetRenderContextSPtr()->Smgr_.get();

	Node_ = new GridMeshSceneNode(smgr->getRootSceneNode(), smgr, -1);
}
