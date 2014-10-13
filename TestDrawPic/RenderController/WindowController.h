#ifndef WindowController_h__
#define WindowController_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

class	WindowController : public IRenderController
{

	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	enum EWindowState
	{
		EWS_NONE,
		EWS_CREATE,
		EWS_PICKING
	};

public:

	WindowController();
	~WindowController();

public://IRenderController

	virtual	void	Init();

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual	void	OnResize();

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	PreRender2D();

	virtual void	PostRender2D();

public:

	void	SetRootODL(CBaseODLWPtr root) { RootODL_ = root; }

public:

	EWindowState			State_;
	irr::core::vector2di	CursorIPos_;
	CBaseODLWPtr			RootODL_;
};

typedef	std::shared_ptr<WindowController>	WindowControllerSPtr;

#endif // WindowController_h__
