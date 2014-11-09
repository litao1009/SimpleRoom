#ifndef WallMeshNode2D_h__
#define WallMeshNode2D_h__

#pragma once

#include "IMeshSceneNode.h"
#include "SMeshBuffer.h"

#include "TopoDS_Shape.hxx"

class	WallMeshNode2D : public irr::scene::IMeshSceneNode
{
public:

	WallMeshNode2D(irr::scene::ISceneNode* parent, irr::s32 id = -1);
	~WallMeshNode2D();

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

	void	UpdateMesh(const TopoDS_Shape& wallBottomFace);

	void	SetSweeping(bool val);

	void	SetPicking(bool val);

private:

	irr::scene::IMeshBuffer*	FaceBuffer_;
	irr::scene::SMeshBuffer*	LineBuffer_;
};

#endif // WallMeshNode2D_h__
