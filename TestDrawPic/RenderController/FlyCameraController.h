#ifndef FlyCameraController_h__
#define FlyCameraController_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "Irrlicht/ISceneNodeAnimatorFinishing.h"

class	FlyCameraController : public IRenderController
{
public:

	FlyCameraController();

public://IRenderController

	virtual	bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	OnPreEvent(const irr::SEvent& event);

public:

	bool	IsFlying() const;

	void	SetFlying(irr::scene::ICameraSceneNode* toSwitchCamera, irr::scene::ICameraSceneNode* activeCamera, irr::scene::ISceneNodeAnimatorFinishing* animator);

private:

	irr::scene::ICameraSceneNode*				ToSwitchCamera_;
	irr::scene::ICameraSceneNode*				ActiveCamera_;
	irr::scene::ISceneNodeAnimatorFinishing*	Animator_;
};

typedef	std::shared_ptr<FlyCameraController>	FlyCameraControllerSPtr;

#endif // FlyCameraController_h__
