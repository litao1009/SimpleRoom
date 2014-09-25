#ifndef MayaCameraController_h__
#define MayaCameraController_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "SceneNode/CDataSceneNodeFwd.h"

#include <vector>

class	MayaCameraController : public IRenderController
{
public:

	MayaCameraController();

	~MayaCameraController();

public:

	virtual bool	PreRender3D(const SRenderContext& rc);
	virtual void	PostRender3D(const SRenderContext& rc);

public:

	void	SetMayaCamera(ICameraSceneNodeSPtr camera);

private:

	ICameraSceneNodeSPtr			MayaCamera_;
	std::vector<CDataSceneNode*>	TmpNodes_;
	
};

typedef	std::shared_ptr<MayaCameraController>	MayaCameraControllerSPtr;

#endif // MayaCameraController_h__
