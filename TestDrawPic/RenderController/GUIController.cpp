#include "stdafx.h"

#include "GUIController.h"
#include "irrlicht.h"
#include "IrrEngine/SRenderContext.h"

#include "StatusMgr.h"

#include <functional>

static const auto ImageBorderX = 3;
static const auto ImageBorderY = 3;

using namespace irr;
using namespace gui;

static	CGUIImageButton*	AddImage( IGUIEnvironment* env, video::ITexture* tex, core::position2di pos = core::position2di(0,0), IGUIElement* parent=0, irr::s32 id=-1, bool useAlphaChannel=true )
{
	auto sz = tex->getOriginalSize();

	auto img = new CGUIImageButton(env, parent ? parent : env->getRootGUIElement(), -1, core::recti(pos, sz));
	img->setUseAlphaChannel(true);
	img->setImage(tex);
	img->drop();

	return img;
}


GUIController::GUIController(const SRenderContextWPtr& rc):IRenderController(rc)
{
	Icon_ = irr::gui::ECI_NORMAL;
	Minimum_ = false;
	CameraVisible_ = true;
	CameraRestore_ = true;
	TopCameraActive_ = true;
	MayaCameraActive_ = false;
	FPSCameraActive_ = false;
	RealWorld_ = false;

	SetName("GUIController");
}


void GUIController::Init()
{
	auto sprc = GetRenderContextSPtr();

	auto driver = sprc->Smgr_->getVideoDriver();

	sprc->SetOnResize();

	{//camera
		auto showTex = driver->getTexture("../Data/Resource/3D/camera.png");
		auto hideTex = driver->getTexture("../Data/Resource/3D/camera_d.png");

		auto guiImage = AddImage(sprc->GUIEnv_.get(), showTex);

		ShowCameraImage_.reset(guiImage, std::bind([](IGUIElement*){}, std::placeholders::_1));

		ShowCameraTex_.reset(showTex, [](video::ITexture*){});
		HideCameraTex_.reset(hideTex, [](video::ITexture*){});
	}

	{//camera type
		auto fpsActTex = driver->getTexture("../Data/Resource/3D/fps_cam_a.png");
		auto fpsDActTex = driver->getTexture("../Data/Resource/3D/fps_cam_d.png");
		auto mayaActTex = driver->getTexture("../Data/Resource/3D/maya_cam_a.png");
		auto mayaDActTex = driver->getTexture("../Data/Resource/3D/maya_cam_d.png");
		auto topActTex = driver->getTexture("../Data/Resource/3D/top_cam_a.png");
		auto topDActTex = driver->getTexture("../Data/Resource/3D/top_cam_d.png");

		auto fpsImage = AddImage(sprc->GUIEnv_.get(), fpsDActTex);
		auto mayaImage = AddImage(sprc->GUIEnv_.get(), mayaDActTex);
		auto topImage = AddImage(sprc->GUIEnv_.get(), topActTex);

		FPSCameraImage_.reset(fpsImage, [](IGUIImage*){});
		MayaCameraImage_.reset(mayaImage, [](IGUIImage*){});
		TopCameraImage_.reset(topImage, [](IGUIImage*){});

		MayaActiveTex_.reset(mayaActTex, [](video::ITexture*){});
		MayaDActiveTex_.reset(mayaDActTex, [](video::ITexture*){});
		FPSActiveTex_.reset(fpsActTex, [](video::ITexture*){});
		FPSDActiveTex_.reset(fpsDActTex, [](video::ITexture*){});
		TopActiveTex_.reset(topActTex, [](video::ITexture*){});
		TopDActiveTex_.reset(topDActTex, [](video::ITexture*){});
	}

	{//real
		auto realTex = driver->getTexture("../Data/Resource/3D/real_a.png");
		auto realDTex = driver->getTexture("../Data/Resource/3D/real_d.png");
		auto guiImage = AddImage(sprc->GUIEnv_.get(), realDTex);
		RealActiveTex_.reset(realTex, [](video::ITexture*){});
		RealDActiveTex_.reset(realDTex, [](video::ITexture*){});
		RealImage_.reset(guiImage, std::bind([](IGUIElement*){}, std::placeholders::_1));
	}
}

bool GUIController::OnPreEvent( const irr::SEvent& event )
{
// 	auto sprc = GetRenderContextSPtr();
// 	if ( sprc )
// 	{
// 		sprc->CursorControl_->setActiveIcon(Icon_);
// 	}

	return false;
}

bool GUIController::OnGUIEvent( const irr::SEvent& event )
{
	if ( !IsEnable() )
	{
		return false;
	}

	auto ret = false;

	if ( event.EventType == EET_GUI_EVENT )
	{
		//CameraVisible
		if ( event.GUIEvent.Caller == this->ShowCameraImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				CameraVisible_ = !CameraVisible_;
				CameraRestore_ = CameraVisible_;

				ret = true;
			}
		}

		//Camera
		if ( event.GUIEvent.Caller == this->TopCameraImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				TopCameraImage_->setImage(TopActiveTex_.get());
				FPSCameraImage_->setImage(FPSDActiveTex_.get());
				MayaCameraImage_->setImage(MayaDActiveTex_.get());
				TopCameraActive_ = true;
				MayaCameraActive_ = false;
				FPSCameraActive_ = false;

				ret = true;
			}
		}

		if ( event.GUIEvent.Caller == this->FPSCameraImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				TopCameraImage_->setImage(TopDActiveTex_.get());
				FPSCameraImage_->setImage(FPSActiveTex_.get());
				MayaCameraImage_->setImage(MayaDActiveTex_.get());
				TopCameraActive_ = false;
				MayaCameraActive_ = false;
				FPSCameraActive_ = true;

				ret = true;
			}
		}

		if ( event.GUIEvent.Caller == this->MayaCameraImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				TopCameraImage_->setImage(TopDActiveTex_.get());
				FPSCameraImage_->setImage(FPSDActiveTex_.get());
				MayaCameraImage_->setImage(MayaActiveTex_.get());
				TopCameraActive_ = false;
				MayaCameraActive_ = true;
				FPSCameraActive_ = false;

				ret = true;
			}
		}

		//Real
		if ( event.GUIEvent.Caller == this->RealImage_.get() )
		{
			if ( event.GUIEvent.EventType == (EGUI_EVENT_TYPE)CGUIImageButton::EIS_PRESSED )
			{
				RealWorld_ = !RealWorld_;
				RealImage_->setImage(RealWorld_ ? RealActiveTex_.get() : RealDActiveTex_.get());

				ret = true;
			}
		}
	}

	auto sprc = GetRenderContextSPtr();
	if ( sprc )
	{
		sprc->CursorControl_->setActiveIcon(Icon_);
	}

	return ret;
}

void GUIController::OnResize()
{
	auto renderSize = GetRenderContextSPtr()->Smgr_->getVideoDriver()->getScreenSize();

	{
		auto texSize = ShowCameraImage_->getImage()->getSize();
		irr::core::position2di pos;
		if ( renderSize.Width > texSize.Width )
		{
			pos.X = (renderSize.Width-texSize.Width) / 2;
		}
		else
		{
			pos.X = 0;
		}
		pos.Y = ImageBorderY;
		ShowCameraImage_->setRelativePosition(pos);
	}

	{
		auto texSize = MayaActiveTex_->getSize();
		FPSCameraImage_->setRelativePosition(irr::core::vector2di(renderSize.Width-texSize.Width-ImageBorderX,renderSize.Height-texSize.Height-ImageBorderY));
		MayaCameraImage_->setRelativePosition(irr::core::vector2di(renderSize.Width-texSize.Width-ImageBorderX,renderSize.Height-texSize.Height*2-ImageBorderY));
		TopCameraImage_->setRelativePosition(irr::core::vector2di(renderSize.Width-texSize.Width-ImageBorderX,renderSize.Height-texSize.Height*3-ImageBorderY));
		RealImage_->setRelativePosition(irr::core::vector2di(renderSize.Width-texSize.Width-ImageBorderX,renderSize.Height-texSize.Height*5-ImageBorderY));
	}
}

bool GUIController::PreRender3D()
{
	StatusMgr::GetInstance().RealWorld_ = IsTopCameraActive() && RealWorld_;

	return Minimum_;
}

void GUIController::PostRender3D()
{

}

bool GUIController::PreRender2D()
{
	ShowCameraImage_->setVisible(true);
	FPSCameraImage_->setVisible(true);
	MayaCameraImage_->setVisible(true);
	TopCameraImage_->setVisible(true);
	RealImage_->setVisible(IsTopCameraActive());

	if ( CameraVisible_ )
	{
		ShowCameraImage_->setImage(ShowCameraTex_.get());
	}
	else
	{
		ShowCameraImage_->setImage(HideCameraTex_.get());
	}

	return false;
}

void GUIController::PostRender2D()
{

}