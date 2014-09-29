#include "stdafx.h"

#include "CombineSceneNode.h"

#include "SceneNode2D.h"
#include "SceneNode3D.h"

#include "ISceneManager.h"

#include "ODL/BaseODL.h"

CCombineSceneNode::CCombineSceneNode( SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr)
	:CBaseSceneNode(rcWPtr, odlWPtr)
{
	Node2D_ = new CSceneNode2D(rcWPtr, odlWPtr);
	Node3D_ = new CSceneNode3D(rcWPtr, odlWPtr);

	addChild(Node2D_);
	addChild(Node3D_);

	Node2D_->drop();
	Node3D_->drop();
}

CCombineSceneNode::~CCombineSceneNode()
{
	
}

void CCombineSceneNode::OnRegisterSceneNode()
{
	if ( !IsVisible )
	{
		return;
	}

	SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void CCombineSceneNode::render()
{

}

irr::video::SMaterial& CCombineSceneNode::getMaterial( irr::u32 i )
{
	return Node3D_->getMaterial(i);
}

irr::u32 CCombineSceneNode::getMaterialCount() const
{
	return Node3D_->getMaterialCount();
}

const irr::core::aabbox3df& CCombineSceneNode::getBoundingBox() const
{
	return Node3D_->getBoundingBox();
}

irr::scene::ISceneNode* CCombineSceneNode::clone( irr::scene::ISceneNode* newParent, irr::scene::ISceneManager* newManager )
{
	return nullptr;
}

void CCombineSceneNode::setVisible( bool visible )
{
	Node3D_->setVisible(visible);
	ISceneNode::setVisible(visible);
}

void CCombineSceneNode::setMesh( irr::scene::IMesh* mesh )
{
	Node3D_->setMesh(mesh);
}

irr::scene::IMesh* CCombineSceneNode::getMesh()
{
	return Node3D_->getMesh();
}

irr::scene::IShadowVolumeSceneNode* CCombineSceneNode::addShadowVolumeSceneNode( const irr::scene::IMesh* mesh, irr::s32 id, bool zf, irr::f32 i)
{
	return Node3D_->addShadowVolumeSceneNode(mesh, id, zf, i);
}

void CCombineSceneNode::setReadOnlyMaterials( bool readonly )
{
	Node3D_->setReadOnlyMaterials(readonly);
}

bool CCombineSceneNode::isReadOnlyMaterials() const
{
	return Node3D_->isReadOnlyMaterials();
}

void CCombineSceneNode::AddToShadowPass()
{
	Node3D_->AddToShadowPass();
}

bool CCombineSceneNode::IsInShadowPass() const
{
	return Node3D_->IsInShadowPass();
}

void CCombineSceneNode::RemoveFromShadowPass()
{
	Node3D_->RemoveFromShadowPass();
}

void CCombineSceneNode::AddToDepthPass()
{
	Node3D_->AddToDepthPass();
}

bool CCombineSceneNode::IsInDepthPass() const
{
	return Node3D_->IsInDepthPass();
}

void CCombineSceneNode::RemoveFromDepthPass()
{
	Node3D_->RemoveFromDepthPass();
}

void CCombineSceneNode::CreateTriSelector()
{
	Node3D_->CreateTriSelector();
}

void CCombineSceneNode::Init()
{

}

CombineSceneNodeSPtr CCombineSceneNode::Create( SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr )
{
	auto newNode = new CCombineSceneNode(rcWPtr, odlWPtr);
	CombineSceneNodeSPtr sptr(newNode, [](CCombineSceneNode* p){ p->drop(); });

	return sptr;
}
