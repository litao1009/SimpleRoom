#pragma once

#include "IMeshSceneNode.h"
#include "SMeshBuffer.h"

class	CornerMeshNode2D : public irr::scene::IMeshSceneNode
{
	class	Imp;

public:

	CornerMeshNode2D(irr::scene::ISceneNode* parent, irr::s32 id = -1);
	~CornerMeshNode2D();

public://ISceneNode

	virtual void OnRegisterSceneNode();

	virtual void render();

	virtual irr::video::SMaterial& getMaterial(irr::u32 i);

	virtual irr::u32 getMaterialCount() const;

	virtual const irr::core::aabbox3df& getBoundingBox() const;

public://IMeshSceneNode

	virtual void setMesh(irr::scene::IMesh* mesh) {}

	virtual irr::scene::IMesh* getMesh() { return nullptr; }

	virtual irr::scene::IShadowVolumeSceneNode* addShadowVolumeSceneNode(const irr::scene::IMesh*, irr::s32, bool, irr::f32) { return nullptr; }

	virtual void setReadOnlyMaterials(bool readonly) {}

	virtual bool isReadOnlyMaterials() const { return false; }

public:

	static	void	SetRadius(float scale);

private:

	irr::scene::SMeshBuffer*	MeshBuffer_;
};
