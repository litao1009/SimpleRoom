#ifndef TopViewController_h__
#define TopViewController_h__

#pragma once

#include "irrEngine/IRenderController.h"

class	TopViewController : public IRenderController
{
public:

	virtual	void	Init() {}

	virtual	bool	OnGUIEvent(const irr::SEvent& evt);

	virtual bool	OnPreEvent(const irr::SEvent& evt);

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual	void	OnResize();

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	PreRender2D();

	virtual void	PostRender2D();
};

#endif // TopViewController_h__
