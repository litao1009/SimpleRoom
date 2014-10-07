#ifndef StatesController_h__
#define StatesController_h__

#pragma once

#include "irrEngine/IRenderController.h"

enum ERenderState
{
	ERS_TOP_VIEW,
	ERS_MAYA_VIEW,
	ERS_FPS_VIEW,
	ERS_ANIMATION,
	ERS_EDIT,
	ERS_COUNT
};

class	StatesController : public IRenderController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	StatesController();
	~StatesController();

public://IRenderController

	virtual	void	Init();

	virtual	bool	OnGUIEvent(const irr::SEvent& evt);

	virtual bool	OnPreEvent(const irr::SEvent& evt);

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual	void	OnResize();

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	PreRender2D();

	virtual void	PostRender2D();

public:

	void	AddController(const ERenderState& state, const IRenderControllerSPtr& controller);

	ERenderState	GetRenderState() const;

	void			SetRenderState(ERenderState ers);
};

typedef	std::shared_ptr<StatesController>	StatesControllerSPtr;

#endif // StatesController_h__
