#ifndef CreatingDoorController_h__
#define CreatingDoorController_h__

#pragma once

#include "irrEngine/IRenderController.h"

class	CreatingDoorController : public IRenderController
{
public:

	CreatingDoorController();
	~CreatingDoorController();

public:

	virtual	void	Init();

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual	void	OnResize();

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	PreRender2D();

	virtual void	PostRender2D();
};

#endif // CreatingDoorController_h__
