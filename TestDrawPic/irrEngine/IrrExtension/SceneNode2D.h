#ifndef SceneNode2D_h__
#define SceneNode2D_h__

#pragma once

#include "BaseSceneNode.h"

class	CSceneNode2D : public CBaseSceneNode
{
public:

	CSceneNode2D(SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr, std::string typeName="");
	~CSceneNode2D();

public://CBaseSceneNode

	virtual	ESceneNodeType	GetSceneNodeType() const { return CBaseSceneNode::ESNT_2D; }

public://ISceneNode

	virtual void OnRegisterSceneNode();

	virtual void render();

	virtual irr::video::SMaterial& getMaterial(irr::u32 i);

	virtual irr::u32 getMaterialCount() const;

	virtual const irr::core::aabbox3df& getBoundingBox() const;

	virtual irr::scene::ISceneNode* clone(irr::scene::ISceneNode* newParent, irr::scene::ISceneManager* newManager);

public://IMeshSceneNode

	virtual void setMesh(irr::scene::IMesh* mesh);

	virtual irr::scene::IMesh* getMesh();

	virtual irr::scene::IShadowVolumeSceneNode* addShadowVolumeSceneNode(const irr::scene::IMesh*, irr::s32, bool, irr::f32) { return nullptr; }

	virtual void setReadOnlyMaterials(bool readonly) {}

	virtual bool isReadOnlyMaterials() const { return false; }

public:

	void	CreateTriSelector();


private:

	irr::scene::IMesh*		Mesh_;
	irr::core::aabbox3df	Box_;
};

#endif // SceneNode2D_h__
