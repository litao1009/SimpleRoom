#ifndef SceneNode3D_h__
#define SceneNode3D_h__

#pragma once

#include "BaseSceneNode.h"

class	CSceneNode3D : public CBaseSceneNode
{
public:

	CSceneNode3D(SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr, std::string debugName="");
	~CSceneNode3D();

public://CBaseSceneNode

	virtual	ESceneNodeType	GetSceneNodeType() const { return CBaseSceneNode::ESNT_3D; }

public://ISceneNode

	virtual void OnRegisterSceneNode();

	virtual void render();

	virtual irr::video::SMaterial& getMaterial(irr::u32 i);

	virtual irr::u32 getMaterialCount() const;

	virtual const irr::core::aabbox3df& getBoundingBox() const;

	virtual irr::scene::ISceneNode* clone(irr::scene::ISceneNode* newParent, irr::scene::ISceneManager* newManager);

	virtual void setVisible(bool isVisible);

public://IMeshSceneNode

	virtual void setMesh(irr::scene::IMesh* mesh);

	virtual irr::scene::IMesh* getMesh();

	virtual irr::scene::IShadowVolumeSceneNode* addShadowVolumeSceneNode(const irr::scene::IMesh*, irr::s32, bool, irr::f32) { return nullptr; }

	virtual void setReadOnlyMaterials(bool readonly) {}

	virtual bool isReadOnlyMaterials() const { return false; }

public:

	void	AddToShadowPass();
	bool	IsInShadowPass() const { return InShadowPass_; }
	void	RemoveFromShadowPass();

	void	AddToDepthPass();
	bool	IsInDepthPass() const { return InDepthPass_; }
	void	RemoveFromDepthPass();

	void	CreateTriSelector();


private:

	bool					InShadowPass_;
	bool					InDepthPass_;

	irr::scene::IMesh*		Mesh_;

	irr::core::aabbox3df	Box_;
};

#endif // SceneNode3D_h__
