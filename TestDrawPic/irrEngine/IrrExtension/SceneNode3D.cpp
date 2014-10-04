#include "stdafx.h"

#include "SceneNode3D.h"

#include "IMesh.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"

#include "irrEngine/SRenderContext.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

CSceneNode3D::CSceneNode3D(SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr)
	:	CBaseSceneNode(rcWPtr, odlWPtr)
{
	InShadowPass_ = false;
	InDepthPass_ = false;
	Mesh_ = nullptr;
}

CSceneNode3D::~CSceneNode3D()
{
	if (Mesh_)
	{
		Mesh_->drop();
	}

	RemoveFromDepthPass();
	RemoveFromShadowPass();
}

void CSceneNode3D::render()
{
	auto driver = SceneManager->getVideoDriver();

	if (!Mesh_ || !driver)
	{
		return;
	}

	Box_ = Mesh_->getBoundingBox();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	for (irr::u32 i=0; i<Mesh_->getMeshBufferCount(); ++i )
	{
		auto mb = Mesh_->getMeshBuffer(i);

		auto& mat = mb->getMaterial();
		driver->setMaterial(mat);
		driver->drawMeshBuffer(mb);
	}

	// for debug purposes only:
	if (DebugDataVisible)
	{
		video::SMaterial m;
		m.Lighting = false;
		m.AntiAliasing=4;
		m.Thickness = 2;
		driver->setMaterial(m);

		driver->draw3DBox(Box_, GetColor());
	}

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
}

const irr::core::aabbox3df& CSceneNode3D::getBoundingBox() const
{
	return Mesh_ ? Mesh_->getBoundingBox() : Box_;
}

void CSceneNode3D::OnRegisterSceneNode()
{
	if ( !isVisible() )
	{
		return;
	}

	SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

irr::video::SMaterial& CSceneNode3D::getMaterial( u32 i )
{
	if (Mesh_ && i<Mesh_->getMeshBufferCount())
	{
		return Mesh_->getMeshBuffer(i)->getMaterial();
	}

	return ISceneNode::getMaterial(i);
}

irr::u32 CSceneNode3D::getMaterialCount() const
{
	if (Mesh_)
	{
		return Mesh_->getMeshBufferCount();
	}

	return ISceneNode::getMaterialCount();
}

void CSceneNode3D::setMesh( irr::scene::IMesh* mesh )
{
	if (!mesh)
	{
		return;
	}

	mesh->grab();
	if (Mesh_)
	{
		Mesh_->drop();
	}

	Mesh_ = mesh;
	SaveMaterialType_.clear();
}

irr::scene::ISceneNode* CSceneNode3D::clone( irr::scene::ISceneNode* newParent, irr::scene::ISceneManager* newManager )
{
	return nullptr;
}

irr::scene::IMesh* CSceneNode3D::getMesh()
{
	return Mesh_;
}

void CSceneNode3D::AddToShadowPass()
{
	InShadowPass_ = true;

	if ( isVisible() )
	{
		GetRenderContextSPtr()->AddShadowToNode(this);
	}
}

void CSceneNode3D::RemoveFromShadowPass()
{
	auto sptr = GetRenderContextSPtr();
	if ( sptr )
	{
		sptr->RemoveShadowFromNode(this);
	}

	InShadowPass_ = false;
}

void CSceneNode3D::AddToDepthPass()
{
	InDepthPass_ = true;
	
	if ( isVisible() )
	{
		GetRenderContextSPtr()->AddNodeToDepthPass(this);
	}
}

void CSceneNode3D::RemoveFromDepthPass()
{
	auto sptr = GetRenderContextSPtr();
	if ( sptr )
	{
		sptr->RemoveNodeFromDepthPass(this);
	}

	InDepthPass_ = false;
}

void CSceneNode3D::CreateTriSelector()
{
	assert(Mesh_);

	auto selector = SceneManager->createTriangleSelector(Mesh_, this);
	setTriangleSelector(selector);
	selector->drop();
}

void CSceneNode3D::setVisible( bool visible )
{
	ISceneNode::setVisible(visible);

	if ( isVisible() )
	{
		if ( InDepthPass_ )
		{
			GetRenderContextSPtr()->AddNodeToDepthPass(this);
		}

		if ( InShadowPass_ )
		{
			GetRenderContextSPtr()->AddShadowToNode(this);
		}
	}
	else
	{
		if ( InDepthPass_ )
		{
			GetRenderContextSPtr()->RemoveNodeFromDepthPass(this);
		}

		if ( InShadowPass_ )
		{
			GetRenderContextSPtr()->RemoveShadowFromNode(this);
		}
	}
}

void CSceneNode3D::SaveMaterialType()
{
	if ( !Mesh_ )
	{
		return;
	}

	SaveMaterialType_.clear();
	SaveMaterialType_.reallocate(Mesh_->getMeshBufferCount());
	for (unsigned index=0; index<Mesh_->getMeshBufferCount(); ++index )
	{
		SaveMaterialType_.push_back(Mesh_->getMeshBuffer(index)->getMaterial().MaterialType);
	}
}

void CSceneNode3D::ResetMaterialType()
{
	if ( !Mesh_ || SaveMaterialType_.empty() )
	{
		return;
	}

	assert(Mesh_->getMeshBufferCount()==SaveMaterialType_.size());

	for ( unsigned index=0; index<SaveMaterialType_.size(); ++index )
	{
		Mesh_->getMeshBuffer(index)->getMaterial().MaterialType = SaveMaterialType_[index];
	}
}

void CSceneNode3D::SetMaterialType( irr::video::E_MATERIAL_TYPE mt )
{
	if ( !Mesh_ )
	{
		return;
	}

	for ( unsigned index=0; index<Mesh_->getMeshBufferCount(); ++index )
	{
		Mesh_->getMeshBuffer(index)->getMaterial().MaterialType = mt;
	}
}
