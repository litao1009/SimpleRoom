#ifndef DoorMeshNode2D_h__
#define DoorMeshNode2D_h__

#pragma once

#include "IMeshSceneNode.h"
#include "SMeshBuffer.h"

class	DoorMeshNode2D : public irr::scene::IMeshSceneNode
{
public:

	DoorMeshNode2D(irr::scene::ISceneNode* parent, irr::s32 id = -1);
	~DoorMeshNode2D();

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

	void	UpdateMesh(float xLength, float zLength);

	void	SetLineColor(const irr::video::SColor& lineColor);

	void	SetDrawTriangleMesh(bool val) { DrawTriangle_ = val; }

private:

	irr::scene::SMeshBuffer*	RectBuffer_;
	irr::scene::SMeshBuffer*	CircleBuffer_;
	irr::core::matrix4			CircleTransform_;
	bool						DrawTriangle_;
};

#endif // DoorMeshNode2D_h__
