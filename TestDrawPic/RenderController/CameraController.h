#ifndef CameraController_h__
#define CameraController_h__

#pragma once

#include "irrEngine/IRenderController.h"

class	CameraController : public IRenderController
{
public:

	CameraController();

	~CameraController();

public://IRenderController

	virtual	void	Init();

	virtual	void	OnResize();

public:

	ICameraSceneNodeSPtr	GetFPSCamera() const { return FPSCamera_; }

	ICameraSceneNodeSPtr	GetMayaCamera() const { return MayaCamera_; }

	ICameraSceneNodeSPtr	GetTopCamera() const { return TopCamera_; }

private:

	ICameraSceneNodeSPtr		FPSCamera_;
	ICameraSceneNodeSPtr		MayaCamera_;
	ICameraSceneNodeSPtr		TopCamera_;
};

typedef	std::shared_ptr<CameraController>	CameraControllerSPtr;

#endif // CameraController_h__
