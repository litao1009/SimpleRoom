#include "stdafx.h"

#include "MetaRenderController.h"

#ifdef DBG_EVENT
#include "Irrlicht/os.h"
#endif


MetaRenderController::MetaRenderController():IRenderController(SRenderContextWPtr())
{

}

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

void MetaRenderController::OnResize()
{
	for ( auto& controller : ControllerList_ )
	{
		controller->OnResize();
	}
}

bool MetaRenderController::PreRender3D()
{
	auto ret = false;

	for ( auto& controller : ControllerList_ )
	{
		ret |= controller->PreRender3D();
	}

	return ret;
}

void MetaRenderController::PostRender3D()
{
	for ( auto& controller : ControllerList_ )
	{
		controller->PostRender3D();
	}
}

bool MetaRenderController::PreRender2D()
{
	auto ret = false;

	for ( auto& controller : ControllerList_ )
	{
		ret |= controller->PreRender2D();
	}

	return ret;
}

void MetaRenderController::PostRender2D()
{
	for ( auto& controller : ControllerList_ )
	{
		controller->PostRender2D();
	}
}

void MetaRenderController::PushController( IRenderControllerSPtr controller )
{
#ifdef _DEBUG
	assert(ControllerList_.end() == std::find(ControllerList_.begin(), ControllerList_.end(), controller));
#endif

	if ( controller )
	{
		ControllerList_.push_back(controller);

		if ( !controller->HasInit() )
		{
			controller->Init();
			controller->SetHasInit(true);
		}
		
	}
}