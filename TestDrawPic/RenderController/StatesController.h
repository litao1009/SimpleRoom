#ifndef StatesController_h__
#define StatesController_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "irrEngine/MetaRenderController.h"

#include <map>

template<typename State>
class	StatesController : public IRenderController
{
public:

	typedef	typename std::shared_ptr<StatesController<State>>	SPtr;

public://IRenderController

	virtual	void	Init()
	{

	}

	virtual	bool	OnGUIEvent(const irr::SEvent& evt)
	{
		return StateControllers_[CurrentState_]->OnGUIEvent(evt);
	}

	virtual bool	OnPreEvent(const irr::SEvent& evt)
	{
		return StateControllers_[CurrentState_]->OnPreEvent(evt);
	}

	virtual	bool	OnPostEvent(const irr::SEvent& evt)
	{
		return StateControllers_[CurrentState_]->OnPostEvent(evt);
	}

	virtual	void	OnResize()
	{
		StateControllers_[CurrentState_]->OnResize();
	}

	virtual bool	PreRender3D()
	{
		return StateControllers_[CurrentState_]->PreRender3D();
	}

	virtual void	PostRender3D()
	{
		StateControllers_[CurrentState_]->PostRender3D();
	}

	virtual bool	PreRender2D()
	{
		return StateControllers_[CurrentState_]->PreRender2D();
	}

	virtual void	PostRender2D()
	{
		StateControllers_[CurrentState_]->PostRender2D();
	}

public:

	void			AddController(const State& state, const IRenderControllerSPtr& controller)
	{
		if ( !StateControllers_[state] )
		{
			StateControllers_[state] = std::make_shared<MetaRenderController>();
			StateControllers_[state]->SetRenderContextWPtr(GetRenderContextSPtr());
		}

		StateControllers_[state]->PushController(controller);
	}

	State			GetCurrentState() const { return CurrentState_; }

	void			SetCurrentState(const State& state) { CurrentState_ = state; }

private:

	State											CurrentState_;
	std::map<State,MetaRenderControllerSPtr>		StateControllers_;
};

#endif // StatesController_h__
