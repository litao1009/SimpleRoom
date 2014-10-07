#ifndef GridController_h__
#define GridController_h__

#include "IrrEngine/IRenderController.h"
#include "ISceneNode.h"

#pragma once

class	GridController : public IRenderController
{
public:

	GridController();

	~GridController();

public:

	virtual	void	Init();

	virtual bool	OnPreEvent(const irr::SEvent& evt);

	virtual bool	PreRender3D();

public:

	float			GetGridSize() const { return GridSize_; }

	void			SetGridSize(float val) { GridSize_ = val; }

public:

	const irr::core::vector3df&	GetGridPos() const { return GridPos_; }

private:

	float						GridSize_;
	irr::scene::ISceneNode*		Node_;
	irr::core::vector3df		GridPos_;
	irr::core::vector2di		CursorIPos_;
	bool						Ignore_;
};
#endif // GridController_h__
