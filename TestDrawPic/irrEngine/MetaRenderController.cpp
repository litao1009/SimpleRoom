#include "stdafx.h"

#include "MetaRenderController.h"

bool MetaRenderController::OnEvent( const irr::SEvent& event )
{
	for ( auto& controller : ControllerList_ )
	{
		if ( controller->OnEvent(event) )
		{
			return true;
		}
	}

	return false;
}

void MetaRenderController::OnResize(const SRenderContext& rc)
{
	for ( auto& controller : ControllerList_ )
	{
		controller->OnResize(rc);
	}
}

bool MetaRenderController::PreRender3D( const SRenderContext& rc )
{
	auto ret = true;

	for ( auto& controller : ControllerList_ )
	{
		ret &= controller->PreRender3D(rc);
	}

	return ret;
}

void MetaRenderController::PostRender3D( const SRenderContext& rc )
{
	for ( auto& controller : ControllerList_ )
	{
		controller->PostRender3D(rc);
	}
}

bool MetaRenderController::PreRender2D( const SRenderContext& rc )
{
	auto ret = true;

	for ( auto& controller : ControllerList_ )
	{
		ret &= controller->PreRender2D(rc);
	}

	return ret;
}

void MetaRenderController::PostRender2D( const SRenderContext& rc )
{
	for ( auto& controller : ControllerList_ )
	{
		controller->PostRender2D(rc);
	}
}

void MetaRenderController::PushController( IRenderControllerSPtr controller )
{
	if ( controller )
	{
		controller->PreInit(RC_.lock());
		ControllerList_.push_back(controller);
		controller->SetParent(shared_from_this());
		controller->PostInit(RC_.lock());
	}
}
