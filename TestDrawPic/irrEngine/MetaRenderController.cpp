#include "stdafx.h"

#include "MetaRenderController.h"

#ifdef DBG_EVENT
#include "Irrlicht/os.h"
#endif

bool MetaRenderController::OnEvent( const irr::SEvent& event )
{
	return OnGUIEvent(event);
}

bool MetaRenderController::OnPreEvent( const irr::SEvent& event )
{
#ifdef DBG_EVENT
	irr::os::Printer::print("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
#endif // DBG_EVENT	

	for ( auto& controller : ControllerList_ )
	{

#ifdef DBG_EVENT
		std::string str = "OnPreEvent:";
		str += controller->GetName();
		irr::os::Printer::print(str.c_str());
#endif // DBG_EVENT	

		if ( controller->OnPreEvent(event) )
		{
			return true;
		}
	}

	return false;
}

bool MetaRenderController::OnPostEvent( const irr::SEvent& event )
{
	for ( auto& controller : ControllerList_ )
	{
#ifdef DBG_EVENT
		std::string str = "OnPostEvent:";
		str += controller->GetName();
		irr::os::Printer::print(str.c_str());
#endif
		if ( controller->OnPostEvent(event) )
		{
			return true;
		}
	}

	return false;
}

bool MetaRenderController::OnGUIEvent( const irr::SEvent& event )
{
	for ( auto& controller : ControllerList_ )
	{
#ifdef DBG_EVENT
		std::string str = "OnGUIEvent:";
		str += controller->GetName();
		irr::os::Printer::print(str.c_str());
#endif
		if ( controller->OnGUIEvent(event) )
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
		auto rc = GetRenderContextSPtr();
		controller->PreInit(rc);
		ControllerList_.push_back(controller);
		controller->SetParent(shared_from_this());
		controller->PostInit(rc);
	}
}