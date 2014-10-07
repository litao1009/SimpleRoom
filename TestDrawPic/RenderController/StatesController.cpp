#include "stdafx.h"

#include "StatesController.h"
#include "irrEngine/MetaRenderController.h"

#include <map>
#include <vector>

class	StatesController::Imp
{
public:

	Imp()
	{
		CurrentState_ = ERS_COUNT;
	}

	ERenderState										CurrentState_;
	std::map<ERenderState,MetaRenderControllerSPtr>		ControllerMap_;
};

StatesController::StatesController():ImpUPtr_(new Imp)
{

}

StatesController::~StatesController()
{

}

void StatesController::Init()
{
	
}


bool StatesController::OnGUIEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	return imp_.ControllerMap_[imp_.CurrentState_]->OnGUIEvent(evt);
}

bool StatesController::OnPreEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	return imp_.ControllerMap_[imp_.CurrentState_]->OnPreEvent(evt);
}

bool StatesController::OnPostEvent( const irr::SEvent& evt )
{
	auto& imp_ = *ImpUPtr_;

	return imp_.ControllerMap_[imp_.CurrentState_]->OnPostEvent(evt);
}

void StatesController::OnResize()
{
	auto& imp_ = *ImpUPtr_;

	imp_.ControllerMap_[imp_.CurrentState_]->OnResize();
}

bool StatesController::PreRender3D()
{
	auto& imp_ = *ImpUPtr_;

	return imp_.ControllerMap_[imp_.CurrentState_]->PreRender3D();
}

void StatesController::PostRender3D()
{
	auto& imp_ = *ImpUPtr_;

	imp_.ControllerMap_[imp_.CurrentState_]->PostRender3D();
}

bool StatesController::PreRender2D()
{
	auto& imp_ = *ImpUPtr_;

	return imp_.ControllerMap_[imp_.CurrentState_]->PreRender2D();
}

void StatesController::PostRender2D()
{
	auto& imp_ = *ImpUPtr_;

	imp_.ControllerMap_[imp_.CurrentState_]->PostRender2D();
}

void StatesController::AddController( const ERenderState& state, const IRenderControllerSPtr& controller )
{
	auto& imp_ = *ImpUPtr_;

	if ( !imp_.ControllerMap_[state] )
	{
		imp_.ControllerMap_[state] = std::make_shared<MetaRenderController>();
		imp_.ControllerMap_[state]->SetRenderContextWPtr(GetRenderContextSPtr());
	}

	ImpUPtr_->ControllerMap_[state]->PushController(controller);
}

ERenderState StatesController::GetRenderState() const
{
	return ImpUPtr_->CurrentState_;
}


void StatesController::SetRenderState( ERenderState ers )
{
	ImpUPtr_->CurrentState_ = ers;
}
