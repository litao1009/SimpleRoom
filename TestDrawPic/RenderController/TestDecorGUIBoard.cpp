#include "stdafx.h"

#include "TestDecorGUIBoard.h"
#include "irrEngine/SRenderContext.h"

#include "irrEngine/IrrExtension/GUIImageButton.h"

#include "StatusMgr.h"

using namespace irr;
using namespace gui;
using namespace video;
using namespace core;

class	TestDecorGUIBoard::Imp
{
public:

	CGUIImageButton*	DoorImage_;
	CGUIImageButton*	WindowImage_;
	CGUIImageButton*	BayWindowImage_;

	static	CGUIImageButton*	AddImage( IGUIEnvironment* env, ITexture* tex, position2di pos = core::position2di(0,0), IGUIElement* parent=0, irr::s32 id=-1, bool useAlphaChannel=true )
	{
		auto sz = tex->getOriginalSize();

		auto img = new CGUIImageButton(env, parent ? parent : env->getRootGUIElement(), -1, core::recti(pos, sz));
		img->setUseAlphaChannel(true);
		img->setImage(tex);
		img->drop();

		return img;
	}

};

TestDecorGUIBoard::TestDecorGUIBoard():ImpUPtr_(new Imp)
{

}

TestDecorGUIBoard::~TestDecorGUIBoard()
{

}

void TestDecorGUIBoard::Init()
{
	auto sprc = GetRenderContextSPtr();
	auto driver = sprc->Smgr_->getVideoDriver();
	auto& imp_ = *ImpUPtr_;

	{//ÃÅ
		auto tex = driver->getTexture("../Data/Resource/3D/test_Door.png");
		imp_.DoorImage_ = Imp::AddImage(sprc->GUIEnv_.get(), tex, position2di(10,64));
	}

	{//´°
		auto tex = driver->getTexture("../Data/Resource/3D/test_Window.png");
		imp_.WindowImage_ = Imp::AddImage(sprc->GUIEnv_.get(), tex, position2di(10,128));
	}

	{//Æ®´°
		auto tex = driver->getTexture("../Data/Resource/3D/test_BayWindow.png");
		imp_.BayWindowImage_ = Imp::AddImage(sprc->GUIEnv_.get(), tex, position2di(10, 192));
	}
}

bool TestDecorGUIBoard::OnGUIEvent( const irr::SEvent& evt )
{
	if ( evt.EventType != EET_GUI_EVENT )
	{
		return false;
	}

	auto& imp_ = *ImpUPtr_;

	//door
	if ( evt.GUIEvent.Caller == imp_.DoorImage_ )
	{
		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_NORMAL )
		{
			
		}

		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_HOVERD )
		{
			
		}

		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
		{

		}

		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_LEFTUP )
		{
			StatusMgr::GetInstance().Test_CreateDoor_ = true;
		}
	}

	//window
	if ( evt.GUIEvent.Caller == imp_.WindowImage_ )
	{
		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_NORMAL )
		{

		}

		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_HOVERD )
		{

		}

		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
		{

		}

		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_LEFTUP )
		{
			StatusMgr::GetInstance().Test_CreateWindow_ = true;
		}
	}

	//bayWindow
	if ( evt.GUIEvent.Caller == imp_.BayWindowImage_ )
	{
		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_NORMAL )
		{

		}

		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_HOVERD )
		{

		}

		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
		{

		}

		if ( evt.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_LEFTUP )
		{
			StatusMgr::GetInstance().Test_CreateBayWindow_ = true;
		}
	}

	return false;
}

bool TestDecorGUIBoard::PreRender2D()
{
	return false;
}

void TestDecorGUIBoard::PostRender2D()
{

}
