#include "stdafx.h"

#include "SceneNode2D.h"

#include "irrlicht.h"
#include "irrEngine/SRenderContext.h"

#include <assert.h>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;


CSceneNode2D::CSceneNode2D(SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr, std::string typeName):
	CBaseSceneNode(rcWPtr, odlWPtr, typeName)
{
	Mesh_ = nullptr;
}

CSceneNode2D::~CSceneNode2D()
{
	if (Mesh_)
	{
		Mesh_->drop();
	}
}

void CSceneNode2D::render()
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

const aabbox3df& CSceneNode2D::getBoundingBox() const
{
	return Mesh_ ? Mesh_->getBoundingBox() : Box_;
}

void CSceneNode2D::OnRegisterSceneNode()
{
	if ( !IsVisible )
	{
		return;
	}

	SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}


irr::video::SMaterial& CSceneNode2D::getMaterial( u32 i )
{
	if (Mesh_ && i<Mesh_->getMeshBufferCount())
	{
		return Mesh_->getMeshBuffer(i)->getMaterial();
	}

	return ISceneNode::getMaterial(i);
}

irr::u32 CSceneNode2D::getMaterialCount() const
{
	if (Mesh_)
	{
		return Mesh_->getMeshBufferCount();
	}

	return ISceneNode::getMaterialCount();
}

void CSceneNode2D::setMesh( irr::scene::IMesh* mesh )
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
}

irr::scene::ISceneNode* CSceneNode2D::clone( irr::scene::ISceneNode* newParent, irr::scene::ISceneManager* newManager )
{
	return nullptr;
}

irr::scene::IMesh* CSceneNode2D::getMesh()
{
	return Mesh_;
}

void CSceneNode2D::CreateTriSelector()
{
	assert(Mesh_);

	auto selector = SceneManager->createTriangleSelector(Mesh_, this);
	setTriangleSelector(selector);
	selector->drop();
}