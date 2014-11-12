#include "stdafx.h"

#include "SceneNode2D.h"

#include "irrlicht.h"
#include "irrEngine/SRenderContext.h"

#include <assert.h>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

CSceneNode2D::CSceneNode2D(SRenderContextWPtr rcWPtr, BaseODLWPtr odlWPtr):
	CBaseSceneNode(rcWPtr, odlWPtr)
{
	
}

CSceneNode2D::~CSceneNode2D()
{

}

void CSceneNode2D::render()
{

}

const aabbox3df& CSceneNode2D::getBoundingBox() const
{
	return Box_;
}

void CSceneNode2D::OnRegisterSceneNode()
{
	if ( !IsVisible )
	{
		return;
	}

	if ( GetRenderMode() & ESNT_2D )
	{
		ISceneNode::OnRegisterSceneNode();
	}
}


irr::video::SMaterial& CSceneNode2D::getMaterial( u32 i )
{
	return ISceneNode::getMaterial(i);
}

irr::u32 CSceneNode2D::getMaterialCount() const
{
	return ISceneNode::getMaterialCount();
}

void CSceneNode2D::setMesh( irr::scene::IMesh* mesh )
{

}

irr::scene::ISceneNode* CSceneNode2D::clone( irr::scene::ISceneNode* newParent, irr::scene::ISceneManager* newManager )
{
	return nullptr;
}

irr::scene::IMesh* CSceneNode2D::getMesh()
{
	return nullptr;
}