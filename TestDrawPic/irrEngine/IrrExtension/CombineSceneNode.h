#ifndef CombineSceneNode_h__
#define CombineSceneNode_h__

#pragma once

#include "BaseSceneNode.h"
#include "SceneNode2D.h"
#include "SceneNode3D.h"

class	CCombineSceneNode;
typedef	std::shared_ptr<CCombineSceneNode>	CombineSceneNodeSPtr;

class	CCombineSceneNode : public CBaseSceneNode
{
public:

	CCombineSceneNode(SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr);
	~CCombineSceneNode();

public:

	static	CombineSceneNodeSPtr	Create(SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr);

public://CBaseSceneNode

	virtual	ESceneNodeType	GetSceneNodeType() const { return CBaseSceneNode::ESNT_COMBINE; }

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

	virtual irr::scene::IShadowVolumeSceneNode* addShadowVolumeSceneNode(const irr::scene::IMesh*, irr::s32, bool, irr::f32);

	virtual void setReadOnlyMaterials(bool readonly);

	virtual bool isReadOnlyMaterials() const;

public:

	void	AddToShadowPass();
	bool	IsInShadowPass() const;
	void	RemoveFromShadowPass();

	void	AddToDepthPass();
	bool	IsInDepthPass() const;
	void	RemoveFromDepthPass();

	void	CreateTriSelector();

	void	Init();

	CSceneNode3D*	GetSceneNode2D() const { return Node3D_; }
	CSceneNode2D*	GetSceneNode3D() const { return Node2D_; }

private:

	CSceneNode2D*		Node2D_;
	CSceneNode3D*		Node3D_;
};
#endif // CombineSceneNode_h__
